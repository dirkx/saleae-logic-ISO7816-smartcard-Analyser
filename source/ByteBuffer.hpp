// Copyright © 2016 Adam Augustyn <adam@augustyn.net>, all rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
//

#ifndef BYTEBUFFER_HPP
#define BYTEBUFFER_HPP

#include <vector>
#include "ByteElement.hpp"

class ByteBuffer : public std::vector<ByteElement>
{
public:
	typedef std::vector<ByteElement> base;

public:
	ByteBuffer()
	{
	}
	virtual ~ByteBuffer()
	{
	}

	std::vector<unsigned char> ToBytes()
	{
		std::vector<unsigned char> ret;
		for (ByteElement t : *(base*)this)
		{
			ret.push_back(t.GetValue());
		}
		return ret;
	}
};

#endif //BYTEBUFFER_HPP
