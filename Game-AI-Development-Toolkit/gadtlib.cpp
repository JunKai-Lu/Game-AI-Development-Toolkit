/*
* gadt lib include some basic function of Game Ai Development Toolkit.
*
*/

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

namespace gadt
{
	//a golbal color ostream 
	console::costream ccout(std::cout);

	namespace console
	{
		std::string costream::color_str(ConColor color)
		{
#ifndef GADT_UNIX
			HANDLE handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handle, color);
			return std::string("");
#else
			static string color_str[16] =
			{
				string(""),
				string("\e[0;40;34m"),		//DEEP_BLUE = 1,
				string("\e[0;40;32m"),		//DEEP_GREEN = 2,
				string("\e[0;40;36m"),		//deep_cyan = 3,
				string("\e[0;40;31m"),		//brown = 4,
				string("\e[0;40;35m"),		//PURPLE = 5,
				string("\e[0;40;33m"),		//DEEP_YELLOW = 6,
				string("\e[0m"),			//deep_white = 7,
				string("\e[0;40;37m"),		//GRAY = 8,
				string("\e[0;40;34m"),		//BLUE = 9,
				string("\e[0;40;32m"),		//GREEN = 10,
				string("\e[0;40;36m"),		//cyan = 11,
				string("\e[0;40;31m"),		//RED = 12,
				string("\e[0;40;35m"),		//pink = 13,
				string("\e[0;40;33m"),		//YELLOW = 14,
				string("\e[0;40;37m")		//white = 15
											//string("\e[0;40;37m")		//white = 15
			};
			return color_str[color];
#endif
		}
		
		void ShowError(std::string reason)
		{
			std::cout << std::endl;
			Cprintf(">> ERROR: ", console::RED);
			Cprintf(reason, console::WHITE);
			std::cout << std::endl << std::endl;
		}

		void ShowMessage(std::string message, bool show_MSG)
		{
			std::cout << ">> ";
			if (show_MSG)
			{
				Cprintf("MSG: ", console::DEEP_GREEN);
			}
			Cprintf(message, console::GREEN);
			std::cout << std::endl << std::endl;
		}

		void WarningCheck(bool condition, std::string reason, std::string file, int line, std::string function)
		{
			if (condition)
			{
				std::cout << std::endl << std::endl;
				Cprintf(">> WARNING: ", console::PURPLE);
				Cprintf(reason, console::RED);
				std::cout << std::endl;
				Cprintf("[File]: " + file, console::GRAY);
				std::cout << std::endl;
				Cprintf("[Line]: " + I2S(line), console::GRAY);
				std::cout << std::endl;
				Cprintf("[Func]: " + function, console::GRAY);
				std::cout << std::endl;
				console::SystemPause();
			}
		}

		void SystemPause()
		{
#ifdef GADT_UNIX
			cout << "Press ENTER to continue." << endl;
			while (!getchar());
#else
			system("pause");
#endif
		}
	}
}

std::string gadt::timer::TimeString()
{
	time_t t = time(NULL);
	char buf[128] = { 0 };
#ifndef GADT_UNIX
	tm local;
	localtime_s(&local, &t);
	strftime(buf, 64, "%Y.%m.%d-%H:%M:%S", &local);
	return std::string(buf);
#else
	tm* local;
	local = localtime(&t);
	strftime(buf, 64, "%Y.%m.%d-%H:%M:%S", local);
	return std::string(buf);
#endif
}

clock_t gadt::timer::GetClock()
{
	return clock();
}

double gadt::timer::GetTimeDifference(const clock_t & start)
{
	return (double)(clock() - start) / CLOCKS_PER_SEC;
}
