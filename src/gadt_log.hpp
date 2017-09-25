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

#pragma once

namespace gadt
{
	namespace log
	{
		//error log class. 
		class ErrorLog
		{
		private:
			std::vector<std::string> _error_list;

		public:
			//default constructor.
			ErrorLog():
				_error_list()
			{
			}

			//constructor with init list.
			ErrorLog(std::initializer_list<std::string> init_list)
			{
				for (auto err : init_list)
				{
					_error_list.push_back(err);
				}
			}

			//copy constructor is banned
			ErrorLog(const ErrorLog&) = delete;

			//add a new error.
			inline void add(std::string err)
			{
				_error_list.push_back(err);
			}

			//return true if no error exist.
			inline bool is_empty() const
			{
				return _error_list.size() == 0;
			}

			//output as json format.
			inline std::string output() const
			{
				std::stringstream ss;
				ss << "[";
				for (size_t i = 0; i < _error_list.size(); i++)
				{
					ss << "\"" << _error_list[i] << "\"";
					if (i != _error_list.size() - 1)
					{
						ss << "," << std::endl;
					}
				}
				ss << "]";
				return ss.str();
			}
		};

		/*
		* SearchLogger is an template of log controller that is used for search logs.
		*
		* [State] is the game state type.
		* [Action] is the game action type.
		* [Result] is the game result type and could be ignore if it is unnecessary.
		*/
		template<typename State, typename Action, typename Result = int>
		class SearchLogger
		{
		private:
			using VisualTree = visual_tree::VisualTree;

		public:
			using StateToStrFunc = std::function<std::string(const State& state)>;
			using ActionToStrFunc = std::function<std::string(const Action& action)>;
			using ResultToStrFunc = std::function<std::string(const Result& result)>;

		private:
			//convert functions
			bool			_initialized;
			StateToStrFunc	_state_to_str_func;
			ActionToStrFunc	_action_to_str_func;
			ResultToStrFunc	_result_to_str_func;

			//log control
			bool			_enable_log;
			std::ostream*	_log_ostream;

			//json output control
			bool			_enable_json_output;
			std::string		_json_output_folder;
			VisualTree		_visual_tree;

		public:
			//default constructor.
			SearchLogger() :
				_initialized(false),
				_state_to_str_func([](const State&)->std::string {return ""; }),
				_action_to_str_func([](const Action&)->std::string {return ""; }),
				_enable_log(false),
				_log_ostream(&std::cout),
				_enable_json_output(false),
				_json_output_folder("JsonOutput"),
				_visual_tree()
			{
			}

			//initialized constructor.
			SearchLogger(
				StateToStrFunc state_to_str_func,
				ActionToStrFunc action_to_str_func,
				ResultToStrFunc result_to_str_func = [](const Result&)->std::string {return ""; }
			) :
				_initialized(true),
				_state_to_str_func(state_to_str_func),
				_action_to_str_func(action_to_str_func),
				_result_to_str_func(result_to_str_func),
				_enable_log(false),
				_log_ostream(std::cout),
				_enable_json_output(false),
				_json_output_folder("JsonOutput"),
				_visual_tree()
			{
			}

			//return true if log enabled.
			inline bool log_enabled() const { return _enable_log; }

			//return true if json output enabled
			inline bool json_output_enabled() const { return _enable_json_output; };

			//get log output stream.
			inline std::ostream& log_ostream() const
			{
				return *_log_ostream;
			}

			//get json output path
			inline std::string json_output_folder() const
			{
				return _json_output_folder;
			}

			//get ref of visual tree.
			inline VisualTree& visual_tree()
			{
				return _visual_tree;
			}

			//get state to string function.
			inline StateToStrFunc state_to_str_func() const
			{
				return _state_to_str_func;
			}

			//get action to string function.
			inline ActionToStrFunc action_to_str_func() const
			{
				return _action_to_str_func;
			}

			//get result to string function.
			inline ResultToStrFunc result_to_str_func() const
			{
				return _result_to_str_func;
			}

			//initialize logger.
			void Init(
				StateToStrFunc state_to_str_func,
				ActionToStrFunc action_to_str_func,
				ResultToStrFunc result_to_str_func
			)
			{
				_initialized = true;
				_state_to_str_func = state_to_str_func;
				_action_to_str_func = action_to_str_func;
				_result_to_str_func = result_to_str_func;
			}

			//enable log.
			inline void EnableLog(std::ostream& os)
			{
				_enable_log = true;
				_log_ostream = &os;
			}

			//disable log.
			inline void DisableLog()
			{
				_enable_log = false;
			}

			//enable json output
			inline void EnableJsonOutput(std::string json_output_folder)
			{
				_enable_json_output = true;
				_json_output_folder = json_output_folder;
			}

			//disable json output.
			inline void DisableJsonOutput()
			{
				_enable_json_output = false;
			}

			//output json to path.
			inline void OutputJson() const
			{
				std::string path = "./" + _json_output_folder;
				if (!file::DirExist(path))
				{
					file::MakeDir(path);
				}
				path += "/";
				path += timer::TimeString();
				path += ".json";
				std::ofstream ofs(path);
				_visual_tree.output_json(ofs);
			}

		};
	}
}