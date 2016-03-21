// Copyright © 2016 Adam Augustyn <adam@augustyn.net>, all rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
//

#ifndef BYTEELEMENT_HPP
#define BYTEELEMENT_HPP

class ByteElement
{
public:
	ByteElement(unsigned char val, unsigned long long startPos, unsigned long long endPos)
	{
		this->val = val;
		this->startPos = startPos;
		this->endPos = endPos;
	}
	virtual ~ByteElement()
	{
	}

	unsigned char GetValue()
	{
		return this->val;
	}

	unsigned long long GetStartPos()
	{
		return this->startPos;
	}

	unsigned long long GetEndPos()
	{
		return this->endPos;
	}

private:
	unsigned char val;
	unsigned long long startPos;
	unsigned long long endPos;
};

#endif //BYTEELEMENT_HPP
