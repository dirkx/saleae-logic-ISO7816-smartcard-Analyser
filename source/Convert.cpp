// Copyright © 2016 Adam Augustyn <adam@augustyn.net>, all rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
//

#include "Convert.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

std::string Convert::ToHex(unsigned long long val)
{
	std::ostringstream tmp;
	tmp << "0x" << std::uppercase << std::setfill('0') << std::setw(16) << std::hex << val;
	return tmp.str();
}

std::string Convert::ToHex(unsigned short val)
{
	std::ostringstream tmp;
	tmp << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << val;
	return tmp.str();
}

std::string Convert::ToHex(unsigned char val)
{
	std::ostringstream tmp;
	tmp << "" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned>(val);
	return tmp.str();
}

std::string Convert::ToHex(int val)
{
	std::ostringstream tmp;
	tmp << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << val;
	return tmp.str();
}

std::string Convert::ToDec(unsigned long long val)
{
	std::ostringstream tmp;
	tmp << val;
	return tmp.str();
}

std::string Convert::ToDec(unsigned int val)
{
	std::ostringstream tmp;
	tmp << val;
	return tmp.str();
}

std::string Convert::ToDec(int val)
{
	std::ostringstream tmp;
	tmp << val;
	return tmp.str();
}

