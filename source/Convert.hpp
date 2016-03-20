// Copyright © 2016 Adam Augustyn <adam@augustyn.net>, all rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
//

#include <string>

#ifndef CONVERT_HPP
#define CONVERT_HPP

class Convert
{
public:
	static std::string ToHex(unsigned long long val);
	static std::string ToHex(unsigned short val);
	static std::string ToHex(unsigned char val);
	static std::string ToHex(int val);

	static std::string ToDec(unsigned long long val);
	static std::string ToDec(unsigned int val);
	static std::string ToDec(int val);

private:
	Convert();
	virtual ~Convert();
};

#endif //CONVERT_HPP
