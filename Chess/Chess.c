#include <stdio.h>
#include <math.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#define SIZE 8
#define MAX_MOVES 1000
#define INPUT_LENGTH 10
#define INPUT_LENGTH_STR "10"
#define ONE_SECOND 1000000

#define C_WHITE   "\x1b[37m"
#define C_GRAY    "\x1b[30m"
#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_RESET   "\x1b[0;0m"
#define C_BOLD "\x1b[1m"

#define XY(a, b) int a x b, int a y b

typedef int Piece;
typedef int Player;
typedef Piece Board[SIZE][SIZE];

typedef char MoveText[7];

typedef struct
{
	int srcX, srcY;
	int destX, destY;
	Piece pieceTaken;
} 
Move;

Move *move_new(int srcX, int srcY, int destX, int destY)
{
	Move *m = (Move *)malloc(sizeof(Move));
	m->srcX = srcX;
	m->srcY = srcY;
	m->destX = destX;
	m->destY = destY;
	return m;
}

const Board START_POSITION = {
{ 4 , 2 , 3 , 5 , 6 , 3 , 2 , 4 },
{ 1,  1,  1,  1,  1,  1,  1,  1 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{-1 ,-1, -1, -1, -1, -1, -1, -1},
{-4, -2, -3, -5, -6, -3, -2, -4} };

const Board TEST_POSITION = {
{ 0 , 0 , 0 , 0 , 0 , 0 , 1 ,-6 },
{ 0,  0,  0,  0,  0,  6, -1,  0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 4 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0,  4,  0,  0,  0,  0,  0,  0 },
{ 0,  0,  0,  0,  0,  0,  0,  0 } };

const double PAWN_EVALUATION[SIZE][SIZE] = {
{0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
{1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00},
{1.01, 1.02, 1.05, 1.10, 1.10, 1.05, 1.02, 1.01},
{1.03, 1.07, 1.10, 1.16, 1.16, 1.10, 1.07, 1.03},
{1.08, 1.12, 1.17, 1.25, 1.25, 1.17, 1.12, 1.08},
{1.15, 1.20, 1.25, 1.35, 1.35, 1.25, 1.20, 1.15},
{1.30, 1.35, 1.40, 1.45, 1.45, 1.40, 1.35, 1.30},
{0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00} };

const double KNIGHT_EVALUATION[SIZE][SIZE] = {
{1.50, 2.00, 2.25, 2.25, 2.25, 2.25, 2.00, 1.50},
{2.00, 2.25, 2.50, 2.50, 2.50, 2.50, 2.25, 2.00},
{2.25, 2.50, 3.00, 3.00, 3.00, 3.00, 2.50, 2.25},
{2.25, 2.50, 3.25, 3.00, 3.00, 3.25, 2.50, 2.25},
{2.25, 2.50, 4.00, 3.50, 3.50, 4.00, 2.50, 2.25},
{2.25, 2.50, 4.50, 4.00, 4.00, 4.50, 2.50, 2.25},
{2.00, 2.25, 2.50, 2.50, 2.50, 2.50, 2.25, 2.00},
{1.50, 2.00, 2.25, 2.25, 2.25, 2.25, 2.00, 1.50} };

const double BISHOP_EVALUATION[SIZE][SIZE] = {
{1.25, 1.50, 1.75, 2.00, 2.00, 1.75, 1.50, 1.25},
{1.50, 3.00, 2.00, 2.50, 2.50, 2.00, 3.00, 1.50},
{1.75, 2.50, 3.50, 3.00, 3.00, 3.50, 2.50, 1.75},
{2.00, 2.25, 3.00, 4.00, 4.00, 3.00, 2.50, 2.00},
{2.00, 2.25, 3.00, 4.00, 4.00, 3.00, 2.50, 2.00},
{1.75, 2.25, 3.50, 3.00, 3.00, 3.50, 2.25, 1.75},
{1.50, 2.25, 2.50, 2.50, 2.50, 2.50, 2.25, 1.50},
{1.25, 1.50, 1.75, 2.00, 2.00, 1.75, 1.50, 1.25} };

const int LEGAL_MOVES_KNIGHT_X[8] =
{ 2, 1, -1, -2, -2, -1, 1, 2 };
const int
LEGAL_MOVES_KNIGHT_Y[8] =
{ 1, 2, 2, 1, -1, -2, -2, -1 };

const int LEGAL_MOVES_KING_X[8] =
{ 0, 1, 1, 1, 0, -1, -1, -1 };
const int
LEGAL_MOVES_KING_Y[8] =
{ 1, 1, 0, -1, -1, -1, 0, 1 };

const int
LEGAL_VECTORS_BISHOP_X[4] =
{ 1, 1, -1, -1 };
const int
LEGAL_VECTORS_BISHOP_Y[4] =
{ 1, -1, -1, 1 };

const int
LEGAL_VECTORS_ROOK_X[4] =
{ 0, 1, 0, -1 };
const int
LEGAL_VECTORS_ROOK_Y[4] =
{ 1, 0, -1, 0 };

Board displayBoard = { 0 };
Board testBoard = { 0 };

int playerTurn = 1;
int currentMove = 0;
MoveText moves[MAX_MOVES];

int bKingMoved, wKingMoved;
double evaluation = 0;

int selectedX = -1, selectedY = -1;
int whiteComputer = 0, blackComputer = 0;

void DrawBoard();
char *PlayerToName(Player player);
int ReadChessNotationPosition(char *position, int *x, int *y);
int CanPieceMoveHere(Board board, int x1, int y1, int x2, int y2);
void DeselectCell();
void Evaluate();
int TryMove(Board board, Move *move);

int sign(int value)
{
	return (value > 0) ? 1 : (value < 0) ? -1 : 0;
}

void PrintMoveTrace()
{
	if (currentMove > 0) printf("Moves: ");
	else printf("No moves played yet.");
	for (int i = 0; i < currentMove; i++)
	{
		char *move = moves[i];
		printf("%s", move);
		if (i + 1 < currentMove) printf(", ");
	}
	printf("\n");
}

void Help()
{
	printf(C_CYAN "/help" C_RESET " - this command;\n");
	printf(C_CYAN "/newgame" C_RESET " - resets chess board and starts new game;\n");
	printf(C_CYAN "/trace" C_RESET " - prints all moves played during current game;\n");
	printf(C_CYAN "/stats" C_RESET " - prints some info about current game;\n");
	printf(C_CYAN "/redraw" C_RESET " - redraws current position;\n");
	printf(C_CYAN "/select" C_RESET " - choose tile to highlight and display possible moves from it.\n");
	printf(C_CYAN "/deselect" C_RESET " - clean selection.\n");
}

void NewGame()
{
	printf(C_YELLOW "Starting new game...\n" C_RESET);
	playerTurn = 1;
	currentMove = 0;
	for (int n = 0; n < SIZE*SIZE; n++)
	{
		*(*displayBoard + n) = *(*START_POSITION + n);
	}
	DeselectCell();
	DrawBoard();
}

void PrintGameStats()
{
	printf("----------Stats----------\n");
	printf("Moves played: %d\n", currentMove);
	PrintMoveTrace();
	printf("Player turn: %s\n", PlayerToName(playerTurn));
	printf("Evaluation: %.2lf\n", evaluation);
	printf("-------------------------\n");
}

void SelectCell()
{
	char input[3];
	int x, y;
	int parsed = 0;

	do
	{
		printf("Enter position to select: \n");
		scanf("%2s", input);
		parsed = ReadChessNotationPosition(input, &x, &y);
		if (!parsed) printf("Invalid position! Try again. ");
	} while (!parsed);
	selectedX = x;
	selectedY = y;
}

void DeselectCell()
{
	selectedX = selectedY = -1;
}

void NextMove()
{
	Evaluate();
	currentMove++;
	playerTurn *= -1;
	DrawBoard();
}

void ChangeComputer(Player player)
{
	switch (player)
	{
	case -1:
		blackComputer = !blackComputer;
		break;
	case 1:
		whiteComputer = !whiteComputer;
	}
}

void MoveToText(Move *move, MoveText text) 
{
	char x1 = 'a' + move->srcX;
	char y1 = '1' + move->srcY;
	char x2 = 'a' + move->destX;
	char y2 = '1' + move->destY;

	text[0] = x1;
	text[1] = y1;
	text[2] = x2;
	text[3] = y2;
}

char *PieceToHighlightedChar(Piece piece) {
	switch (piece)
	{
	case -1: return C_GREEN C_BOLD "P" C_RESET;
	case -2: return C_GREEN C_BOLD "N" C_RESET;
	case -3: return C_GREEN C_BOLD "B" C_RESET;
	case -4: return C_GREEN C_BOLD "R" C_RESET;
	case -5: return C_GREEN C_BOLD "Q" C_RESET;
	case -6: return C_GREEN C_BOLD "K" C_RESET;
	case 0: return C_GREEN "+" C_RESET;
	case 1: return C_GREEN C_BOLD "P" C_RESET;
	case 2: return C_GREEN C_BOLD "N" C_RESET;
	case 3: return C_GREEN C_BOLD "B" C_RESET;
	case 4: return C_GREEN C_BOLD "R" C_RESET;
	case 5: return C_GREEN C_BOLD "Q" C_RESET;
	case 6: return C_GREEN C_BOLD "K" C_RESET;
	default: return "?";
	}
}

char *PieceToColoredChar(Piece piece)
{
	switch (piece)
	{
	case -1: return C_GRAY C_BOLD "P" C_RESET;
	case -2: return C_GRAY C_BOLD "N" C_RESET;
	case -3: return C_GRAY C_BOLD "B" C_RESET;
	case -4: return C_GRAY C_BOLD "R" C_RESET;
	case -5: return C_GRAY C_BOLD "Q" C_RESET;
	case -6: return C_GRAY C_BOLD "K" C_RESET;
	case 0: return "-";
	case 1: return C_BOLD "P" C_RESET;
	case 2: return C_BOLD "N" C_RESET;
	case 3: return C_BOLD "B" C_RESET;
	case 4: return C_BOLD "R" C_RESET;
	case 5: return C_BOLD "Q" C_RESET;
	case 6: return C_BOLD "K" C_RESET;
	default: return "?";
	}
}

char *PieceToChar(Piece piece)
{
	switch (abs(piece))
	{
	case 0: return "-";
	case 1: return C_BOLD "P" C_RESET;
	case 2: return C_BOLD "N" C_RESET;
	case 3: return C_BOLD "B" C_RESET;
	case 4: return C_BOLD "R" C_RESET;
	case 5: return C_BOLD "Q" C_RESET;
	case 6: return C_BOLD "K" C_RESET;
	default: return "?";
	}
}

char *PlayerToName(Player player)
{
	switch (player)
	{
	case -1: return "Black";
	case 1: return "White";
	default: return "Unknown";
	}
}

char *PlayerToColoredName(Player player)
{
	switch (player)
	{
	case -1: return C_GRAY C_BOLD "Black" C_RESET;
	case 1: return C_WHITE C_BOLD "White" C_RESET;
	default: return "Unknown";
	}
}

int InBounds(int x, int y)
{
	return
		!(x < 0 || x > 7 || y < 0 || y > 7);
}

int GetCell(Board board, int x, int y)
{
	if (InBounds(x, y)) return board[y][x];

	return 0;
}

int GetPlayer(Board board, int x, int y)
{
	return sign(GetCell(board, x, y));
}

int GetPiece(Board board, int x, int y)
{
	return abs(GetCell(board, x, y));
}

int IsEmpty(Board board, int x, int y)
{
	return !GetCell(board, x, y);
}

int PieceValue(Piece cell)
{
	switch (cell)
	{
	case -1: return - 1;
	case -2: return -3;
	case -3: return -3;
	case -4: return -5;
	case -5: return -9;
	case -6: return -1000;

	case 1: return 1;
	case 2: return 3;
	case 3: return 3;
	case 4: return 5;
	case 5: return 9;
	case 6: return 1000;
	default: return 0;
	}
}

Move *TryComputerMove(Board board, Player player)
{
	Move *moves[SIZE * SIZE];
	int count = 0;
	for (int xp = 0; xp < SIZE; xp++)
	{
		for (int yp = 0; yp < SIZE; yp++)
		{
			if (GetPlayer(board, xp, yp) == player)
			{
				for (int x = 0; x < SIZE; x++)
				{
					for (int y = 0; y < SIZE; y++)
					{
						if (CanPieceMoveHere(board, xp, yp, x, y))
						{
							moves[count] = move_new(xp, yp, x, y);
							count++;
						}
					}
				}
			}
		}
	}
	if (count > 0) 
	{
		int moveIndex = rand() % count;
		Move *move = move_new(
			moves[moveIndex]->srcX, moves[moveIndex]->srcY, 
			moves[moveIndex]->destX, moves[moveIndex]->destY);
		TryMove(board, move);
		for (int i = 0; i < count; i++) 
		{
			free(moves[i]);
		}
		return move;
	}
	else
	{
		perror(C_YELLOW "No moves avaliable" C_RESET);
		return NULL;
	}
}

void SetCell(Board board, int x, int y, Piece value) 
{
	if (InBounds(x, y)) board[y][x] = value;
}

int IsSameLine(int x1, int y1, int x2, int y2)
{
	return (x1 == x2) || (y1 == y2);
}

int IsSameDiagonal(int x1, int y1, int x2, int y2)
{
	return (abs(x1 - x2) == abs(y1 - y2));
}

int IsEnemy(Board board, int x1, int y1, int x2, int y2)
{
	return (!IsEmpty(board, x1, y1) && !IsEmpty(board, x2, y2)) && (GetPlayer(board, x1, y1) != GetPlayer(board, x2, y2));
}

int IsAlly(Board board, int x1, int y1, int x2, int y2)
{
	return (!IsEmpty(board, x1, y1) && !IsEmpty(board, x2, y2)) && (GetPlayer(board, x1, y1) == GetPlayer(board, x2, y2));
}

int CanPieceMoveHere(Board board, int x1, int y1, int x2, int y2)
{
	int cell = GetCell(board, x1, y1);
	int piece = GetPiece(board, x1, y1);
	int player = GetPlayer(board, x1, y1);

	if (IsAlly(board, x1, y1, x2, y2)) return 0;

	switch (piece)
	{
	//pawn
	case 1:
		if (x1 == x2)
		{
			int pawnRank = (player == 1) ? 1 : 6;
			if ((y2 - y1 == 2 * player && y1 == pawnRank && IsEmpty(board, x1, y1 + player)) || y2 - y1 == player)
			{
				return IsEmpty(board, x1, y2);
			}
			return 0;
		}
		else if (abs(x1 - x2) == 1 && y1 + player == y2) return IsEnemy(board, x1, y1, x2, y2);
		break;

	//knight
	case 2:
		for (int i = 0; i < 8; i++)
		{
			if (x2 - x1 == LEGAL_MOVES_KNIGHT_X[i] && y2 - y1 == LEGAL_MOVES_KNIGHT_Y[i])
				return 1;
		}
		break;

	//bishop
	case 3:
		for (int i = 0; i < 4; i++)
		{
			int vx = x1 + LEGAL_VECTORS_BISHOP_X[i];
			int vy = y1 + LEGAL_VECTORS_BISHOP_Y[i];
			while (InBounds(vx, vy))
			{
				if (x2 == vx && y2 == vy)
				{
					return (IsEmpty(board, x2, y2) || IsEnemy(board, x1, y1, x2, y2));
				}
				else if (!IsEmpty(board, vx, vy)) break;
				vx += LEGAL_VECTORS_BISHOP_X[i];
				vy += LEGAL_VECTORS_BISHOP_Y[i];
			}
		}
		break;

	//rook
	case 4:
		for (int i = 0; i < 4; i++)
		{
			int vx = x1 + LEGAL_VECTORS_ROOK_X[i];
			int vy = y1 + LEGAL_VECTORS_ROOK_Y[i];
			while (InBounds(vx, vy))
			{
				if (x2 == vx && y2 == vy)
				{
					return (IsEmpty(board, x2, y2) || IsEnemy(board, x1, y1, x2, y2));
				}
				else if (!IsEmpty(board, vx, vy)) break;
				vx += LEGAL_VECTORS_ROOK_X[i];
				vy += LEGAL_VECTORS_ROOK_Y[i];
			}
		}
		break;

	//queen
	case 5:
		for (int i = 0; i < 4; i++)
		{
			int vx = x1 + LEGAL_VECTORS_BISHOP_X[i];
			int vy = y1 + LEGAL_VECTORS_BISHOP_Y[i];
			while (InBounds(vx, vy))
			{
				if (x2 == vx && y2 == vy)
				{
					return (IsEmpty(board, x2, y2) || IsEnemy(board, x1, y1, x2, y2));
				}
				else if (!IsEmpty(board, vx, vy)) break;
				vx += LEGAL_VECTORS_BISHOP_X[i];
				vy += LEGAL_VECTORS_BISHOP_Y[i];
			}
		}
		for (int i = 0; i < 4; i++)
		{
			int vx = x1 + LEGAL_VECTORS_ROOK_X[i];
			int vy = y1 + LEGAL_VECTORS_ROOK_Y[i];
			while (InBounds(vx, vy))
			{
				if (x2 == vx && y2 == vy)
				{
					return (IsEmpty(board, x2, y2) || IsEnemy(board, x1, y1, x2, y2));
				}
				else if (!IsEmpty(board, vx, vy)) break;
				vx += LEGAL_VECTORS_ROOK_X[i];
				vy += LEGAL_VECTORS_ROOK_Y[i];
			}
		}
		break;

	//king
	case 6:
		for (int i = 0; i < 8; i++)
		{
			if ((x2 - x1 == LEGAL_MOVES_KING_X[i]) && (y2 - y1 == LEGAL_MOVES_KING_Y[i]))
			{
				return 1;
			}
		}
		break;
	}
	return 0;
}

double EvaluateCell(Board board, int x, int y)
{
	int piece = GetPiece(board, x, y);
	int player = GetPlayer(board, x, y);
	double positionalValue = 0, attackingValue, defenceValue, value = 0;
	switch (piece)
	{
	//pawn
	case 1:
		if (player == 1)
			positionalValue = PAWN_EVALUATION[y][x];
		else
			positionalValue = PAWN_EVALUATION[SIZE - y - 1][x];
		break;

	//knight
	case 2:
		if (player == 1)
			positionalValue = KNIGHT_EVALUATION[y][x];
		else
			positionalValue = KNIGHT_EVALUATION[SIZE - y - 1][x];
		break;
	}
	value = positionalValue;
	return value * (double)player;
}

void Evaluate()
{
	evaluation = 0;
	for (int y = 0; y < SIZE; y++)
	{
		for (int x = 0; x < SIZE; x++)
		{
			evaluation += EvaluateCell(displayBoard, x, y);
		}
	}
}

void DrawBoard()
{
	int lastSx = -1, lastSy = -1, lastDx = -1, lastDy = -1;
	if (currentMove > 0)
	{
		char *move = moves[currentMove - 1];
		char lastSrc[] = { move[0], move[1] };
		char lastDest[] = { move[2], move[3] };
		ReadChessNotationPosition
		(lastSrc, &lastSx, &lastSy);
		ReadChessNotationPosition
		(lastDest, &lastDx, &lastDy);
	}

	printf("\n==+");
	for (int x = 0; x < SIZE; x++)
	{
		printf("==");
	}
	printf("\n");
	for (int y = SIZE - 1; y >= 0; y--)
	{
		printf("%d |", y + 1);
		for (int x = 0; x < SIZE; x++)
		{
			int piece = GetCell(displayBoard, x, y);
			if (selectedX == x && selectedY == y) printf(PieceToHighlightedChar(GetCell(displayBoard, x, y)));
			else if (CanPieceMoveHere(displayBoard, selectedX, selectedY, x, y))
			{
				if (!IsEnemy(displayBoard, selectedX, selectedY, x, y)) printf(PieceToHighlightedChar(piece));
				else printf(C_RED C_BOLD "%s" C_RESET, PieceToChar(piece));
			}
			else if (lastSx == x && lastSy == y) printf(C_BOLD C_YELLOW "*" C_RESET);
			else if (lastDx == x && lastDy == y) printf(C_YELLOW C_BOLD "%s" C_RESET, PieceToChar(piece));
			else printf("%s", PieceToColoredChar(piece));
			if (x + 1 < SIZE) printf(" ");
		}
		printf("|\n");
	}
	printf("==+");
	for (int x = 0; x < SIZE; x++)
	{
		printf("==");
	}
	printf("\n  |");
	for (int x = 0; x < SIZE; x++)
	{
		printf("%c ", 'A' + x);
	}
	printf("\n\n");
}

int ReadChessNotationPosition(char *position, int *x, int *y)
{
	if (strlen(position) < 2) return 0;

	int sx = (int)(position[0] - 'a');
	int sy = (int)(position[1] - '1');

	if (!InBounds(sx, sy)) return 0;

	*x = sx;
	*y = sy;

	return 1;
}

int ReadMove(char input[], Move *move)
{
	if (strlen(input) != 4)
		return 0;

	char positionSrc[3] = { input[0], input[1] };
	char positionDest[3] = { input[2], input[3] };

	if (!ReadChessNotationPosition(positionSrc, &move->srcX, &move->srcY) || !ReadChessNotationPosition(positionDest, &move->destX, &move->destY))
		return 0;

	return 1;
}

int TryMove(Board board, Move *move)
{
	int srcX = move->srcX;
	int srcY = move->srcY;
	int destX = move->destX;
	int destY = move->destY;
	if (!InBounds(srcX, srcY) || !InBounds(destX, destY)) return 0;
	int srcCell = GetCell(board, srcX, srcY);
	int destCell = GetCell(board, srcX, srcY);
	int srcPlayer = GetPlayer(displayBoard, srcX, srcY);
	int destPlayer = GetPlayer(displayBoard, srcX, srcY);

	if (srcPlayer == playerTurn)
	{
		if (CanPieceMoveHere(board, srcX, srcY, destX, destY))
		{
			SetCell(board, destX, destY, srcCell);
			SetCell(board, srcX, srcY, 0);
			if (selectedX == srcX && selectedY == srcY)
			{
				selectedX = destX;
				selectedY = destY;
			}
			return 1;
		}
	}
	return 0;
}

int TryCommand(char *command)
{
	if (!strcmp(command, "/help"))
	{
		Help();
		return 1;
	}
	if (!strcmp(command, "/trace"))
	{
		PrintMoveTrace();
		return 1;
	}
	if (!strcmp(command, "/newgame"))
	{
		NewGame();
		return 1;
	}
	if (!strcmp(command, "/stats"))
	{
		PrintGameStats();
		return 1;
	}
	if (!strcmp(command, "/redraw"))
	{
		DrawBoard();
		return 1;
	}
	if (!strcmp(command, "/select"))
	{
		SelectCell();
		DrawBoard();
		return 1;
	}
	if (!strcmp(command, "/deselect"))
	{
		DeselectCell();
		DrawBoard();
		return 1;
	}
	if (!strcmp(command, "/wcomp"))
	{
		ChangeComputer(1);
		return 1;
	}
	if (!strcmp(command, "/bcomp"))
	{
		ChangeComputer(-1);
		return 1;
	}
	return 0;
}

int main()
{
	NewGame();
	char input[INPUT_LENGTH + 1]; // +1 for \0

	while (1)
	{
		if ((playerTurn == -1 && !blackComputer) || playerTurn == 1 && !whiteComputer)
		{
			printf("%s player, your turn.\n", PlayerToColoredName(playerTurn));
			printf("Enter your move or command: \n" C_GREEN);
			scanf("%" INPUT_LENGTH_STR "s", input);
			printf(C_RESET);

			if (input[0] == '/')
			{
				if (!TryCommand(input))
				{
					printf("Unknown command \"" C_CYAN "%s" C_RESET "\". Type \"/help\".\n", input);
					continue;
				}
			}
			else
			{
				Move *move = move_new(0, 0, 0, 0);
				if (!ReadMove(input, move) || !TryMove(displayBoard, move))
				{
					printf(C_RED "Invalid move! Try again.\n" C_RESET);
					continue;
				}
				free(move);
				strcpy(moves[currentMove], input);
				NextMove();
			}
		}
		else 
		{
			Move *compMove;
			if (compMove = TryComputerMove(displayBoard, playerTurn))
			{
				MoveToText(compMove, input);
				strcpy(moves[currentMove], input);
				free(compMove);
				NextMove();
			}
			else break;
		}
	}
	return 0;
}