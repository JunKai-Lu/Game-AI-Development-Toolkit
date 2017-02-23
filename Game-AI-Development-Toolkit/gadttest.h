/* Copyright (c) 2017 Junkai Lu <junkai-lu@outlook.com>.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "gadtlib.h"
#include "bitboard.hpp"

#pragma once

#define GADT_ASSERT(fir,sec) if(fir != sec)std::cout<<"Assert Failed, func = "<<__FUNCTION__<<" line = "<<__LINE__<<std::endl;
#define GADT_TEST_FINISH(name, start) std::cout<<">> test "<<name<<" finish, time = "<<gadt::timer::GetTimeDifference(start)<<std::endl;

namespace gadt
{
	namespace unittest
	{
		typedef std::pair<std::string, std::function<void()>> FuncPair;
		extern const std::vector<FuncPair> func_list;
		void RunTest(FuncPair func_pair);
	}
}
