#include<iostream>
#include<algorithm>
#include<Windows.h>
using namespace std;
#define INF 10
const char nl = ' ';

void Goto(pair<int, int>P) {
	COORD p = { P.second, P.first };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

char board[3][3];

pair<int, int> nextMove;

//draw board
void draw() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Goto({ i * 2, j * 2 });
			cout << board[i][j] << "|";
		}
		cout << "\n------";
	}
}
//

//'X' for CPU .. 'O' for User .. 'T' for Tie .. nl for Null
char checkWinner() {
	int empty = 0;
	for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) if (board[i][j] == nl) empty++;
	if (
		(board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[0][0] == 'X') ||
		(board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[0][1] == 'X') ||
		(board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[0][2] == 'X') ||
		(board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][0] == 'X') ||
		(board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][0] == 'X') ||
		(board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][0] == 'X') ||
		(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] == 'X') ||
		(board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[2][0] == 'X')
		) return 'X';
	else if (
		(board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[0][0] == 'O') ||
		(board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[0][1] == 'O') ||
		(board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[0][2] == 'O') ||
		(board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][0] == 'O') ||
		(board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][0] == 'O') ||
		(board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][0] == 'O') ||
		(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] == 'O') ||
		(board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[2][0] == 'O')
		)return 'O';
	else if (!empty) return 'T';
	return nl;
}

//CPU for turn = 1 .. User for turn = 0
int MiniMax(int depth, bool turn) {
	char Res = checkWinner();
	if (Res == 'O') return -1;
	else if (Res == 'X') return 1;
	else if (Res == 'T') return 0;
	if (!depth) return 0;

	//Initializing bestScore
	int bestScore;
	if (turn) bestScore = -1;
	else bestScore = 1;
	//
	bool tie = true;
	for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) {
		if (board[i][j] == nl) {
			if (tie&&depth == 9) nextMove = { i,j };
			tie = false;

			//Take a move
			if (turn) board[i][j] = 'X';
			else board[i][j] = 'O';
			//

			//Deeping
			int deep = MiniMax(depth - 1, !turn); 
			//

			//Maximize
			if (turn && bestScore < deep) {  
				bestScore = deep;
				if (depth == 9) nextMove = { i,j };
			}
			//

			//Minimize
			if (!turn && bestScore > deep) { 
				bestScore = deep;
			}
			//

			//Release taken move
			board[i][j] = nl;
			//
		}
	}
	return (tie) ? 0 : bestScore;
}


int main() {
	//Initializing board
	for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) board[i][j] = nl;
	draw();
	//

	while (checkWinner() == nl) {
		//flag means if User made his move or not
		bool user = 0;
		while (!user) {
			if (GetAsyncKeyState(VK_NUMPAD1) && board[2][0] == nl) {
				board[2][0] = 'O';
				user = 1;
			}
			else if (GetAsyncKeyState(VK_NUMPAD2) && board[2][1] == nl) {
				user = 1;
				board[2][1] = 'O';
			}
			else if (GetAsyncKeyState(VK_NUMPAD3) && board[2][2] == nl) {
				user = 1; board[2][2] = 'O';
			}
			else if (GetAsyncKeyState(VK_NUMPAD4) && board[1][0] == nl) {
				user = 1; board[1][0] = 'O';
			}
			else if (GetAsyncKeyState(VK_NUMPAD5) && board[1][1] == nl) {
				user = 1; board[1][1] = 'O';
			}
			else if (GetAsyncKeyState(VK_NUMPAD6) && board[1][2] == nl) {
				user = 1; board[1][2] = 'O';
			}
			else if (GetAsyncKeyState(VK_NUMPAD7) && board[0][0] == nl) {
				user = 1; board[0][0] = 'O';
			}
			else if (GetAsyncKeyState(VK_NUMPAD8) && board[0][1] == nl) {
				user = 1; board[0][1] = 'O';
			}
			else if (GetAsyncKeyState(VK_NUMPAD9) && board[0][2] == nl) {
				user = 1; board[0][2] = 'O';
			}
		}

		//Update console
		draw();
		//

		if (checkWinner() != nl) break;

		//Move generation for CPU
		MiniMax(9, 1);
		Goto(nextMove);
		board[nextMove.first][nextMove.second] = 'X';
		//

		//Update console
		draw();
		//
	}

	//Happy Ending...
	Goto({ 6,0 });
	if (checkWinner() == 'X') cout << "CPU wins!\n";
	else if (checkWinner() == 'O') cout << "You win!\n";
	else cout << "Tie!\n";
	//
}