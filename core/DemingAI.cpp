//DemingAI.cpp
#include <ics46/factory/DynamicFactory.hpp> 
#include "DemingAI.hpp"
#include <limits> 

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, demingh::DemingAI, "Deming's AI (Required)");

int demingh::DemingAI:: Ismyturn(unique_ptr<OthelloGameState>& state)
/*get what whose turn if is black turn return 1 else, return 0 */
{
	if(state->isBlackTurn())
		return 1;
	else
		return 0;  
}

pair<int,int> demingh::DemingAI::chooseMove(const OthelloGameState& state)//should be no problem 
/*automatically make a move */
{		
	int best_score=numeric_limits<int>::min();
	pair<int, int> move; 
	unique_ptr<OthelloGameState> copy= state.clone(); 
	int myturn =Ismyturn(copy);
	vector<pair<int, int>> possible_moves =possible_move(copy);

	if(possible_moves.size()==1)
	{
		move.first=possible_moves[0].first; 
		move.second=possible_moves[0].second; 
	}
	else
		for (int i=0; i<possible_moves.size(); i++)
		{ 
			unique_ptr<OthelloGameState> copy1=state.clone(); 
			copy1->makeMove(possible_moves[i].first,possible_moves[i].second);
			int score=search(copy1,3,myturn);
			if(score>best_score)
			{ 
				best_score=score;
				move.first=possible_moves[i].first; 
				move.second=possible_moves[i].second;
			}
		} 
	return move;  
}

vector<pair<int, int>> demingh::DemingAI::possible_move(unique_ptr<OthelloGameState>& state)
/*move all possible move fromthe board to a vector */
{
	vector<pair<int,int>> move; 
	vector<pair<int,int>> danger_move; 
	vector<pair<int,int>> better_move; 
	for(int row=0;row<get_width(state);++row)
	{
		for(int col=0; col<get_height(state); ++col)
		{
			if(state->board().isValidCell(row,col))
			{
				if(state->isValidMove(row,col)) 
				{
					move.push_back(pair<int,int>(row,col));
				}
			}
		}
	}
	
	danger_move=dangerous_area(move, state);

	if(danger_move.size()==0)
	{
		return move; 
	}

	else if (danger_move.size()>=1)
	{
		for(auto element:move)
		{	
			int count=0; 
			for(auto bad_move_element:danger_move)
			{
				if(bad_move_element==element)
				{
					count++; 
				}
			}
			if(count==0)
			{
				better_move.push_back(element);
			}
		}
		if(better_move.size()>0)
		{
			return better_move; 
		}
		else  
			return move; 
	}
	else 
		return move; 
}

vector<pair<int,int>> demingh:: DemingAI:: dangerous_area(vector<pair<int, int>> move, unique_ptr<OthelloGameState>& state)
 /*to improve deming's AI slighly smarter through avoiding the three position aournd each corner.*/
{
	int width=get_width(state)-2;
	int height=get_height(state)-2; 
	vector<pair<int,int>> danger_area; 
	for (int n=0; n<move.size(); n++)
	{
		if(move[n]==pair<int,int>(0,1)||move[n]==pair<int,int>(1,0)||move[n]==pair<int,int>(1,1)) //top-left
		{
			danger_area.push_back(move[n]);
		}
		if(move[n]==pair<int,int>(0,height)||move[n]==pair<int,int>(1,height)||move[n]==pair<int,int>(1,height+1))
		{
			danger_area.push_back(move[n]);
		}
		if(move[n]==pair<int,int>(width,0)||move[n]==pair<int,int>(width,1)||move[n]==pair<int,int>(width+1,1))
		{
			danger_area.push_back(move[n]);
		}
		if(move[n]==pair<int,int>(width,height)||move[n]==pair<int,int>(width,height+1)||move[n]==pair<int,int>(width+1,height))
		{
			danger_area.push_back(move[n]);
		}
	}
	return danger_area; 
}

int demingh::DemingAI::get_width(unique_ptr<OthelloGameState>& state)
/*get width of the board */
{
	return state->board().width(); 
}
			
int demingh::DemingAI::get_height(unique_ptr<OthelloGameState>& state)
/*get the height of the board */
{
	return state->board().height(); 
}

int demingh::DemingAI::search(unique_ptr<OthelloGameState>& state, int depth,int turn)
/*using minMax algorithm to calculate future best possible move for Deming's AI  */ 
{	
	int best_score=numeric_limits<int>::min(); 
	int worst_score=numeric_limits<int>::max(); 
	vector<pair<int,int>> move=possible_move(state);
	if(depth==0||state->isGameOver())
	{	
		if(turn==1)
		{
			return state->blackScore()-state->whiteScore();
		}
		if(turn==0)
		{
			return state->whiteScore()-state->blackScore(); 
		}
	}
	else
		if((state->isBlackTurn()&&turn==1)||(state->isWhiteTurn()&&turn==0))
		{	
			for(int n=0; n<move.size(); n++)
			{
				unique_ptr<OthelloGameState> new_state=state->clone(); 
				new_state->makeMove(move[n].first,move[n].second);  
				int score1=search(new_state, depth-1,turn); 
				if(score1>best_score) 
				{
					best_score=score1;  
				}
			}
			return best_score; 
		}
		else
			for(int i=0; i<move.size(); i++)
			{
				unique_ptr<OthelloGameState> new_state=state->clone(); 
				new_state->makeMove(move[i].first,move[i].second);
				int score2=search(new_state, depth-1,turn);
				if(score2<worst_score)
				{
					worst_score=score2; 
				} 
			}
			return worst_score;
}