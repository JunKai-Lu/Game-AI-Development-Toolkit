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
	namespace visual_tree
	{
		extern const char* g_VISUAL_TREE_CHILD_KEY;

		class VisualTree;
		class TreeNode;

		//dictionary value.
		class DictValue
		{
			friend class TreeNode;

		private:
			enum ValueType :uint8_t
			{
				NULL_TYPE = 0,
				INTEGER_TYPE = 1,
				FLOAT_TYPE = 2,
				STRING_TYPE = 3,
				BOOLEAN_TYPE = 4
			};

			const ValueType	_type;
			int				_integer_value;
			double			_float_value;
			std::string		_string_value;
			bool			_boolean_value;

		public:
			inline explicit DictValue() :
				_type(NULL_TYPE)
			{

			}
			inline explicit DictValue(int value) :
				_type(INTEGER_TYPE),
				_integer_value(value),
				_float_value(0),
				_string_value(""),
				_boolean_value(false)
			{
			}
			inline explicit DictValue(size_t value) :
				_type(INTEGER_TYPE),
				_integer_value((int)value),
				_float_value(0),
				_string_value(""),
				_boolean_value(false)
			{
			}
			inline explicit DictValue(double value) :
				_type(FLOAT_TYPE),
				_integer_value(0),
				_float_value(value),
				_string_value(""),
				_boolean_value(false)
			{
			}
			inline explicit DictValue(std::string value) :
				_type(STRING_TYPE),
				_integer_value(0),
				_float_value(0),
				_string_value(value),
				_boolean_value(false)
			{
			}
			inline explicit DictValue(bool value) :
				_type(BOOLEAN_TYPE),
				_integer_value(0),
				_float_value(0),
				_string_value(""),
				_boolean_value(value)
			{
			}

			//return true if the value is null.
			inline bool is_null() const
			{
				return _type == NULL_TYPE;
			}

			//return true if the value is integer.
			inline bool is_integer() const
			{
				return _type == INTEGER_TYPE;
			}

			//return true if the value is float number.
			inline bool is_float() const
			{
				return _type == FLOAT_TYPE;
			}

			//return true if the value is string.
			inline bool is_string() const
			{
				return _type == STRING_TYPE;
			}

			//return true if the value is boolean.
			inline bool is_boolean() const
			{
				return _type == BOOLEAN_TYPE;
			}

			//get integer value.
			inline int integer_value() const
			{
				return _integer_value;
			}

			//get float value.
			inline double float_value() const
			{
				return _float_value;
			}

			//get string value.
			inline std::string string_value() const
			{
				return _string_value;
			}

			//get boolean value.
			inline bool boolean_value() const
			{
				return _boolean_value;
			}
		};

		//the node class of visual tree.
		class TreeNode
		{
			friend class VisualTree;
		public:
			using pointer = TreeNode*;
			using reference = TreeNode&;
			using ChildPtrSet = std::vector<pointer>;
			using NodeDict = std::map<std::string, DictValue>;

		private:
			NodeDict		_dict;
			ChildPtrSet		_childs;
			const pointer	_parent;
			const size_t	_depth;
			size_t			_count;
			VisualTree*		_owner;

		private:
			//constructor function.
			inline TreeNode(pointer parent, size_t depth, VisualTree* owner) :
				_parent{ parent },
				_depth{ depth },
				_count{ 1 },
				_owner{ owner }
			{
			}

			//default copy constructor is deleted.
			TreeNode(const TreeNode&) = delete;

			//copy constructor.
			TreeNode(const TreeNode& node, pointer parent, VisualTree* owner);

			//destructor function.
			inline ~TreeNode()
			{
				for (pointer p : _childs)
				{
					delete p;
				}
			}

			//increase count.
			void incr_count();

			//refresh count of all nodes.
			size_t refresh_count();

		public:
			//return true if the keye exist in the dict.
			inline bool exist_value(std::string key) const
			{
				return _dict.count(key) != 0;
			}

			//get integer value by key.
			inline int integer_value(std::string key)
			{
				if (exist_value(key))
				{
					if (_dict[key].is_integer())
					{
						return _dict[key].integer_value();
					}
				}
				return 0;
			}

			//get float value by key.
			inline double float_value(std::string key)
			{
				if (exist_value(key))
				{
					if (_dict[key].is_float())
					{
						return _dict[key].float_value();
					}
				}
				return 0;
			}

			//get string value by key.
			inline std::string string_value(std::string key)
			{
				if (exist_value(key))
				{
					if (_dict[key].is_string())
					{
						return _dict[key].string_value();
					}
				}
				return std::string("");
			}

			//add value to node, return true if add successfully.
			template<typename T>
			inline bool add_value(std::string key, T value)
			{
				if (key != g_VISUAL_TREE_CHILD_KEY)
				{
					_dict.insert({ key,DictValue(value) });
					return true;
				}
				return false;

			}

			//get first added child, return nullptr if not child exist.
			inline pointer first_child() const
			{
				if (_childs.size() > 0)
				{
					return _childs[0];
				}
				return nullptr;
			}

			//get last added child, return nullptr if not child exist.
			inline pointer last_child() const
			{
				if (_childs.size() > 0)
				{
					return _childs[_childs.size() - 1];
				}
				return nullptr;
			}

			//get number of child nodes.
			inline size_t child_num() const
			{
				return _childs.size();
			}

			//get depth of current node.
			inline size_t depth() const
			{
				return _depth;
			}

			inline VisualTree* owner_tree()
			{
				return _owner;
			}

			//return the count of nodes in the subtree from current node.
			inline size_t count() const
			{
				return _count;
			}

			//create a new child and return its index.
			inline pointer create_child()
			{
				pointer p = new TreeNode(this, _depth + 1, _owner);
				_childs.push_back(p);
				incr_count();
				return last_child();
			}

			//create child with obj
			template <typename T>
			inline pointer create_child(const T& obj)
			{
				pointer p = create_child();
				*p << obj;
				return p;
			}

			//create child with obj and callback function.
			template <typename T>
			inline pointer create_child(const T& obj, std::function<void(const T&,reference)> callback)
			{
				pointer p = create_child();
				callback(obj, *p);
				return p;
			}

			//traverse all nodes.
			void traverse_subtree(std::function<void(reference)> callback);

			//to json string
			std::string to_json() const;
		};

		//the visual tree.
		class VisualTree
		{
		private:
			TreeNode _root_node;

		public:

			//default constructor.
			VisualTree();

			//copy constructor.
			VisualTree(const VisualTree& tree);

			//get root node of the tree.
			inline TreeNode& root_node()
			{
				return _root_node;
			}

			//get number of nodes in the tree.
			inline size_t size() const
			{
				return _root_node.count();
			}

			//traverse nodes.
			inline void traverse_nodes(std::function<void(TreeNode&)> callback)
			{
				_root_node.traverse_subtree(callback);
			}

			//to json string.
			inline std::string to_json()
			{
				return _root_node.to_json();
			}

			//output json to ostream.
			inline void output_json(std::ostream& os)
			{
				os << to_json();
			}
		};

	}
}