﻿/* Copyright (c) 2017 Junkai Lu <junkai-lu@outlook.com>.
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

#include "game_shell.h"

namespace gadt
{
	namespace shell
	{
		//shell defination.
		namespace define
		{
			//default params check func.
			bool DefaultParamsCheck(const ParamsList & list)
			{
				return true;
			}

			//default no params check func.
			bool DefaultNoParamsCheck(const ParamsList & list)
			{
				return DefaultParamsCountCheck<0>(list);
			}
		}

		//shell command class.
		namespace command
		{
			//get the name of command type.
			std::string GetCommandTypeName(CommandType type)
			{
				static const char* type_name[define::GADT_SHELL_COMMAND_TYPE_COUNT] = {
					"DEFAULT COMMAND",
					"DATA COMMAND",
					"PARAMS COMMAND",
					"DATA_AND_PARAMS_COMMAND",
					"BOOL_PARAMS COMMAND",
					"BOOL_DATA_AND_PARAMS_COMMAND"
				};
				return std::string(type_name[static_cast<size_t>(type)]);
			}

			//get the symbol of command type.
			std::string GetCommandTypeSymbol(CommandType type)
			{
				static const char* type_symbol[define::GADT_SHELL_COMMAND_TYPE_COUNT] = {
					"[F]",
					"[F]",
					"[F]",
					"[F]",
					"[F]",
					"[F]"
				};
				return std::string(type_symbol[static_cast<size_t>(type)]);
			}

			//default constructor.
			CommandParser::CommandParser() :
				_is_legal(false),
				_is_relative(true),
				_commands(),
				_params()
			{
			}

			//constructor function by command string.
			CommandParser::CommandParser(std::string original_command) :
				_is_legal(false),
				_is_relative(true),
				_commands(),
				_params()
			{
				_is_legal = ParseOriginalCommand(original_command);
			}

			//CommandParser
			bool CommandParser::CheckStringLegality(std::string str)
			{
				for (auto c : str)
				{
					if (c == 92 || c == 47 || c == 58 || c == 42 || c == 34 || c == 60 || c == 62 || c == 124)
					{
						return false;
					}
				}
				return true;
			}

			//get next state of current parser.
			CommandParser CommandParser::GetNext() const
			{
				auto temp = _commands;
				if (temp.size() > 0)
				{
					temp.pop_front();
				}
				return CommandParser(_is_legal, _is_relative, temp, _params);
			}

			//get parser of the path.
			CommandParser CommandParser::GetPathParser() const
			{
				auto temp = _commands;
				if (temp.size() > 0)
				{
					temp.pop_back();
				}
				return CommandParser(_is_legal, _is_relative, temp, std::vector<std::string>());
			}

			//parse the command.
			bool CommandParser::ParseParameters(std::string params_str)
			{
				auto param_list = DivideString(params_str, " ");
				for (auto param : param_list)
				{
					add_parameter(param);
				}
				return true;
			}

			//parse command
			bool CommandParser::ParseCommands(std::string cmd_str)
			{
				auto cmd_list = DivideString(cmd_str, "/");
				size_t begin = 0;
				if (cmd_list.size() != 0)
				{
					if (cmd_list[0].empty())
					{
						_is_relative = false;//absolute path.
						begin = 1;
					}
				}
				for (size_t i = begin; i < cmd_list.size(); i++)
				{
					if (CheckStringLegality(cmd_list[i]) == true)
					{
						add_command(cmd_list[i]);
					}
					else
					{
						return false;//find illegal command
					}
				}
				return true;
			}

			//divide origin command into command and parameters.
			bool CommandParser::ParseOriginalCommand(std::string original_command)
			{
				size_t space_pos = original_command.find(" ");
				if (space_pos != std::string::npos)
				{
					//find space, divide command into command part and param part.
					std::string commands = original_command.substr(0, space_pos);
					std::string params = original_command.substr(space_pos, original_command.length() - space_pos);
					return ParseCommands(commands) && ParseParameters(params);
				}
				//no params, parse commands.
				return ParseCommands(original_command);
			}

			//remove space behind or after the string.
			std::string CommandParser::RemoveSpace(std::string str)
			{
				for (;;)
				{
					if (str.length() == 0)
						return str;
					else if (str[0] == ' ')
						str = str.substr(1, str.length() - 1);
					else if (str.back() == ' ')
						str.pop_back();
					else
						return str;
				}
				return str;
			}

			//divide single string into multi string by separator.
			std::vector<std::string> CommandParser::DivideString(std::string str, std::string separator)
			{
				std::vector<std::string> result;
				str = RemoveSpace(str);
				while (str.length() != 0)
				{
					size_t divide_pos = str.find(separator);
					if (divide_pos != std::string::npos)
					{
						std::string single_str = RemoveSpace(str.substr(0, divide_pos));
						result.push_back(single_str);
						str = str.substr(divide_pos + separator.length(), str.length() - divide_pos);
					}
					else
					{
						result.push_back(RemoveSpace(str));
						break;
					}
				}
				return result;
			}
		}

		//shell pages class
		namespace page
		{
			//ShellPageBase
			ShellPageBase::ShellPageBase(PageBasePtr parent_page, ShellPageBase::ShellPtr belonging_shell, std::string name, InfoFunc info_func) :
				_parent_page(parent_page),
				_belonging_shell(belonging_shell),
				_name(name),
				_info_func(info_func),
				_index(AllocPageIndex()),
				_child_pages()
			{
			}

			//get root page of current page.
			PageBasePtr ShellPageBase::GetRootPage()
			{
				if (parent_page() == nullptr)
					return this;
				return parent_page()->GetRootPage();
			}

			//get relative path page. return nullptr if page not found.
			PageBasePtr ShellPageBase::GetRelativePathPage(command::CommandParser parser)
			{
				PageBasePtr temp_page_ptr = this;
				if (parser.is_relative() == false)
				{
					temp_page_ptr = GetRootPage();
				}
				for (size_t i = 0; i < define::GADT_SHELL_MAX_PAGE_LAYER; i++)
				{
					if (parser.no_commands())
					{
						return temp_page_ptr;
					}
					else
					{
						std::string page_name = parser.fir_command();
						if (page_name == define::GADT_SHELL_PAGE_LAST_STR && temp_page_ptr->parent_page() != nullptr)
						{
							temp_page_ptr = temp_page_ptr->parent_page();
							parser.to_next_command();
						}
						else if (page_name == define::GADT_SHELL_PAGE_THIS_STR)
						{
							//temp_page_ptr = temp_page_ptr->parent_page();
							parser.to_next_command();
						}
						else if (temp_page_ptr->ExistChildPage(page_name))
						{
							temp_page_ptr = temp_page_ptr->GetChildPagePtr(page_name);
							parser.to_next_command();
						}
						else
						{
							parser.clear();
							return nullptr;
						}
					}
				}
				return nullptr;
			}

			//print path of current page 
			void ShellPageBase::PrintPath() const
			{
				if (parent_page() == nullptr)
				{
					console::Cprintf(name(), console::ConsoleColor::Yellow);
				}
				else
				{
					parent_page()->PrintPath();
					console::Cprintf(name(), console::ConsoleColor::Green);
				}
				std::cout << "/";
			}

			//return true if the page exist.
			bool ShellPageBase::ExistChildPage(std::string name) const
			{
				if (_child_pages.count(name) > 0)
					return true;
				return false;
			}

			//get child page pointer by name.
			PageBasePtr ShellPageBase::GetChildPagePtr(std::string name) const
			{
				if (ExistChildPage(name))
				{
					return _child_pages.at(name).ptr.get();
				}
				return nullptr;
			}

			//get child page description by name.
			std::string ShellPageBase::GetChildPageDesc(std::string name) const
			{
				if (ExistChildPage(name))
				{
					return _child_pages.at(name).desc;
				}
				return "";
			}

			//get child page help description by name.
			std::string ShellPageBase::GetChildPageHelpDesc(std::string name) const
			{
				if (ExistChildPage(name))
				{
					return _child_pages.at(name).help_desc;
				}
				return "";
			}

			//return true if command name is legal.
			bool ShellPageBase::CheckCommandNameLegality(std::string command) const
			{
				if (command.size() > define::GADT_SHELL_COMMAND_MAX_NAME_LENGTH)
				{
					console::PrintError("command '" + command + "' out of max length");
					return false;
				}
				if (!command::CommandParser::CheckStringLegality(command))
				{
					console::PrintError("illegal command name '" + command + "'.");
					return false;
				}
				return true;
			}
		}

		//GameShell
		GameShell* GameShell::_g_focus_game = nullptr;

		//public constructor function
		GameShell::GameShell(std::string name) :
			_name(name),
			_shell_cmd(nullptr, this, "shell_cmd", []() {}),
			_root_page(nullptr, this, "root", DefaultInfoFunc),
			_focus_page(&_root_page)
		{
			InitializeShellCommands();
		}

		//default info func of game shell.
		void GameShell::DefaultInfoFunc()
		{
			timer::TimePoint tp;
			console::Cprintf("=============================================\n", console::ConsoleColor::Gray);
			console::Cprintf("       Game AI Development Toolkit\n", console::ConsoleColor::Yellow);
			console::Cprintf("       Copyright @ Junkai-Lu 2018 \n", console::ConsoleColor::Yellow);
			console::Cprintf("=============================================", console::ConsoleColor::Gray);
			console::PrintEndLine<2>();
		}

		//print current dir.
		void GameShell::PrintFocusPath() const
		{
			//print shell name.
			console::Cprintf(GameShell::focus_game()->name(), console::ConsoleColor::Purple);
			std::cout << " @ ";
			focus_page()->PrintPath();
		}

		//print input tips.
		void GameShell::InputTip(std::string tip)
		{
			if (focus_game() != nullptr)
			{
				focus_game()->PrintFocusPath();
				if (tip != "")
				{
					std::cout << "/";
				}
				console::Cprintf(tip, console::ConsoleColor::Green);
				std::cout << ": >> ";
			}
			else
			{
				console::Cprintf("ERROR: focus game not exist", console::ConsoleColor::Purple);
			}
		}

		std::string GameShell::GetInput()
		{
			char buffer[256];
			std::cin.clear();
			std::cin.getline(buffer, 256);
			return std::string(buffer);
		}

		//clear screen
		void GameShell::ClearScreen() const
		{
			console::SystemClear();
			focus_page()->_info_func();
			std::cout << ">> ";
			console::Cprintf("[ Shell ", console::ConsoleColor::DeepYellow);
			console::Cprintf("<" + focus_page()->_name + ">", console::ConsoleColor::Yellow);
			console::Cprintf(" ]\n", console::ConsoleColor::DeepYellow);
			std::cout << ">> ";
			console::Cprintf(std::string("use '") + std::string(define::GADT_SHELL_COMMAND_HELP_NAME) + std::string("' to get more command\n\n"), console::ConsoleColor::DeepGreen);
		}

		//initialize shell commands.
		void GameShell::InitializeShellCommands()
		{
			using CommandPtr = typename page::ShellPage<int>::CommandPtr;
			using ParamsCommand = typename page::ShellPage<int>::ParamsCommand;

			auto ListCommandFunc = [&](const ParamsList& params)->void {
				if (params.size() == 0)
				{
					focus_page()->PrintCommandList("-n");
				}
				else
				{
					focus_page()->PrintCommandList(params[0]);
				}
			};
			auto ListCommandCond = [](const ParamsList& params)->bool {
				if (params.size() == 0)
				{
					return true;
				}
				else if (params.size() == 1)
				{
					if (params[0] == "-n" || params[0] == "-t")
					{
						return true;
					}
					console::PrintMessage("'" + std::string(define::GADT_SHELL_COMMAND_LIST_NAME) + "' only accept -n or -t as parameter");
					return false;
				}
				console::PrintMessage("'" + std::string(define::GADT_SHELL_COMMAND_LIST_NAME) + "' only accept one paramater");
				return false;
			};

			auto HelpCommandFunc = [&](const ParamsList& params)->void {
				if (params.size() == 1)
				{

				}
				else
				{
					_shell_cmd.PrintCommandList(std::string("-n"));
				}
			};
			auto HelpCommandCond = [&](const ParamsList& params)->bool {
				if (params.size() == 0)
				{
					return true;
				}
				else if (params.size() == 1)
				{
					std::string name = params.front();
					if (this->focus_page()->ExistCommand(name) || this->focus_page()->ExistChildPage(name))
					{
						return true;
					}
					else
					{
						console::PrintMessage(std::string("command '") + params.front() + "' not found.");
						return false;
					}
				}
				console::PrintMessage(std::string("'") + define::GADT_SHELL_COMMAND_HELP_NAME + "' only accept one parameter.");
				return false;
			};

			auto cd_command = CommandPtr(new ParamsCommand(
				define::GADT_SHELL_COMMAND_CD_NAME,
				define::GADT_SHELL_COMMAND_CD_DESC,
				[&](const ParamsList& params)->void {
					std::stringstream ss;
					for (auto p : params)
						ss << p;
					ChangeDirectory(ss.str()); 
				},
				define::DefaultParamsCheck
			));
			/*auto bat_command = CommandPtr(new ParamsCommand(
				define::GADT_SHELL_COMMAND_BAT_NAME,
				define::GADT_SHELL_COMMAND_BAT_DESC,
				[&](const ParamsList& params)->void { set_focus_page(nullptr); },
				define::DefaultParamsCountCheck<1>
			));*/
			auto exit_command = CommandPtr(new ParamsCommand(
				define::GADT_SHELL_COMMAND_EXIT_NAME,
				define::GADT_SHELL_COMMAND_EXIT_DESC,
				[&](const ParamsList& params)->void { set_focus_page(nullptr); },
				define::DefaultNoParamsCheck
			));
			auto list_command = CommandPtr(new ParamsCommand(
				define::GADT_SHELL_COMMAND_LIST_NAME,
				define::GADT_SHELL_COMMAND_LIST_DESC,
				ListCommandFunc,
				ListCommandCond
			));
			auto help_command = CommandPtr(new ParamsCommand(
				define::GADT_SHELL_COMMAND_HELP_NAME,
				define::GADT_SHELL_COMMAND_HELP_DESC,
				HelpCommandFunc,
				HelpCommandCond
			));
			auto clear_command = CommandPtr(new ParamsCommand(
				define::GADT_SHELL_COMMAND_CLEAR_NAME,
				define::GADT_SHELL_COMMAND_CLEAR_DESC,
				[&](const ParamsList& data)->void { this->ClearScreen(); },
				define::DefaultNoParamsCheck
			));

			_shell_cmd.add_command(define::GADT_SHELL_COMMAND_CD_NAME, cd_command);
			_shell_cmd.add_command(define::GADT_SHELL_COMMAND_EXIT_NAME, exit_command);
			_shell_cmd.add_command(define::GADT_SHELL_COMMAND_LIST_NAME, list_command);
			_shell_cmd.add_command(define::GADT_SHELL_COMMAND_HELP_NAME, help_command);
			_shell_cmd.add_command(define::GADT_SHELL_COMMAND_CLEAR_NAME, clear_command);
		}

		//execute command in focus page.
		void GameShell::ExecuteCommand(page::PageBasePtr page_ptr, std::string command, const ParamsList& params)
		{
			if (_shell_cmd.ExistCommand(command))
			{
				_shell_cmd.ExecuteCommand(command, params);
			}
			else
			{
				page_ptr->ExecuteCommand(command, params);
			}
		}

		//run single command.
		bool GameShell::RunSingleCommand(std::string command_str)
		{
			command::CommandParser parser(command_str);
			if (parser.no_commands() || !parser.is_legal())
			{
				console::PrintError("inavailable command!");
				return false;
			}
			auto target_page_ptr = focus_page()->GetRelativePathPage(parser.GetPathParser());
			if (target_page_ptr != nullptr)
			{
				std::string cmd_name = parser.fir_command();
				if (target_page_ptr->ExistCommand(cmd_name) || exist_shell_cmd(cmd_name))
				{
					ExecuteCommand(target_page_ptr, parser.fir_command(), parser.params());
					//if (!no_focus_page())//exit command was executed.
					//	set_focus_page(temp_page_ptr);
					return true;
				}
				else
				{
					console::PrintError("command " + cmd_name + " not found");
				}
			}
			else
			{
				console::PrintError(name() + ": unexcepted command: " + command_str);
			}
			return false;
		}

		//start from root.
		void GameShell::Run(std::string init_command)
		{
			be_focus();
			ClearScreen();
			
			std::string command = init_command;
			for (;;)
			{
				if (no_focus_page())
				{
					return;//exit command was executed.
				}
				if (command.empty())
				{
					InputTip();
					command = GetInput();
				}
				else
				{
					RunSingleCommand(command);
					command.clear();
				}
			}
		}

		//run test and report time.
		void TestPage::RunTest(const FuncItem & item)
		{
			using namespace console;
			console::PrintEndLine();
			console::Cprintf(">> test start, target = ", ConsoleColor::White);
			console::Cprintf(item.first, console::ConsoleColor::Green);
			timer::TimePoint tp;
			console::PrintEndLine();
			item.second();
			console::Cprintf(">> test complete, time = ", ConsoleColor::White);
			console::Cprintf(tp.time_since_created(), console::ConsoleColor::Red);
			console::PrintEndLine();
		}

		//add 'all' test function.
		void TestPage::AddTestAll()
		{
			if (is_initialized())
			{
				_test_page->AddFunction("all", "run all test function", [&](FuncList& functions)->void {
					for (const auto& p : functions)
					{
						RunTest(p);
					}
				});
			}
		}

		//add function by name and function, which would generate a default description. 
		void TestPage::AddFunction(std::string name, FuncType func)
		{
			AddFunction(name, "test " + name, func);
		}

		//add function by name, description and function.
		void TestPage::AddFunction(std::string name, std::string desc, FuncType func)
		{
			if (is_initialized())
			{
				_test_page->data().push_back({ name, func });
				const size_t size = _test_page->data().size();
				_test_page->AddFunction(name, desc, [=](FuncList& func_list)->void {
					RunTest(func_list[size - 1]);
				});
			}
		}

		//add multi functions by name and function, which would generate a default description.
		void TestPage::AddFunctionList(std::initializer_list<FuncItem> init_list)
		{
			for (const auto& pair : init_list)
			{
				AddFunction(pair.first, pair.second);
			}
		}
}
}