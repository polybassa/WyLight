/*
   Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.

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

#include "Script.h"
#include "trace.h"
#include <fstream>

namespace WyLight
{
static const uint32_t __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

const unsigned int Script::currentVersion = 1;

const std::string FwCmdSetFade::TOKEN("fade");
const std::string FwCmdSetGradient::TOKEN("gradient");
const std::string FwCmdSetMove::TOKEN("move");
const std::string FwCmdLoopOn::TOKEN("loop");
const std::string FwCmdLoopOff::TOKEN("loop_off");
const std::string FwCmdWait::TOKEN("wait");

Script::Script(const std::string& filename)
{
    Script::deserialize(filename, *this);
}

Script::~Script(void)
{
    clear();
}

Script::Script(Script&& other) : mName(std::move(other.mName)), mList(std::move(other.mList)) {}

bool Script::operator==(const Script& ref) const
{
    if (size() != ref.size())
        return false;

    auto refIt = ref.begin();
    for (const auto& cmd :* this) {
        if (*cmd != **refIt++)
            return false;
    }
    return true;
}

Script::ScriptList::const_iterator Script::begin() const noexcept
{
    return mList.begin();
}

void Script::clear()
{
    for (auto& cmd : mList) {
        cmd.reset();
    }
    mList.clear();
}

void Script::deserialize(const std::string& filename, Script& newScript)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open())
        throw FatalError("Open '" + filename + "' to read script failed");

    newScript.deserialize(inFile, newScript);
    inFile.close();

    if (newScript.mName.empty()) {
        // std::string::npos+1 will overflow to 0 just like we want!
        const size_t indexOfLastSeperator = filename.find_last_of('/') + 1;
        newScript.mName = filename.substr(indexOfLastSeperator);
    }
}

void Script::deserialize(std::istream& inStream, WyLight::Script& newScript)
{
    int c = inStream.peek();
    if (c == EOF)
        return;

    if (std::isdigit(static_cast<unsigned char>(c))) {
        std::string firstLine;
        std::getline(inStream, firstLine);
        //const int fileVersion = std::stoi(firstLine);
        /* do something with different versions here */
        std::getline(inStream, newScript.mName);
    }

    std::string command;
    while (inStream >> command) {
        if (0 == command.compare(FwCmdLoopOn::TOKEN))
            newScript.push_back(std::unique_ptr<FwCmdScript>(new FwCmdLoopOn()));
        else if (0 == command.compare(FwCmdLoopOff::TOKEN))
            newScript.push_back(std::unique_ptr<FwCmdScript>(new FwCmdLoopOff(inStream)));
        else if (0 == command.compare(FwCmdWait::TOKEN))
            newScript.push_back(std::unique_ptr<FwCmdScript>(new FwCmdWait(inStream)));
        else if (0 == command.compare(FwCmdSetFade::TOKEN))
            newScript.push_back(std::unique_ptr<FwCmdScript>(new FwCmdSetFade(inStream)));
        else if (0 == command.compare(FwCmdSetGradient::TOKEN))
            newScript.push_back(std::unique_ptr<FwCmdScript>(new FwCmdSetGradient(inStream)));
        else if (0 == command.compare(FwCmdSetMove::TOKEN))
            newScript.push_back(std::unique_ptr<FwCmdScript>(new FwCmdSetMove(inStream)));
        else
            throw FatalError("Unknown command: " + command);
    }
}

Script::ScriptList::const_iterator Script::end() const noexcept
{
    return mList.end();
}

const std::string& Script::getName() const
{
    return mName;
}

void Script::setName(const std::string& name)
{
    mName = name;
}

void Script::push_back(std::unique_ptr<FwCmdScript>&& pNew)
{
    mList.push_back(std::move(pNew));
}

void Script::serialize(const std::string& filename, const Script& newScript)
{
    std::ofstream outFile(filename);
    if (!outFile.is_open())
        throw FatalError("Open '" + filename + "' to save script failed");

    outFile << currentVersion << '\n';
    outFile << newScript.mName << '\n';

    std::string command;
    size_t identation = 0;
    for (const auto& cmd : newScript) {
        cmd->Write(outFile, identation) << '\n';
    }
    outFile.close();
}

size_t Script::size() const
{
    return mList.size();
}
} /* namespace WyLight */
