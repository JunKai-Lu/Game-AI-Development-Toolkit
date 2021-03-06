﻿/* Copyright (c) 2018 Junkai Lu <junkai-lu@outlook.com>.
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

#include "../src/gadtlib.h"
#include "../src/visual_tree.h"
#include "../src/bitboard.hpp"
#include "../src/gadt_game.hpp"
#include "../src/gadt_container.hpp"
#include "../src/gadt_memory.hpp"
#include "../src/gadt_algorithm.hpp"
#include "../src/gadt_log.hpp"
#include "../src/gadt_table.h"
#include "../src/gadt_filesystem.h"
#include "../src/monte_carlo.hpp"
#include "../src/minimax.hpp"
#include "../src/mcts.hpp"
#include "../src/parallel_mcts.hpp"
#include "../src/gadt_io.h"
#include "../src/gshell_args.hpp"

#pragma once

#define GADT_ASSERT(fir,sec) if(fir != sec)std::cout<<"Assert Failed, [func = "<<__FUNCTION__<<" line = "<<__LINE__<<"]"<<std::endl;

namespace gadt
{
	namespace unittest
	{
		namespace tic_tac_toe
		{
			enum Player :AgentIndex
			{
				BLACK = -1,
				WHITE = 1,
				EMPTY = GADT_DEFAULT_NO_WINNER_INDEX,
				DRAW = 2
			};

			struct State
			{
			public:
				Player dot[3][3];
				Player next_player;

				State():
					next_player(BLACK)
				{
					for (size_t x = 0; x < 3; x++)
					{
						for (size_t y = 0; y < 3; y++)
						{
							dot[x][y] = EMPTY;
						}
					}
				}
			};

			struct Action
			{
			public:
				size_t x;
				size_t y;
				Player player;
			};

			using Result = Player;
			using ActionSet = std::vector<Action>;

			void UpdateState(State& state, const Action& action);
			void MakeAction(const State& state, ActionSet& as);
			Player DetemineWinner(const State& state);
			Result StateToResult(const State& state, AgentIndex winner);
			bool AllowUpdateValue(const State& state, Result winner);
		}

		void TestConvertFunc();
		void TestPoint();
		void TestBitBoard();
		void TestBitBoard64();
		void TestBitArray();
		void TestBitArraySet();
		void TestFilesystem ();
		void TestIndex ();
		void TestMctsNode ();
		void TestMctsSearch ();
		void TestVisualTree ();
		void TestStlAllocator ();
		void TestStlLinearAlloc ();
		void TestStlList ();
		void TestStlStaticMatrix ();
		void TestStlDynamicMatrix ();
		void TestTable ();
		void TestRandomPool ();
		void TestMinimax ();
		void TestMonteCarlo ();
		void TestDynamicArray ();
		void TestPodFileIO();
		void TestArgConvertor();
	}
}
