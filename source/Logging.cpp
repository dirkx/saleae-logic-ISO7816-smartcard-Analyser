// Copyright © 2016 Adam Augustyn <adam@augustyn.net>, all rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
//

#include "Logging.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>
#endif //_WIN32

void Logging::Write(const char *msg)
{
#if defined(_WIN32)
	std::ostringstream tmp;
	tmp << msg;
	OutputDebugStringA(tmp.str().c_str());
#endif //_WIN32
}

void Logging::Write(const std::string& msg)
{
#if defined(_WIN32)
	std::ostringstream tmp;
	tmp << msg.c_str();
	OutputDebugStringA(tmp.str().c_str());
#endif //_WIN32
}
