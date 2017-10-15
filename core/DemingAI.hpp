//DemingAI.hpp 
#ifndef DEMINGAI_HPP
#define DEMINGAI_HPP
#include "OthelloAI.hpp"
using namespace std; 


namespace demingh
{
	class DemingAI: public OthelloAI
	{
		public:
			virtual pair<int, int> chooseMove(const OthelloGameState& state);
			vector<pair<int, int>> possible_move(unique_ptr<OthelloGameState>& state);
			int search(unique_ptr<OthelloGameState>& state, int depth, int turn);
			vector<pair<int,int>> dangerous_area(vector<pair<int, int>> move,unique_ptr<OthelloGameState>& state);
			int get_width(unique_ptr<OthelloGameState>& state);
			int get_height(unique_ptr<OthelloGameState>& state);
			int Ismyturn(unique_ptr<OthelloGameState>& state);
	};
}


#endif 