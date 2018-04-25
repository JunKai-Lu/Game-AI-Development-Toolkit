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

#include "gadt_unittest.h"

using std::cout;
using std::endl;

namespace gadt
{
	namespace unittest
	{
		namespace tic_tac_toe
		{
			void UpdateState(State& state, const Action& action)
			{
				state.dot[action.x][action.y] = action.player;
				state.next_player = action.player == WHITE ? BLACK : WHITE;
			}

			void MakeAction(const State& state, ActionSet& as)
			{
				for (size_t x = 0; x < 3; x++)
				{
					for (size_t y = 0; y < 3; y++)
					{
						if (state.dot[x][y] == EMPTY)
						{
							as.push_back({ x,y,state.next_player });
						}
					}
				}
			}

			Player DetemineWinner(const State& state)
			{
				bool all_filled = true;
				for (size_t i = 0; i < 3; i++)
				{
					for (size_t y = 0; y < 3; y++)
					{
						if (state.dot[i][y] == EMPTY)
						{
							all_filled = false;
						}
					}
					if (state.dot[i][0] == state.dot[i][1] && state.dot[i][1] == state.dot[i][2] && state.dot[i][0] != EMPTY)
					{
						return state.dot[i][0];
					}
					if (state.dot[0][i] == state.dot[1][i] && state.dot[1][i] == state.dot[2][i] && state.dot[0][i] != EMPTY)
					{
						return state.dot[0][i];
					}
				}
				if (state.dot[0][0] == state.dot[1][1] && state.dot[1][1] == state.dot[2][2] && state.dot[1][1] != EMPTY)
				{
					return state.dot[0][0];
				}
				if (state.dot[2][0] == state.dot[1][1] && state.dot[1][1] == state.dot[0][2] && state.dot[1][1] != EMPTY)
				{
					return state.dot[2][0];
				}
				if (all_filled)
				{
					return DRAW;
				}
				return EMPTY;
			}

			Result StateToResult(const State& state, AgentIndex winner)
			{
				return (Result)winner;
			}

			bool AllowUpdateValue(const State& state, Result winner)
			{
				if (state.next_player == BLACK && winner == WHITE)
				{
					return true;
				}
				if (state.next_player == WHITE && winner == BLACK)
				{
					return true;
				}
				return false;
			}

			std::string StateToStr(const State& state)
			{
				std::stringstream ss;
				for (size_t x = 0; x < 3; x++)
				{
					for (size_t y = 0; y < 3; y++)
					{
						switch (state.dot[x][y])
						{
						case BLACK:
							ss << "O";
							break;
						case WHITE:
							ss << "X";
							break;
						default:
							ss << "_";
							break;
						}
						//ss << state.dot[x][y];
					}
					ss << " ";
				} 
				ss << "  [" << state.next_player << "] is next";
				return ss.str();
			}

			std::string ActionToStr(const Action& action)
			{
				std::stringstream ss;
				ss << "player "<<action.player << " [" << action.x << "," << action.y << "]";
				return ss.str();
			}

			std::string ResultToStr(const Result& result)
			{
				std::stringstream ss;
				ss << result;
				return ss.str();
			}

			EvalValue EvalForParent(const State& state, const AgentIndex winner)
			{
				if (winner == DRAW)
				{
					return 0;
				}
				return state.next_player == winner ? -999 : 999;
			}
		}

		void TestCoordinate()
		{
			//test signed coordinate
			BasicCoordinate<int64_t> coord64(-100, -200);
			BasicCoordinate<int8_t> coord8(-1, -2);

			//comparison
			GADT_ASSERT(coord64 == BasicCoordinate<int64_t>(-100, -200), true);
			GADT_ASSERT(coord64 != BasicCoordinate<int64_t>(-100, 0), true);
			GADT_ASSERT(coord64 != BasicCoordinate<int64_t>(0, -200), true);

			//integer
			GADT_ASSERT(coord64 * -10 == BasicCoordinate<int16_t>(1000, 2000), true);
			GADT_ASSERT(coord64 / -10 == BasicCoordinate<int16_t>(10, 20), true);
			coord64 *= -10;
			GADT_ASSERT(coord64 == BasicCoordinate<int16_t>(1000, 2000), true);
			coord64 /= -10;
			GADT_ASSERT(coord64 == BasicCoordinate<int16_t>(-100, -200), true);

			//coordinate
			GADT_ASSERT((coord64 + coord8) == BasicCoordinate<int16_t>(-101, -202), true);
			GADT_ASSERT((coord64 - coord8) == BasicCoordinate<int16_t>(-99, -198), true);
			coord64 += coord8;
			GADT_ASSERT(coord64 == BasicCoordinate<int16_t>(-101, -202), true);
			coord64 -= coord8;
			GADT_ASSERT(coord64 == BasicCoordinate<int16_t>(-100, -200), true);
			GADT_ASSERT(coord64 * coord8, 500);

			//test unsigned coordinate
			BasicUnsignedCoordinate<uint8_t> ucoord8(1, 2);
			BasicUnsignedCoordinate<uint64_t> ucoord64(100, 200);

			//comparison
			GADT_ASSERT(ucoord64 == BasicUnsignedCoordinate<uint64_t>(100, 200), true);
			GADT_ASSERT(ucoord64 != BasicUnsignedCoordinate<uint64_t>(100, 0), true);
			GADT_ASSERT(ucoord64 != BasicUnsignedCoordinate<uint64_t>(0, 200), true);

			//uinteger
			GADT_ASSERT(ucoord64 * 10 == BasicUnsignedCoordinate<uint16_t>(1000, 2000), true);
			GADT_ASSERT(ucoord64 / 10 == BasicUnsignedCoordinate<uint16_t>(10, 20), true);
			ucoord64 *= 10;
			GADT_ASSERT(ucoord64 == BasicUnsignedCoordinate<uint16_t>(1000, 2000), true);
			ucoord64 /= 10;
			GADT_ASSERT(ucoord64 == BasicUnsignedCoordinate<uint16_t>(100, 200), true);

			//ucoordinate
			GADT_ASSERT((ucoord64 + ucoord8) == BasicUnsignedCoordinate<uint16_t>(101, 202), true);
			GADT_ASSERT((ucoord64 - ucoord8) == BasicUnsignedCoordinate<uint16_t>(99, 198), true);
			ucoord64 += ucoord8;
			GADT_ASSERT(ucoord64 == BasicUnsignedCoordinate<uint16_t>(101, 202), true);
			ucoord64 -= ucoord8;
			GADT_ASSERT(ucoord64 == BasicUnsignedCoordinate<uint16_t>(100, 200), true);
			GADT_ASSERT(ucoord64 * ucoord8, 500);
		}
		void TestBitBoard()
		{
			//test BitBoard<>
			bitboard::BitBoard<56> temp;
			bitboard::BitBoard<56> subtemp;
			subtemp.set(1);
			size_t count = 0;
			for (bool v : subtemp)
			{
				GADT_ASSERT(v, (count == 1 ? true : false));
				count++;
			}
			GADT_ASSERT(count, temp.upper_bound());
			GADT_ASSERT(0, temp.total());
			GADT_ASSERT(false, temp.any());
			GADT_ASSERT(true, temp.none());
			temp.set(1);
			GADT_ASSERT(true, subtemp.is_subset_of(temp));
			GADT_ASSERT(true, temp.exist_subset(subtemp));
			GADT_ASSERT(true, temp.any());
			GADT_ASSERT(false, temp.none());
			temp.set(51);
			temp.set(26);
			subtemp.set(51);
			GADT_ASSERT(true, subtemp.is_subset_of(temp));
			GADT_ASSERT(true, temp.exist_subset(subtemp));
			GADT_ASSERT(3, temp.total());
			temp.reset(1);
			temp.write(26, 0);
			GADT_ASSERT(1, temp.total());
			GADT_ASSERT(true, temp.get(51));
			GADT_ASSERT(56, temp.upper_bound());



			//test BitBoard64
			bitboard::BitBoard64 temp_64;
			GADT_ASSERT(0, temp_64.total());
			GADT_ASSERT(false, temp_64.any());
			GADT_ASSERT(true, temp_64.none());
			temp_64.set(1);
			GADT_ASSERT((temp_64 ^ temp_64).total(), 0);
			GADT_ASSERT((~temp_64).get(1), false);
			GADT_ASSERT((~temp_64).get(0), true);
			GADT_ASSERT((~temp_64).get(2), true);
			count = 0;
			for (bool v : temp_64)
			{
				GADT_ASSERT(v, (count == 1 ? true : false));
				count++;
			}
			GADT_ASSERT(count, temp_64.upper_bound());
			GADT_ASSERT(true, temp_64.any());
			GADT_ASSERT(false, temp_64.none());
			temp_64.set(51);
			temp_64.set(26);
			GADT_ASSERT(3, temp_64.total());
			temp_64.reset(1);
			temp_64.write(26, 0);
			GADT_ASSERT(1, temp_64.total());
			GADT_ASSERT(true, temp_64.get(51));
			GADT_ASSERT(64, temp_64.upper_bound());

			//test BitPoker
			bitboard::BitPoker temp_poker;
			bitboard::BitPoker subtemp_poker;
			GADT_ASSERT(0, temp_poker.total());
			GADT_ASSERT(false, temp_poker.any());
			GADT_ASSERT(true, temp_poker.none());
			temp_poker.set(1, 1);
			GADT_ASSERT(true, temp_poker.any());
			GADT_ASSERT(false, temp_poker.none());
			temp_poker.set(2, 2);
			temp_poker.set(3, 3);
			count = 0;
			for (size_t v : temp_poker)
			{
				GADT_ASSERT(v, (count > 0 && count < 4 ? count : 0));
				count++;
			}
			GADT_ASSERT(count, temp_poker.upper_bound());
			subtemp_poker.set(1, 1);
			subtemp_poker.set(2, 1);
			subtemp_poker.set(3, 1);
			GADT_ASSERT(true,temp_poker.exist_subset(subtemp_poker));
			GADT_ASSERT(true,subtemp_poker.is_subset_of(temp_poker));
			GADT_ASSERT(6, temp_poker.total());
			temp_poker.reset(1);
			GADT_ASSERT(5, temp_poker.total());
			temp_poker.push(3);
			GADT_ASSERT(4, temp_poker.get(3));
			temp_poker.decrease(3);
			GADT_ASSERT(3, temp_poker.get(3));
			temp_poker.increase(4);
			GADT_ASSERT(1, temp_poker.get(4));

			//test BitMahjong
			bitboard::BitMahjong temp_mahjong;
			GADT_ASSERT(0, temp_mahjong.total());
			GADT_ASSERT(false, temp_mahjong.any());
			GADT_ASSERT(true, temp_mahjong.none());
			temp_mahjong.set(1, 1);
			GADT_ASSERT(true, temp_mahjong.any());
			GADT_ASSERT(false, temp_mahjong.none());
			temp_mahjong.set(2, 2);
			temp_mahjong.set(3, 3);
			count = 0;
			for (size_t v : temp_mahjong)
			{
				GADT_ASSERT(v, (count > 0 && count < 4 ? count : 0));
				count++;
			}
			GADT_ASSERT(count, temp_mahjong.upper_bound());
			GADT_ASSERT(6, temp_mahjong.total());
			temp_mahjong.reset(1);
			GADT_ASSERT(5, temp_mahjong.total());
			temp_mahjong.push(3);
			GADT_ASSERT(4, temp_mahjong.get(3));
			temp_mahjong.decrease(3);
			GADT_ASSERT(3, temp_mahjong.get(3));
			temp_mahjong.increase(4);
			GADT_ASSERT(1, temp_mahjong.get(4));
			temp_mahjong.push(25);
			GADT_ASSERT(1, temp_mahjong.get(25));
			temp_mahjong.decrease(25);
			GADT_ASSERT(0, temp_mahjong.get(25));
			temp_mahjong.increase(25);
			GADT_ASSERT(1, temp_mahjong.get(25));
			GADT_ASSERT(7, temp_mahjong.total());

			//test ValueVector
			bitboard::ValueVector<14> temp_vec;
			temp_vec.push(2);
			count = 0;
			for (auto v : temp_vec)
			{
				GADT_ASSERT(v, 2);
				count++;
			}
			GADT_ASSERT(count, temp_vec.length());
			GADT_ASSERT(1, temp_vec.length());
			GADT_ASSERT(2, temp_vec.draw_value());
			GADT_ASSERT(2, temp_vec.draw_and_remove_value());
			GADT_ASSERT(0, temp_vec.length());
			temp_vec.push(3);
			GADT_ASSERT(1, temp_vec.length());

			//test ValueVector
			bitboard::ValueVector<5> temp_vec_5{1,2,3,4,5,6};
			GADT_ASSERT(temp_vec_5.get(4), 5);
			GADT_ASSERT(temp_vec_5.length(), 5);
			GADT_ASSERT(temp_vec_5.is_full(), true);
			GADT_ASSERT(temp_vec_5[0], 1);
			GADT_ASSERT(temp_vec_5.upper_bound(), 5);
		}
		void TestFileLib()
		{
			srand((unsigned int)time(NULL));
			std::string path = "./test_dir"  + ToString(rand() % 99999999);
			if (!file::DirExist(path))
			{
				GADT_ASSERT(true, file::MakeDir(path));
				GADT_ASSERT(true, file::DirExist(path));
				GADT_ASSERT(true, file::RemoveDir(path));
				GADT_ASSERT(false, file::DirExist(path));
			}
		}
		void TestIndex()
		{
			player::PlayerRange<AgentIndex, 1, 5> index;
			AgentIndex player = 2;
			player = index.get_next(player);
			GADT_ASSERT(3, player);
			GADT_ASSERT(4, index.get_next(player));
			GADT_ASSERT(2, index.get_prev(player));
			GADT_ASSERT(5, index.get_jump(player,2));
			GADT_ASSERT(1, index.get_jump(player, 3));
			GADT_ASSERT(2, index.get_jump(player, 14));
			GADT_ASSERT(2, index.get_jump(player, 24));
		}
		void TestMctsNode()
		{
			tic_tac_toe::State state;
			mcts::MctsFuncPackage<tic_tac_toe::State, tic_tac_toe::Action, tic_tac_toe::Result, true> func(
				tic_tac_toe::UpdateState,
				tic_tac_toe::MakeAction,
				tic_tac_toe::DetemineWinner,
				tic_tac_toe::StateToResult,
				tic_tac_toe::AllowUpdateValue
			);
			mcts::MctsNode<tic_tac_toe::State, tic_tac_toe::Action, tic_tac_toe::Result, true> node(state, nullptr, func, mcts::MctsSetting());
			gadt::stl::StackAllocator<mcts::MctsNode<tic_tac_toe::State, tic_tac_toe::Action, tic_tac_toe::Result, true>, true> alloc(100);

			auto p = alloc.construct(state, nullptr, func, mcts::MctsSetting());
			GADT_ASSERT(node.action_num(), 9);
			GADT_ASSERT(p->action_num(), 9);
		}
		void TestMctsSearch()
		{
			mcts::MctsSetting setting;
			setting.thread_num = 4;
			setting.max_node_per_thread = 10000;
			setting.max_iteration_per_thread = 10000;
			setting.timeout = 0;

			mcts::MonteCarloTreeSearch<tic_tac_toe::State, tic_tac_toe::Action, tic_tac_toe::Result, true> mcts
			(
				tic_tac_toe::UpdateState,
				tic_tac_toe::MakeAction,
				tic_tac_toe::DetemineWinner,
				tic_tac_toe::StateToResult,
				tic_tac_toe::AllowUpdateValue
			);
			mcts.InitLog(tic_tac_toe::StateToStr, tic_tac_toe::ActionToStr, tic_tac_toe::ResultToStr);
			//mcts.EnableJsonOutput();
			//mcts.EnableLog();
			tic_tac_toe::State state;
			state.dot[0][0] = tic_tac_toe::WHITE;
			/*for (size_t i = 1; i <= 16; i++)
			{
				setting.thread_num = i;
				tic_tac_toe::Action action = mcts.DoMcts(state, setting);
			}*/
			tic_tac_toe::Action action = mcts.DoMcts(state, setting);
			GADT_ASSERT((action.x == 1 && action.y == 1), true);
		}
		void TestVisualTree()
		{
			visual_tree::VisualTree tree;
			visual_tree::VisualNode::pointer ptr = tree.root_node();
			const size_t ub = 100;
			for (size_t  i = 0; i < ub; i++)
			{
				ptr->set_value("depth", ptr->depth());
				ptr->add_value("sqrt", sqrt((double)ptr->depth()));
				ptr->set_value("name", "hello ");
				ptr->add_value("name", "world!");
				GADT_ASSERT(ptr->string_value("depth"), "");
				GADT_ASSERT((size_t)ptr->integer_value("depth"), ptr->depth());
				ptr->create_child();
				ptr->last_child()->set_value("depth", ptr->last_child()->depth());
				ptr = ptr->create_child();
			}
			ptr->add_value("hello", "world");
			visual_tree::VisualTree new_tree = tree;
			std::ofstream os("JsonTest1.dat");
			new_tree.output_json(os);
			GADT_ASSERT(new_tree.size(), ub*2 +1);
			GADT_ASSERT(new_tree.root_node()->first_child()->count(), 1);
			GADT_ASSERT(new_tree.root_node()->last_child()->count(), ub * 2 - 1);
			tree.clear();
			ptr = tree.root_node()->create_child();
			for (size_t i = 0; i < 10; i++)
			{
				ptr->set_value("number", i);
				ptr->create_child()->set_value("is_leaf", true);
				ptr = ptr->create_child();
			}
			GADT_ASSERT(tree.root_node()->first_child()->first_child()->boolean_value("is_leaf"), true);
			std::ofstream os2("JsonTest2.dat");
			tree.output_json(os2);
		}
		void TestStlList()
		{
			using ActionList = gadt::stl::List<tic_tac_toe::Action, true>;
			ActionList list(1000);
			for (size_t i = 0; i < 9; i++)
			{
				GADT_ASSERT(list.size(), i);
				list.push_back({ i / 3,i % 3,tic_tac_toe::BLACK });
			}
			size_t count = 0;
			for (list.reset_iterator(); list.is_end() == false; list.to_next_iterator())
			{
				GADT_ASSERT(list.iterator().x, count / 3);
				GADT_ASSERT(list.iterator().y, count % 3);
				count++;
			}
			GADT_ASSERT(count, 9);
			list.clear();
			GADT_ASSERT(list.begin(), nullptr);
			GADT_ASSERT(list.end(), nullptr);
			GADT_ASSERT(list.to_next_iterator(), false);
		}
		void TestStlAllocator()
		{
			struct TestClass
			{
				size_t a;
				size_t b;
				size_t c;
				std::vector<size_t> num;

				TestClass(size_t _a, size_t _b, size_t _c) :
					a(_a),
					b(_b),
					c(_c),
					num{ _a,_b,_c }
				{
				}
			};
			using TestAlloc = stl::StackAllocator<TestClass, true>;
			const size_t ub = 10000;
			TestAlloc alloc(ub);
			for (size_t i = 0; i < ub / 2; i++)
			{
				TestClass* p = alloc.construct(i, i + i, i*i);
				GADT_ASSERT(p->a, i);
				alloc.destory(p);
			}
			GADT_ASSERT(alloc.is_full(), false);
			GADT_ASSERT(alloc.remain_size(), alloc.total_size());
			for (size_t i = 0; i < ub; i++)
			{
				TestClass* p = alloc.construct(i, i + i, i*i);
				GADT_ASSERT(p->a, i);
			}
			GADT_ASSERT(alloc.is_full(), true);
			alloc.flush();
			GADT_ASSERT(alloc.is_empty(), true);
		}
		void TestStlLinearAlloc()
		{
			struct TestClass
			{
				size_t a;
				size_t b;
				size_t c;
				std::vector<size_t> num;

				TestClass(size_t _a, size_t _b, size_t _c) :
					a(_a),
					b(_b),
					c(_c),
					num{ _a,_b,_c }
				{
				}
			};
			using TestAlloc = stl::LinearAllocator<TestClass, true>;
			const size_t ub = 10000;
			TestAlloc alloc(ub);
			for (size_t i = 0; i < ub / 2; i++)
			{
				TestClass* p = alloc.construct(i, i + i, i*i);
				GADT_ASSERT(p->a, i);
				//GADT_ASSERT(p->num[0], i);
				alloc.destory_last();
			}
			GADT_ASSERT(alloc.is_full(), false);
			GADT_ASSERT(alloc.remain_size(), alloc.total_size());
			for (size_t i = 0; i < ub; i++)
			{
				TestClass* p = alloc.construct(i, i + i, i*i);
			}
			GADT_ASSERT(alloc.is_full(), true);
			alloc.flush();
			GADT_ASSERT(alloc.is_empty(), true);
		}
		void TestStlElementMatrix()
		{
			stl::ElementMatrix<size_t> martix(4, 4);
			for (auto coord : martix)
				martix[coord] = coord.x * coord.y;
			auto trans_func = [](size_t i)->std::string { return gadt::ToString(i); };
			GADT_ASSERT(martix.element(3, 3), 9);
			martix.IncreaseRow(5, 0);
			GADT_ASSERT(martix.element(3, 8), 0);
			GADT_ASSERT(martix.height(), 9);
			martix.DecreaseRow(4);
			GADT_ASSERT(martix.element(3, 4), 0);
			GADT_ASSERT(martix.height(), 5);
			martix.IncreaseColumn(3, 0);
			GADT_ASSERT(martix.element(6, 3), 0);
			GADT_ASSERT(martix.width(), 7);
			martix.DecreaseColumn(4);
			GADT_ASSERT(martix.width(), 3);
			martix.DecreaseColumn(4);
			GADT_ASSERT(martix.width(), 0);
			martix.DecreaseRow(5);
			GADT_ASSERT(martix.height(), 0);
			martix.Resize(5, 5);
			GADT_ASSERT(martix.element(4, 4), 0);
			martix.Resize(4, 4);
			GADT_ASSERT(martix.element(3, 3), 0);
		}
		void TestStlRectangeArray()
		{

		}
		void TestTable()
		{
			std::ios::sync_with_stdio(false);
			log::ConsoleTable table(3, 5,{
				{ "1","2","3" },
				{ "4","5","6" },
				{ "7","8","9" },
				{ "10","11","12" }
			});
			table.set_width({ 4,4,4,4 });
			GADT_ASSERT(table.get_row(0).size(), 3);
			GADT_ASSERT(table.get_column(0).size(), 5);
			GADT_ASSERT(table.get_cell(1,0).str, "2");
			table.set_cell_in_row(0, { "hello", console::BLUE, log::ALIGN_RIGHT });
			table.set_cell_in_column(1, { "world", console::RED, log::ALIGN_MIDDLE });
			GADT_ASSERT(table.get_cell(0, 0).str, "hello");
			GADT_ASSERT(table.get_cell(2, 0).str, "hello");
			GADT_ASSERT(table.get_cell(1, 2).str, "world");
			//table.print();
			//std::cout << table.output_string();

		}
		void TestMinimax()
		{
			const double timeout = 10;
			const size_t max_depth = 10;
			const bool enable_ab = false;
			
			minimax::MinimaxSearch<tic_tac_toe::State, tic_tac_toe::Action, true> minimax(
				tic_tac_toe::UpdateState,
				tic_tac_toe::MakeAction,
				tic_tac_toe::DetemineWinner, 
				tic_tac_toe::EvalForParent
				);
			minimax.InitLog(tic_tac_toe::StateToStr, tic_tac_toe::ActionToStr);
			//minimax.EnableJsonOutput();
			//minimax.EnableLog();
			tic_tac_toe::State state;
			state.dot[0][0] = tic_tac_toe::WHITE;
			tic_tac_toe::Action action = minimax.DoNegamax(state, { timeout, max_depth, false });
			GADT_ASSERT((action.x == 1 && action.y == 1), true);
		}
		void TestRandomPool()
		{
			const size_t ub = 20;
			stl::RandomPool<size_t, true> pool(ub);
			for (size_t i = 0; i < ub; i++)
			{
				pool.add(i, i);
				GADT_ASSERT(pool.get_weight(i), i);
				GADT_ASSERT(pool[i], i);
				GADT_ASSERT(pool.get_element(i), i);
			}
			//std::cout << pool.info();
			GADT_ASSERT(pool.size(), ub);
			GADT_ASSERT(pool.random() > 0, true);
		}
		void TestMonteCarlo()
		{
			monte_carlo::MonteCarloSetting setting;
			monte_carlo::MonteCarloSimulation<tic_tac_toe::State, tic_tac_toe::Action, tic_tac_toe::Result, true> mc(
				tic_tac_toe::UpdateState,
				tic_tac_toe::MakeAction,
				tic_tac_toe::DetemineWinner,
				tic_tac_toe::StateToResult,
				tic_tac_toe::AllowUpdateValue
			);
			setting.thread_num = 4;
			setting.simulation_times = 10000;
			setting.timeout = 0;
			setting.enable_action_policy = true;

			tic_tac_toe::State state;
			state.dot[0][0] = tic_tac_toe::WHITE;
			mc.InitLog(tic_tac_toe::StateToStr, tic_tac_toe::ActionToStr);
			//mc.EnableLog();
			tic_tac_toe::Action action = mc.DoMonteCarlo(state, setting);
			//GADT_ASSERT((action.x == 1 && action.y == 1), true);
		}

		const std::vector<FuncPair> func_list = {
			{ "coordinate"		,TestCoordinate			},
			{ "bitboard"		,TestBitBoard			},
			{ "file"			,TestFileLib			},
			{ "index"			,TestIndex				},
			{ "mcts_node"		,TestMctsNode			},
			{ "mcts"			,TestMctsSearch			},
			{ "visual_tree"		,TestVisualTree			},
			{ "allocator"		,TestStlAllocator		},
			{ "linear_alloc"	,TestStlLinearAlloc		},
			{ "list"			,TestStlList			},
			{ "martix"			,TestStlElementMatrix	},
			{ "table"			,TestTable				},
			{ "random_pool"		,TestRandomPool			},
			{ "minimax"			,TestMinimax			},
			{ "monte_carlo"		,TestMonteCarlo			}
		};
		void RunTest(FuncPair func_pair)
		{
			cout << endl << ">> test start, target = ";
			console::Cprintf(func_pair.first, console::GREEN);
			auto t = timer::GetClock();
			cout << endl;
			func_pair.second();
			cout << ">> test complete, time = ";
			console::Cprintf(timer::GetTimeDifference(t), console::RED);
			cout << endl;
		}
	}
}