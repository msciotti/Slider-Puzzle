#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <PCDraw.h>
#include <PCText.h>
#include <PCMouse.h>
#include <PCRect.h>
using namespace std;

vector<char> ClosedList;
vector<int> OpenList;

class Board{
private:
	struct Square{
		Square(){};
		int current_x, current_y, dest_x, dest_y;
		int value;
		int Manhattan_Dist;
		Square(int a, int b, int c, int d, int e){ current_x = a; current_y = b; dest_x = c; dest_y = d; Manhattan_Dist = abs(c - a) + abs(d - b); value = e; }
	};
	vector < vector<rectangle>> grects;
	int size;
	vector<vector<Square>> GameBoard;
	vector<vector<Square>> SolvedBoard;
	int promising(Square temp, char direction){
		int retVal;
		int a, b, c, q, e;
		a = temp.current_x; b = temp.current_y; c = temp.dest_x; q = temp.dest_y; e = temp.value;

		switch (direction){
		case 'u':{
			Square change(a - 1, b, c, q, e);
			if (change.Manhattan_Dist < temp.Manhattan_Dist)
				retVal = abs(change.Manhattan_Dist - temp.Manhattan_Dist);
			else retVal = NULL;
			break;
		}

		case 'd':{
			Square change(a + 1, b, c, q, e);
			if (change.Manhattan_Dist < temp.Manhattan_Dist)
				retVal = abs(change.Manhattan_Dist - temp.Manhattan_Dist);
			else retVal = NULL;
			break;
		}

		case 'l':{
			Square change(a, b - 1, c, q, e);
			if (change.Manhattan_Dist < temp.Manhattan_Dist)
				retVal = abs(change.Manhattan_Dist - temp.Manhattan_Dist);
			else retVal = NULL;
			break;
		}

		case 'r':{
			Square change(a, b + 1, c, q, e);
			if (change.Manhattan_Dist < temp.Manhattan_Dist)
				retVal = abs(change.Manhattan_Dist - temp.Manhattan_Dist);
			else retVal = NULL;
			break;
		}
		}
		return retVal;
	}

public:
	Board(){};
	Board(int n){ 
		size = n;
		GameBoard.resize(size);
		SolvedBoard.resize(size);
		for (int i = 0; i < size; i++){
			GameBoard[i].resize(size);
			SolvedBoard[i].resize(size);
		}
	}	//THIS WORKS DONT TOUCH
	bool isSolved(){
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				if (GameBoard[i][j].dest_x != size-1 && GameBoard[i][j].dest_y != size-1)
					if (GameBoard[i][j].current_x != GameBoard[i][j].dest_x || GameBoard[i][j].current_y != GameBoard[i][j].dest_y)
						return false;
		return true;
	}

	void swapTiles(Square &A, Square &B){
		Square temp = A;
		A = B;
		B = temp;
	}

	char chooseMove(){
		Square temp;
		char u, d, l, r;
		char choice = 'u';
		int w=0, x=0, y=0, z=0, row, column;


		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				if ((GameBoard[i][j].dest_x == size - 1) && (GameBoard[i][j].dest_y == size - 1)){
					temp = GameBoard[i][j];
					row = i;
					column = j;
				}

		int a = temp.current_x, b = temp.current_y;

		if (a - 1 >= 0){
			x = promising(GameBoard[row - 1][column], 'd');
		}

		if (a + 1 < size){
			w = promising(GameBoard[row + 1][column], 'u');
		}

		if (b - 1 >= 0){
			z = promising(GameBoard[row][column - 1], 'r');
		}


		if (b + 1 < size){
			y = promising(GameBoard[row][column + 1], 'l');

		}


		if (w != 0)
			slideTile('u');

		if (x != 0)
			slideTile('d');

		if (y != 0){
			slideTile('l');
			cout << endl << endl << "Slid Y\n";
		}

		if (z != 0)
			slideTile('r');


		for (int g = 0; g < size; g++){
			cout << endl;
			for (int h = 0; h < size; h++)
				cout << GameBoard[g][h].value << " ";
		}
		cout << endl;

		return 'u';
	}

	void slideTile(char move){
		int emptyRow;
		int emptyCol;
		bool legalMoves[4] = { 1, 1, 1, 1 }; //array of legal moves, [0] = left, [1] = right, [2] = up, [3] = down. true (1) indicates a legal move.

		for (unsigned int row = 0; row < size; row++)
			for (unsigned int column = 0; column < size; column++)
				if ((GameBoard[row][column].dest_x == size-1) && (GameBoard[row][column].dest_y == size-1)){
					emptyRow = row;
					emptyCol = column;
				}

		if (emptyRow + 1 > size) //Can i move up?
			legalMoves[2] = false; //If no, set move flag to false

		else if (emptyRow - 1 < 0) //Move down?
			legalMoves[3] = false;

		if (emptyCol - 1 < 0) //Move right?
			legalMoves[1] = false;

		else if (emptyCol + 1 > size) //Move left?
			legalMoves[0] = false;

		switch (move) //Replace zero space with space to the left right up or down.
		{
		case 'u':
			if (legalMoves[2])
			{
				swapTiles(GameBoard[emptyRow][emptyCol], GameBoard[emptyRow+1][emptyCol]);
			}
			break;
		case 'd':
			if (legalMoves[3])
			{
				swapTiles(GameBoard[emptyRow][emptyCol], GameBoard[emptyRow-1][emptyCol]);
			}
			break;
		case 'l':
			if (legalMoves[0])
			{
				swapTiles(GameBoard[emptyRow][emptyCol], GameBoard[emptyRow][emptyCol+1]);
			}
			break;
		case 'r':
			if (legalMoves[1])
			{
				swapTiles(GameBoard[emptyRow][emptyCol], GameBoard[emptyRow][emptyCol-1]);
			}
			break;
		}

	}

	void makeSquare(vector<vector<int>> values){

		for (unsigned int i = 0; i < values.size(); i++){
			for (unsigned int j = 0; j < values.size(); j++){

				if (values[i][j] == 0){
					Square newSquare(i, j, size - 1, size - 1, values[i][j]);
					GameBoard[i][j] = newSquare;
				}

				else{
					int row, column;

					if (values[i][j] % size == 0){
						row = values[i][j] / size - 1;
						column = size - 1;
					}

					else{
						row = values[i][j] / size;
						column = (values[i][j] % size) - 1;
					}

					Square newSquare(i, j, row, column,values[i][j]);
					GameBoard[i][j] = newSquare;
					int x = 0;
							}
						}
					}
				}


	void getBoard(){
		for (int i = 0; i < size; i++){
			cout << endl;
			for (int j = 0; j < size; j++)
				cout << GameBoard[i][j].value << " ";
		}
	}

	void drawHorizontal(int xstart, int xend, int y){
		DrawLine(xstart, y - 1, xend, y - 1);
		DrawLine(xstart, y, xend, y);
		DrawLine(xstart, y + 1, xend, y + 1);
	}
	void drawVertical(int x, int ystart, int yend){
		DrawLine(x - 1, ystart, x - 1, yend);
		DrawLine(x, ystart, x, yend);
		DrawLine(x + 1, ystart, x + 1, yend);
	}
	void drawGrid(int n){
		int w = 600, h = 600;
		int r, c, sz = 50, b = 50, bx, by;

		InitGraphics("Upper left", w, h);
		GetGraphicsWidth(); GetGraphicsHeight();
		SetGraphicsTitle("Brigid and Mason Slider Puzzle");
		SetPenColor("Black");
		for (r = 0; r <= n; r++){
			drawHorizontal(b, b + sz*n, b + r*sz);
		}
		for (c = 0; c <= n; c++)
			drawVertical(b + sz*c, b, b + sz*n);

		bx = (w - n*sz) / 2;  // border in the horizontal direction
		by = (h - n*sz) / 2;  // border in the vertical direction
		grects = vector<vector<rectangle>>(n, vector<rectangle>(n));
		for (r = 0; r < n; r++){
			for (c = 0; c < n; c++){
				grects[r][c].init(point(b + sz*c, h - (h + 1) + (b + r*sz - 1)), sz, sz);
			}
		}
		return;

	}	// THIS WORKS DO NOT TOUCH

	inline int max(int a, int b){ return (a >= b) ? a : b; }
	bool drawLabels(int n){
		static bool firstTime = true;
		bool retval = true;
		PCFont font1, fontLarger;
		int sqrlab, r, c, x, y, sz;

		double osr;
		static double offsetRatio = 0.4;
		char buffer[32];
		int pointsize, ps[] = { 60, 60, 60, 55, 50, 45, 40, 35, 30, 25, 20 };
		if (n > 10)
			pointsize = 15;
		else
			pointsize = ps[max(n, n)];
		sz = grects[0][0].get_width();

		osr = offsetRatio;
		for (r = 0; r < n; r++){
			for (c = 0; c < n; c++){
				x = (int)(grects[r][c].get_corner().get_x());
				y = (int)(grects[r][c].get_corner().get_y());
				sqrlab = GameBoard[r][c].value;
				if (sqrlab != 0){
					sprintf_s(buffer, 32, "%d", sqrlab);
					DrawTextString(x + osr*sz, y + osr*sz, buffer);
				}
			}
		}
		return true;
	}
};

	int main(){
		int size, num = 0, choice;
		vector<vector<int>> values;
		srand(time(NULL));

		cout << "Enter 1 for input from a file, or 2 for randomly generated puzzle: ";
		cin >> choice;

		if (choice == 1){
			string fname;
			cout << "Enter file name: ";
			cin >> fname;
			ifstream fin;
			fin.open(fname);
			fin >> size >> size;
			Board GameBoard(size);

			int input;
			vector<int> othernum;
			for (int i = 0; i < size; i++){
				values.push_back(othernum);
				for (int j = 0; j < size; j++){
					fin >> input;
					values[i].push_back(input);
					num++;
				}
			}
			GameBoard.makeSquare(values);
			GameBoard.drawGrid(size);

			while (!GameBoard.isSolved()){
				GameBoard.drawLabels(size);
				ClosedList.push_back(GameBoard.chooseMove());
			}
		}

		else if (choice == 2){
			cout << "Enter puzzle size: ";
			cin >> size;
			Board GameBoard(size);

			vector<int> tempvalues;
			int tempnum = 0;
			for (unsigned i = 0; i < size*size; i++){
				tempvalues.push_back(tempnum);
				tempnum++;
			}
			random_shuffle(tempvalues.begin(), tempvalues.end());

			vector<int> othernum;
			for (int i = 0; i < size; i++){
				values.push_back(othernum);
				for (int j = 0; j < size; j++){
					values[i].push_back(tempvalues[0]);
					tempvalues.erase(tempvalues.begin());
					num++;
				}
			}

			random_shuffle(values.begin(), values.end());
			GameBoard.makeSquare(values);
			GameBoard.getBoard();
			GameBoard.drawGrid(size);

			while (!GameBoard.isSolved()){
				GameBoard.drawLabels(size);
				ClosedList.push_back(GameBoard.chooseMove());
			}

		}

		else{
			cout << "That is not a valid choice. Goodbye.\n";
		}



		return 0;
	}
