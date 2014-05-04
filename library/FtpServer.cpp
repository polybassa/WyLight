/**
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.
 
 This file is part of Wifly_Light.
 
 Wifly_Light is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Wifly_Light is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */

#include "FtpServer.h"
#include "trace.h"
#include <fstream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sstream>

namespace WyLight {

#define FTP_PORT 60021
	
	static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

	static const timeval RESPONSE_TIMEOUT = {5, 0};
	static const uint32_t LOCALHOST = 2130706433;
	
	FtpServer::FtpServer(void) throw (FatalError)
	{
		mFtpServerThread = std::thread([&]{
			TcpServerSocket telnetListener(INADDR_ANY, FTP_PORT);
			while (mFtpServerRunning) {
				Trace(ZONE_VERBOSE, "FtpServer running\n");
				TcpSocket telnet(telnetListener.GetSocket());
				if (!mFtpServerRunning) {
					return;
				}

				Trace(ZONE_VERBOSE, "Starting client Thread\n");
				try {
					handleFiletransfer(telnet);
				} catch (FatalError &e) {
					Trace(ZONE_ERROR, "%s\n", e.what());
				}
			}
		});
	}
	
	FtpServer::~FtpServer(void) {
		mFtpServerRunning = false;

		TcpSocket shutdownSocket(LOCALHOST, FTP_PORT);
		mFtpServerThread.join();
	}

	void FtpServer::handleFiletransfer(const TcpSocket& telnet) {
		size_t bytesRead = 0;
		uint8_t buffer[1024];
		std::stringstream dataInput;
		bool isClientLoggedIn = false;
		TcpServerSocket *dataSocket = NULL;
		
		telnet.Send("220 WyLight Ftp Server running.\r\n");
		
		while (true) {
			
			if ( (bytesRead = telnet.Recv(reinterpret_cast<uint8_t *>(buffer), sizeof(buffer))) == 0) {
				throw FatalError("FtpServer: read error: " + std::to_string(errno));
			}
			
			dataInput.write(reinterpret_cast<const char *>(buffer), bytesRead);
			
			std::string requestCMD;
			dataInput >> requestCMD;
			
			if (requestCMD == "USER")
			{
				//=============================
				std::string userName;
				std::getline(dataInput, userName);
				
				if (userName.find("roving") != std::string::npos) {
					telnet.Send("331 Username ok, send password.\r\n");
				} else {
					telnet.Send("430 Invalid username or password. Good Bye.\r\n");
					return;
				}
			} else if (requestCMD == "PASS")
			{
				//=============================
				std::string password;
				std::getline(dataInput, password);
				
				if (password.find("Pass123") != std::string::npos) {
					telnet.Send("230 Login successful.\r\n");
					isClientLoggedIn = true;
				} else {
					telnet.Send("430 Invalid username or password. Good Bye.\r\n");
					return;
				}
			} else if (requestCMD == "CWD" && isClientLoggedIn)
			{
				//=============================
				std::string directory;
				std::getline(dataInput, directory);
				
				if (directory.find("public") != std::string::npos) {
					telnet.Send("250 \"/public\" ist the current directory.\r\n");
				} else {
					telnet.Send("550 Requested action not taken. File unavailable (e.g., file not found, no access). Good Bye.\r\n");
					return;
				}
			} else if (requestCMD == "TYPE" && isClientLoggedIn)
			{
				//=============================
				std::string dataType;
				std::getline(dataInput, dataType);
				
				if (dataType.find("I") != std::string::npos) {
					telnet.Send("200 Type set to: Binary.\r\n");
				} else {
					telnet.Send("550 Requested action not taken. File unavailable (e.g., file not found, no access). Good Bye.\r\n");
					return;
				}
			} else if (requestCMD == "PASV" && isClientLoggedIn)
			{
				//=============================
				//clear content of dataInput
				dataInput.str(std::string());
				
				try {
					dataSocket = openDataConnection(telnet);
				} catch (FatalError& e) {
					telnet.Send(std::string("451 Internal error - ") + std::string(e.what()));
				}

				uint16_t port = dataSocket->GetPort();
				uint32_t ip = dataSocket->GetIp();

				telnet.Send("227 Entering pasive mode ("
					+ std::to_string((ip >> 24) & 0xff) + ","
					+ std::to_string((ip >> 16) & 0xff) + ","
					+ std::to_string((ip >>  8) & 0xff) + ","
					+ std::to_string((ip      ) & 0xff) + ","
					+ std::to_string(port / 256) + ","
					+ std::to_string(port % 256) + ").\r\n");
			} else if (requestCMD == "RETR" && isClientLoggedIn)
			{
				std::string fileName;
				std::getline(dataInput, fileName);
				
				//TODO: get the right file !!!!
				char currentDirectory[FILENAME_MAX];
				
				getcwd(currentDirectory, FILENAME_MAX);
							
				//FIXME: static filename only for debugging
				//fileName = "/Users/nilsweiss/Dropbox/Wifly_Light/FtpUpdateServer/public/wifly7-441.mif";
				fileName = "./rn171_fw/wifly7-441.mif";
				
				std::ifstream file(fileName, std::ios::in | std::ios::binary);
				
				if (file.is_open()) {
					telnet.Send("125 Data connection already open. Transfer starting.\r\n");
					transferDataPassive(file, *dataSocket);
					file.close();
					telnet.Send("226 Transfer complete.\r\n");
				} else {
					telnet.Send("550 Requested action not taken. File unavailable (e.g., file not found, no access). Good Bye.\r\n");
					delete dataSocket;
					return;
				}
				
			} else if (requestCMD == "QUIT" && isClientLoggedIn) {
				telnet.Send("221 Thank you for updating.\r\n");
				delete dataSocket;
				return;
				
			} else {
#define HAVE_MERCY 1
#if HAVE_MERCY
				//clear content of dataInput
				dataInput.str(std::string());
				telnet.Send("150 Command not supported.\r\n");
#else
				telnet.Send("150 Command not supported. Good Bye.\r\n");
				delete dataSocket;
				return;
#endif
			}
		}
	}
	
	void FtpServer::transferDataPassive(std::ifstream& file, const TcpServerSocket& dataSocket) const throw(FatalError) {
		size_t bytesRead = 0;
		uint8_t buffer[2048];

		file.seekg(0, file.end);
		int length = (int)file.tellg();
        file.seekg(0, file.beg);
		
		sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);
		
		// start a timeout kill thread here!!
		int tempDataSock = -1;
		
		std::thread killThread = std::thread([&]{
			
			std::this_thread::sleep_for(std::chrono::seconds(1));
			uint16_t port = dataSocket.GetPort();

			//shutting down if no accept happend
			if (tempDataSock == -1) {
				TcpSocket shutdownSocket(LOCALHOST, port);
			}
		});
		
		//blocking accept here !!
		tempDataSock = accept(dataSocket.GetSocket(), reinterpret_cast<struct sockaddr *>(&clientAddr), &clientAddrLen);
		
		if (tempDataSock == -1) {
			killThread.join();
			throw FatalError("Invalid Data Socket");
		}
		
#if 0 //Disabled to allow localhost connections
		if (ntohl(clientAddr.sin_addr.s_addr) == LOCALHOST) {
			killThread.join();
			throw FatalError("Timeout occured, unblocked accept by killThread!! ");
		}
#endif
		
		for (bytesRead = 0; bytesRead < length - sizeof(buffer); bytesRead = bytesRead + sizeof(buffer)) {
			file.read((char*)buffer, sizeof(buffer));
			if (!file) {
				killThread.join();
				close(tempDataSock);
				throw FatalError("Error in Filestream occured");
			}
			Send(buffer, sizeof(buffer), tempDataSock);
		}
		
		file.read((char*)buffer, length - bytesRead);
		if (!file) {
			killThread.join();
			close(tempDataSock);
			throw FatalError("Error in Filestream occured");
		}
		
		Send(buffer, length - bytesRead, tempDataSock);

		killThread.join();
		close(tempDataSock);
	}
	
	TcpServerSocket* FtpServer::openDataConnection(const TcpSocket& telnet) throw(FatalError)
	{
		struct sockaddr_in sin;
		socklen_t len = sizeof(sin);
		if (getsockname(telnet.GetSocket(), (struct sockaddr *)&sin, &len) == -1) {
			throw FatalError("Getsockname failed");
		}
		
		TcpServerSocket *result = new TcpServerSocket(ntohl(sin.sin_addr.s_addr), 0);
		if(!result) {
			throw FatalError("openDataConnection(): create TcpServerSocket failed");
		}
		return result;
	}
	
	size_t FtpServer::Send(const std::string &message, const int& socket) const throw (FatalError){
		return this->Send(message.data(), message.length(), socket);
	}
	
	size_t FtpServer::Send(const void *frame, const size_t length, const int& socket) const throw(FatalError) {
		if (socket != -1) {
			//TraceBuffer(ZONE_INFO, frame, length, "%02x ", "Sending on socket 0x%04x, %zu bytes: ", mClientSock, length);
			const ssize_t result = write(socket, frame, length);
			if(result == -1) {
				throw FatalError("send on socket " + std::to_string(socket) + " failed with returnvalue -1 and errno:" + std::to_string(errno));
			}
			return result;
		}
		else {
			throw FatalError("Invalid Socket");
		}
	}

} /* namespace WyLight */

