// Copyright © 2016 Adam Augustyn <adam@augustyn.net>, all rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
//

#include "ISO7816Pps.hpp"
#include "Definitions.hpp"

int ISO7816Pps::FiMap[16] = {372, 372, 558, 744, 1116, 1488, 1860, -1,
	-1, 512, 768, 1024, 1536, 2048, -1, -1};

int ISO7816Pps::DiMap[16] = {-1, 1, 2, 4, 8, 16, 32, 64,
	12, 20, -1, -1, -1, -1, -1, -1};


int ISO7816Pps::CalculateETU(unsigned char fi, unsigned char di)
{
	if (fi > 15 || di > 15) return -1;

	int _fi = FiMap[fi];
	int _di = DiMap[di];

	if (_fi < 0 || _di < 0) return -1;

	return _fi / _di;
}


int ISO7816Pps::IsPpsFrame(std::vector<unsigned char> buff, size_t pos)
{
	if (pos >= buff.size()) return -1;
	if (buff[pos] != PPS_HEADER) return -1;

	unsigned char crc = buff[pos];

	if (pos + 1 >= buff.size()) return -1;
	unsigned char pps0 = buff[pos + 1];
	crc = crc ^ pps0;
	size_t offset = pos + 2;
	if (0 != (PPS0_1 & pps0))
	{
		if (offset >= buff.size()) return -1;
		unsigned char tmp = buff[offset];
		crc = crc ^ tmp;
		// PPS1 exists
		offset++;
	}
	if (0 != (PPS0_2 & pps0))
	{
		if (offset >= buff.size()) return -1;
		unsigned char tmp = buff[offset];
		crc = crc ^ tmp;
		// PPS2 exists
		offset++;
	}
	if (0 != (PPS0_3 & pps0))
	{
		if (offset >= buff.size()) return -1;
		unsigned char tmp = buff[offset];
		crc = crc ^ tmp;
		// PPS3 exists
		offset++;
	}
	if (offset >= buff.size()) return -1;
	unsigned char crcEnd = crc ^ buff[offset];
	if (0 == crcEnd)
	{
		return static_cast<int>(offset + 1);
	}
	return -1;
}

ISO7816Pps::ptr ISO7816Pps::DecodeFrame(std::vector<unsigned char> buff, size_t pos)
{
	std::shared_ptr<ISO7816Pps> ret;
	if (pos >= buff.size()) return ret;
	if (buff[pos] != PPS_HEADER) return ret;

	unsigned char crc = buff[pos];

	if (pos + 1 >= buff.size()) return ISO7816Pps::ptr();
	unsigned char pps0 = buff[pos + 1];
	crc = crc ^ pps0;
	size_t offset = pos + 2;
	ret.reset(new ISO7816Pps());
	ret->protocol = pps0 & 0x0f;

	if (0 != (PPS0_1 & pps0))
	{
		if (offset >= buff.size()) return ISO7816Pps::ptr();
		unsigned char tmp = buff[offset];
		ret->fi = (tmp >> 4) & 0x0f;
		ret->di = tmp & 0x0f;
		crc = crc ^ tmp;
		// PPS1 exists
		offset++;
	}
	if (0 != (PPS0_2 & pps0))
	{
		if (offset >= buff.size()) return ISO7816Pps::ptr();
		unsigned char tmp = buff[offset];
		crc = crc ^ tmp;
		// PPS2 exists
		offset++;
	}
	if (0 != (PPS0_3 & pps0))
	{
		if (offset >= buff.size()) return ISO7816Pps::ptr();
		unsigned char tmp = buff[offset];
		crc = crc ^ tmp;
		// PPS3 exists
		offset++;
	}

	if (offset >= buff.size()) return ISO7816Pps::ptr();
	unsigned char crcEnd = crc ^ buff[offset];
	if (0 != crcEnd)
	{
		ret.reset();
	}
	return ret;
}

bool ISO7816Pps::Equal(ISO7816Pps::ptr other)
{
	if (!other) return false;
	if (this->fi != other->fi) return false;
	if (this->di != other->di) return false;
	if (this->protocol != other->protocol) return false;

	// Thanks God it is the same ;)
	return true;
}

int ISO7816Pps::GetFi()
{
	return this->fi;
}

int ISO7816Pps::GetDi()
{
	return this->di;
}

int ISO7816Pps::GetProtocol()
{
	return this->protocol;
}

ISO7816Pps::ISO7816Pps()
{
	fi = -1;
	di = -1;
	protocol = -1;
}

ISO7816Pps::~ISO7816Pps()
{
}
