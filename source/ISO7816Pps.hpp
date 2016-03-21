// Copyright © 2016 Adam Augustyn <adam@augustyn.net>, all rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
//

#include <string>
#include <vector>
#include <memory>
#include "Convert.hpp"

#ifndef ISO7816PPS_HPP
#define ISO7816PPS_HPP

class ISO7816Pps
{
public:
	typedef std::shared_ptr<ISO7816Pps> ptr;

public:
	static int FiMap[16];
	static int DiMap[16];

public:
	virtual ~ISO7816Pps();
	static int CalculateETU(unsigned char fi, unsigned char di);
	static int IsPpsFrame(std::vector<unsigned char> buff, size_t pos);
	static ISO7816Pps::ptr DecodeFrame(std::vector<unsigned char> buff, size_t pos);

	bool Equal(ISO7816Pps::ptr other);

	int GetFi();
	int GetDi();
	int GetEtu()
	{
		return CalculateETU(GetFi(), GetDi());
	}
	int GetProtocol();
	std::string ToString()
	{
		return std::string("Fi=") + Convert::ToDec(GetFi()) + std::string(", ")
			+ std::string("Di=") + Convert::ToDec(GetDi()) + std::string(", ")
			+ std::string("ETU=") + Convert::ToDec(GetEtu()) + std::string(", ")
			+ std::string("Protocol=T") + Convert::ToDec(GetProtocol());
	}

protected:
	int fi;
	int di;
	int protocol;

private:
	ISO7816Pps();
};

#endif //ISO7816PPS_HPP
