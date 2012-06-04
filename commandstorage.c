/** Changelog
 * 2012-05-08 pb:
 * - refactor functions to access and manage a buffer for led commands, which is stored in the eeprom
**/

#include "platform.h"
#include "commandstorage.h"

struct led_cmd* commandstorage_read(struct led_cmd *pDest, bit movePtr)
{
	//check parameter
	if(0 == pDest) return 0;

	//commands available in eeprom?
	char nextCmd = EEPROM_RD(CmdPointerAddr);
	if(0 == nextCmd) return 0;

	//read command from eeprom
	EEPROM_RD_BLK((char*)pDest, (nextCmd - CmdWidth), CmdWidth);

	//do we have to update the CmdPointer?
	if(movePtr)
	{		
		EEPROM_WR(CmdPointerAddr, nextCmd - CmdWidth);
	}
	return pDest;
}

bit commandstorage_write(char *pSrc, char length)
{
	//check parameter
	if(0 == pSrc) return FALSE;
	
	//enought free space in eeprom?
	char nextCmd = EEPROM_RD(CmdPointerAddr);
	if(nextCmd >= (CmdPointerAddr - CmdWidth)) return FALSE;

	//increase the command pointer in eeprom
	EEPROM_WR(CmdPointerAddr,(nextCmd + CmdWidth));
		
	//write data to eeprom
	EEPROM_WR_BLK(pSrc, nextCmd, length);
		
	return TRUE;
}

void commandstorage_get_commands()
{	
	if(RingBufHasError)
	{
		// *** if a RingBufError occure, I have to throw away the current command,
		// *** because the last byte was not saved. Commandstring is inconsistent
		ClearCmdBuf();
	}

	if(RingBufIsNotEmpty)
	{
		// *** preload variables and 
		// *** get new_byte from ringbuffer
		char new_byte, temp, j;
		temp = 0;
		j = 0;
		// *** get new byte
		new_byte = RingBufGet();	
		// *** do I wait for databytes?
		if(gCmdBuf.frame_counter == 0)
		{
			// *** I don't wait for databytes
			// *** Do I receive a Start_of_Text sign
			if((uns8)new_byte == STX)
			{
				// *** increse the cmd_counter
				gCmdBuf.cmd_counter = 1;
				// *** Write the startsign at the begin of the buffer
				gCmdBuf.cmd_buf[0] = new_byte;
                // *** Reset crc Variables
                newCRC(&gCmdBuf.crcH, &gCmdBuf.crcL);
                // *** add new_byte to crc checksum
                addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
			}
			else
			{	
				// *** to avoid arrayoverflow
				temp = FRAMELENGTH - 2;
				// *** check if I get the framelength byte
				if((new_byte < temp) && (gCmdBuf.cmd_counter == 1))
				{
					gCmdBuf.frame_counter = new_byte;
					gCmdBuf.cmd_buf[1] = new_byte;
					gCmdBuf.cmd_counter = 2;
                    // *** add new_byte to crc checksum
                    addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
				}
			}
		}
		else
		{
			// *** I wait for Databytes, so I save all bytes 
			// *** that I get until my framecounter is > 0
			gCmdBuf.cmd_buf[gCmdBuf.cmd_counter] = new_byte;
			gCmdBuf.cmd_counter++;
			
            // *** add new_byte to crc checksum
			if(gCmdBuf.frame_counter > 2)
            addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
			gCmdBuf.frame_counter--;
			// *** now I have to check if my framecounter is null.
			// *** If it's null my string is complete 
			// *** and I can give the string to the crc check function.
			if(gCmdBuf.frame_counter == 0)
			{
#ifdef NO_CRC
				if(1==1)
#else
                // *** verify crc checksum
                if( (gCmdBuf.crcL == gCmdBuf.cmd_buf[gCmdBuf.cmd_counter - 1]) &&
                    (gCmdBuf.crcH == gCmdBuf.cmd_buf[gCmdBuf.cmd_counter - 2]) )
#endif
                {
					// *** Execute the simple Commands
					switch(gCmdBuf.cmd_buf[2])
					{
						case DELETE: 
							{
								EEPROM_WR(CmdPointerAddr,0);
								return;
							}
#ifndef X86
						case SET_ON: 
							{
								BCF(PORTC.0); 
								return;
								}
						case SET_OFF: 
							{
								BSF(PORTC.0); 
								return;
							}
#endif /* #ifndef X86 */
					}			
					if( commandstorage_write(&gCmdBuf.cmd_buf[2], (gCmdBuf.cmd_counter - 4)))
					{
						USARTsend('G');
						USARTsend('C');
					}
					else 
						gERROR.eeprom_failure = 1;
#ifdef TEST
					USARTsend_arr(&gCmdBuf.cmd_buf[2], (gCmdBuf.cmd_counter - 4));
#endif
                }
                else
                {
                    // *** Do some error handling in case of an CRC failure here
					gERROR.crc_failure = 1;
                    return;
                }
			}
		}
	}
}

void commandstorage_execute_commands()
{
	// *** get the pointer to commands in the EEPROM
	struct led_cmd nextCmd;

	// read next command from eeprom and move command pointer in eeprom to the next command (TRUE)
	struct led_cmd *result = commandstorage_read(&nextCmd, TRUE);
	if(0 != result)
	{
		// *** commands available, check what to do
		switch(nextCmd.cmd) 
		{	
			case SET_COLOR: 
			{
				ledstrip_set_color(&nextCmd.data.set_color);
				break;
			}
			case SET_FADE: {break;}
			case SET_RUN: {break;}
		}
	}
}

void commandstorage_init()
{
	/** EEPROM contains FF in every cell after inital start,
	*** so I have to delete the pointer address
	*** otherwise the PIC thinks he has the EEPROM full with commands
	**/
	if (EEPROM_RD(CmdPointerAddr) == 0xff)
		EEPROM_WR(CmdPointerAddr, 0);

	// set loop pointer address to start
	EEPROM_WR(CmdLoopPointerAddr, 0);
}

