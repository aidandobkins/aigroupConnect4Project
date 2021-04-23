#include <iostream>
#include <stdio.h>
#include <array>
#include <limits.h>
#include <sstream>
#include <vector>

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

using namespace std;

void printBoard(vector<vector<int> >&);
int userMove();
void makeMove(vector<vector<int> >&, int, unsigned int);
void errorMessage(int);
int aiMove();
bool winningMove(vector<vector<int> >&, unsigned int);
int scoreSet(vector<unsigned int>, unsigned int);
int tabScore(vector<vector<int> >, unsigned int);
int heurFunction(unsigned int, unsigned int, unsigned int);
int heurFunction2(unsigned int, unsigned int, unsigned int);
int heurFunction3(unsigned int, unsigned int, unsigned int);
vector<vector<int> > copyBoard(vector<vector<int> >);
array<int, 2> miniMax(vector<vector<int> >&, unsigned int, int, int, unsigned
                      int);

unsigned int NUM_COL = 7; // Board width
unsigned int NUM_ROW = 6; // Board height
unsigned int COMPUTER1 = 1; // AI number 1
unsigned int COMPUTER2 = 2; // AI number 2
unsigned int depth = 2; // Designated difficulty level
int heur1 = 1;
int heur2 = 1;
bool gameOver = false; // sends a signal when the game has ended
unsigned int turns = 0; // count for number of turns
unsigned int currentPlayer = COMPUTER1;
vector<vector<int>> board(NUM_ROW, vector<int>(NUM_COL));

//******************************************************************************
// Function that lets the user input the difficulty level that being the depth
// and the 2 heuristic functions and runs it through a loop. Also sends error
// message if number selected outside of available number choices.
//******************************************************************************
void playGame()
{
	while(true)
	{
		cout << "What Depth do you want? ";
		cin >> depth;
		if(depth != 2 & depth != 4 & depth != 8)
			errorMessage(1);
		else
			break;
	}

	while(true)
	{
		cout << "What Heuristic Function do you want to use for AI 1? ";
		cin >> heur1;

		if(heur1 > 3 || heur1 < 1)
			errorMessage(2);
		else
			break;
	}

	while(true)
	{
		cout << "What Heuristic Function do you want to use for AI 2? ";
		cin >> heur2;

		if(heur2 > 3 || heur2 < 1)
			errorMessage(2);
		else
			break;
	}

	printBoard(board); // Prints the initial board to the screen
	while(!gameOver)
    {
		if(currentPlayer == COMPUTER2)
		{
			makeMove(board, aiMove(), COMPUTER2);
		}
		else if(currentPlayer == COMPUTER1)
		{
			makeMove(board, aiMove(), COMPUTER1);
		}
		else if (turns == NUM_ROW * NUM_COL) { // if max number of turns reached
			gameOver = true;
		}
		gameOver = winningMove(board, currentPlayer);

		if(currentPlayer == 1)
			cout << "AI 1's TURN" << endl;
		if(currentPlayer == 2)
			cout << "AI 2's TURN" << endl;
		currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch players
		turns++;
		cout << endl;
		printBoard(board); // Print board after successful move
	}
	if(turns == NUM_ROW * NUM_COL) // Determines if the game is a draw
	{
		cout << "Draw!" << endl;
	}
	else // Indicates if someone wins the game
	{
		cout << ((currentPlayer == COMPUTER1) ? "AI 1 Wins!" : "AI 2 Wins!")
		<< endl;
	}
	system("PAUSE");
}

//******************************************************************************
// Function that makes the moves for each player.
//******************************************************************************
void makeMove(vector<vector<int> >& b, int c, unsigned int p)
{
	// start from bottom of board going up
	for(unsigned int r = 0; r < NUM_ROW; r++)
    {
		if(b[r][c] == 0)
		{
			b[r][c] = p;
			break;
		}
	}
}

//******************************************************************************
// Function that utilizes the minimax algorithm to find the ideal move.
//******************************************************************************
int aiMove()
{
	return miniMax(board, depth, 0 - INT_MAX, INT_MAX, currentPlayer)[1];
}

//******************************************************************************
// This function is the minimax implementation that uses the alpha-beta pruning.
//******************************************************************************
array<int, 2> miniMax(vector<vector<int> > &b, unsigned int d, int alf, int bet,
                       unsigned int p)
{
	if(d == 0 || d >= (NUM_COL * NUM_ROW) - turns)
    {
		// Gets current score to return
		return array<int, 2>{tabScore(b, COMPUTER2), -1};
	}
	if(p == COMPUTER2)
	{
		array<int, 2> moveSoFar = {INT_MIN, -1}; // Lowest possible value
		if (winningMove(b, COMPUTER1))
		{
			return moveSoFar;
		}
		for(unsigned int c = 0; c < NUM_COL; c++)
		{
			if(b[NUM_ROW - 1][c] == 0)
			{
				vector<vector<int> > newBoard = copyBoard(b);
				makeMove(newBoard, c, p);
				int score = miniMax(newBoard, d - 1, alf, bet, COMPUTER1)[0];
				if (score > moveSoFar[0]) // Replaces current score if that one
                                          // is better
				{
					moveSoFar = {score, (int)c};
				}
				alf = max(alf, moveSoFar[0]);
				if(alf >= bet) { break; } // Pruning capability
			}
		}
		return moveSoFar; // Returns best possible move
	}
	else
	{
		array<int, 2> moveSoFar = {INT_MAX, -1};
		if(winningMove(b, COMPUTER2))
		{
			return moveSoFar; // Returns better move
		}
		for(unsigned int c = 0; c < NUM_COL; c++)
		{
			if(b[NUM_ROW - 1][c] == 0)
			{
				vector<vector<int> > newBoard = copyBoard(b);
				makeMove(newBoard, c, p);
				int score = miniMax(newBoard, d - 1, alf, bet, COMPUTER2)[0];
				if(score < moveSoFar[0])
				{
					moveSoFar = {score, (int)c};
				}
				bet = min(bet, moveSoFar[0]);
				if(alf >= bet) { break; }
			}
		}
		return moveSoFar;
	}
}

//******************************************************************************
// Function that calculates the current board value.
//******************************************************************************
int tabScore(vector<vector<int> > b, unsigned int p)
{
	int score = 0;
	vector<unsigned int> rs(NUM_COL);
	vector<unsigned int> cs(NUM_ROW);
	vector<unsigned int> set(4);

	for(unsigned int r = 0; r < NUM_ROW; r++)
    {
		for (unsigned int c = 0; c < NUM_COL; c++)
		{
			rs[c] = b[r][c];
		}
		for(unsigned int c = 0; c < NUM_COL - 3; c++)
		{
			for(int i = 0; i < 4; i++)
			{
				set[i] = rs[c + i];
			}
			score += scoreSet(set, p); // Finds current score
		}
	}

	for(unsigned int c = 0; c < NUM_COL; c++)
    {
		for(unsigned int r = 0; r < NUM_ROW; r++)
		{
			cs[r] = b[r][c];
		}
		for(unsigned int r = 0; r < NUM_ROW - 3; r++)
		{
			for(int i = 0; i < 4; i++)
			{
				set[i] = cs[r + i];
			}
			score += scoreSet(set, p);
		}
	}
	for(unsigned int r = 0; r < NUM_ROW - 3; r++)
	{
		for(unsigned int c = 0; c < NUM_COL; c++)
		{
			rs[c] = b[r][c];
		}
		for(unsigned int c = 0; c < NUM_COL - 3; c++)
		{
			for(int i = 0; i < 4; i++)
			{
				set[i] = b[r + i][c + i];
			}
			score += scoreSet(set, p);
		}
	}
	for(unsigned int r = 0; r < NUM_ROW - 3; r++)
	{
		for(unsigned int c = 0; c < NUM_COL; c++)
		{
			rs[c] = b[r][c];
		}
		for(unsigned int c = 0; c < NUM_COL - 3; c++)
		{
			for(int i = 0; i < 4; i++)
			{
				set[i] = b[r + 3 - i][c + i];
			}
			score += scoreSet(set, p);
		}
	}
	return score; // Returns the current score
}

//******************************************************************************
// Function that finds the score for a set of four spots in a row.
//******************************************************************************
int scoreSet(vector<unsigned int> v, unsigned int p)
{
	unsigned int good = 0; // Points in favor of p
	unsigned int bad = 0; // Points against p
	unsigned int empty = 0; // Neutral
	for(unsigned int i = 0; i < v.size(); i++)
    {
		good += (v[i] == p) ? 1 : 0;
		bad += (v[i] == COMPUTER1 || v[i] == COMPUTER2) ? 1 : 0;
		empty += (v[i] == 0) ? 1 : 0;
	}
	bad -= good;

	if(currentPlayer == COMPUTER1)
	{
		if(heur1 == 1)
			return heurFunction(good, bad, empty);
		else if(heur1 == 2)
			return heurFunction2(good, bad, empty);
		else if(heur1 == 3)
			return heurFunction3(good, bad, empty);
	}

	if(currentPlayer == COMPUTER2)
	{
		if(heur2 == 1)
			return heurFunction(good, bad, empty);
		else if(heur2 == 2)
			return heurFunction2(good, bad, empty);
		else if(heur2 == 3)
			return heurFunction3(good, bad, empty);
	}

}

//******************************************************************************
// These are the 3 minimax evaluation functions with the 3 designated depths.
//******************************************************************************
int heurFunction(unsigned int g, unsigned int b, unsigned int z)
{
	int score = 0;
	if(g == 2) { score += 500001; }
	else if(g == 3 && z == 1) { score += 5000; }
	else if(g == 2 && z == 2) { score += 500; }
	else if(b == 2 && z == 2) { score -= 501; }
	else if(b == 3 && z == 1) { score -= 5001; }
	return score;
}

int heurFunction2(unsigned int g, unsigned int b, unsigned int z)
{
	int score = 0;
	if(g == 4) { score += 500001; }
	else if(g == 3 && z == 1) { score += 5000; }
	else if(g == 2 && z == 2) { score += 500; }
	else if(b == 2 && z == 2) { score -= 501; }
	else if(b == 3 && z == 1) { score -= 5001; }
	return score;
}

int heurFunction3(unsigned int g, unsigned int b, unsigned int z)
{
	int score = 0;
	if(g == 8) { score += 500001; }
	else if(g == 3 && z == 1) { score += 5000; }
	else if(g == 2 && z == 2) { score += 500; }
	else if(b == 2 && z == 2) { score -= 501; }
	else if(b == 3 && z == 1) { score -= 5001; }
	return score;
}

//******************************************************************************
// Function that determines if there is a winning move that has been made.
//******************************************************************************
bool winningMove(vector<vector<int> > &b, unsigned int p)
{
	// Counts adjacent pieces for horizontal check
	unsigned int winSequence = 0;

	for(unsigned int c = 0; c < NUM_COL - 3; c++)
    {
		for(unsigned int r = 0; r < NUM_ROW; r++)
		{
			for(int i = 0; i < 4; i++) // To win 4 in a row
			{
				if((unsigned int)b[r][c + i] == p)
				{
					winSequence++;
				}
				if(winSequence == 4) {return true;} // if the winner gets 4
                                                       // in a row
			}
			winSequence = 0;
		}
	}
	// Counts adjacent pieces for vertical check
	for(unsigned int c = 0; c < NUM_COL; c++)
	{
		for(unsigned int r = 0; r < NUM_ROW - 3; r++)
		{
			for(int i = 0; i < 4; i++)
			{
				if((unsigned int)b[r + i][c] == p)
				{
					winSequence++;
				}
				if(winSequence == 4) {return true;}
			}
			winSequence = 0;
		}
	}
	// Loop for the diagonal check
	for(unsigned int c = 0; c < NUM_COL - 3; c++)
	{
		for(unsigned int r = 3; r < NUM_ROW; r++)
		{
			for(int i = 0; i < 4; i++)
			{
				if((unsigned int)b[r - i][c + i] == p)
				{
					winSequence++;
				}
				if(winSequence == 4) {return true;}
			}
			winSequence = 0;
		}
	}

	for(unsigned int c = 0; c < NUM_COL - 3; c++)
    {
		for(unsigned int r = 0; r < NUM_ROW - 3; r++)
		{
			for(int i = 0; i < 4; i++)
			{
				if((unsigned int)b[r + i][c + i] == p)
				{
					winSequence++;
				}
				if(winSequence == 4) {return true;}
			}
			winSequence = 0;
		}
	}
	return false;
}

//******************************************************************************
// Function that resets the board so it is filled with empty spaces. Resets
// board back to this when a winner has been established.
//******************************************************************************
void initialBoard()
{
	for(unsigned int r = 0; r < NUM_ROW; r++)
    {
		for(unsigned int c = 0; c < NUM_COL; c++)
		{
			board[r][c] = 0;
		}
	}
}

//******************************************************************************
// Function that copies the board state using a vector.
//******************************************************************************
vector<vector<int> > copyBoard(vector<vector<int> > b)
{
	vector<vector<int>> newBoard(NUM_ROW, vector<int>(NUM_COL));

	for(unsigned int r = 0; r < NUM_ROW; r++)
	{
		for(unsigned int c = 0; c < NUM_COL; c++)
        {
			newBoard[r][c] = b[r][c];
		}
	}
	return newBoard;
}
//******************************************************************************
// Function that prints the board, X, and O to the console.
//******************************************************************************
void printBoard(vector<vector<int> > &b)
{
	for(unsigned int i = 0; i < NUM_COL; i++)
    {
		cout << " " << i;
	}
	cout << endl << "---------------" << endl;
	for(unsigned int r = 0; r < NUM_ROW; r++)
	{
		for(unsigned int c = 0; c < NUM_COL; c++)
		{
			cout << "|";

			switch (b[NUM_ROW - r - 1][c])
			{
                case 0: cout << " "; break;
                case 1: cout << "O"; break;
                case 2: cout << "X"; break;
			}
			if(c + 1 == NUM_COL) {cout << "|";}
		}
		cout << endl;
	}
	cout << "---------------" << endl;
	cout << endl;
}

//******************************************************************************
// Function that displays the error messages when necessary.
//******************************************************************************
void errorMessage(int e)
{
	if(e == 1)
    {
		cout << "Enter a correct depth value (2, 4, or 8)." << endl;
	}
	else if(e == 2)
	{
		cout << "Enter a value between 1 and 3." << endl;
	}
	cout << endl;
}
//******************************************************************************
// Main driver function.
//******************************************************************************
int main(int argc, char** argv)
{
	int i = -1; bool flag = false;

	if(argc == 2)
    {
		istringstream in(argv[1]);
		if(!(in >> i)) {flag = true;}
		if(i > (int)(NUM_ROW * NUM_COL) || i <= -1) {flag = true;}
		if(flag)
		{
		    cout << "Invalid command line argument, using default depth = 2."
            << endl;
        }
		else {depth = i;}
	}
	initialBoard();
	playGame();

	return 0;
}
