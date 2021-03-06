#include "GameConstants.h"

const Piece 
NoPiece			=  0,
Pawn			=  1, 
Knight			=  2, 
Bishop			=  3, 
Rook			=  4, 
Queen			=  5, 
King			=  6;

const Player 
White			=  1, 
Black			= -1, 
UnknownPlayer	=  0;

const Cell
Empty			=  0,
WhitePawn		=  1,
BlackPawn		= -1,
WhiteKnight		=  2,
BlackKnight		= -2,
WhiteBishop		=  3,
BlackBishop		= -3,
WhiteRook		=  4,
BlackRook		= -4,
WhiteQueen		=  5,
BlackQueen		= -5,
WhiteKing		=  6,
BlackKing		= -6;

const int LEGAL_MOVES_KNIGHT_X[8] =
{ 2, 1, -1, -2, -2, -1, 1, 2 };
const int
LEGAL_MOVES_KNIGHT_Y[8] =
{ 1, 2, 2, 1, -1, -2, -2, -1 };

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

const Position START_POSITION = {
{ 4 , 2 , 3 , 5 , 6 , 3 , 2 , 4 },
{ 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 },
{-4, -2 ,-3 ,-5 ,-6 ,-3 ,-2 ,-4 }};

const Position TEST_POSITION = {
{-6 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 5 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
{ 0 , 4 , 0 , 0 , 6 , 0 , 0 , 0 },
{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }};

const BoardEvaluation PAWN_EVALUATION = {
{0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
{1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00},
{1.01, 1.02, 1.05, 1.10, 1.10, 1.05, 1.02, 1.01},
{1.03, 1.07, 1.10, 1.16, 1.16, 1.10, 1.07, 1.03},
{1.08, 1.12, 1.17, 1.25, 1.25, 1.17, 1.12, 1.08},
{1.15, 1.20, 1.25, 1.35, 1.35, 1.25, 1.20, 1.15},
{1.30, 1.35, 1.40, 1.45, 1.45, 1.40, 1.35, 1.30},
{0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00}};

const BoardEvaluation KNIGHT_EVALUATION = {
{1.50, 2.00, 2.25, 2.25, 2.25, 2.25, 2.00, 1.50},
{2.00, 2.25, 2.50, 2.50, 2.50, 2.50, 2.25, 2.00},
{2.25, 2.50, 3.00, 3.00, 3.00, 3.00, 2.50, 2.25},
{2.25, 2.50, 3.25, 3.00, 3.00, 3.25, 2.50, 2.25},
{2.25, 2.50, 4.00, 3.50, 3.50, 4.00, 2.50, 2.25},
{2.25, 2.50, 4.50, 4.00, 4.00, 4.50, 2.50, 2.25},
{2.00, 2.25, 2.50, 2.50, 2.50, 2.50, 2.25, 2.00},
{1.50, 2.00, 2.25, 2.25, 2.25, 2.25, 2.00, 1.50}};

const BoardEvaluation BISHOP_EVALUATION = {
{1.25, 1.50, 1.75, 2.00, 2.00, 1.75, 1.50, 1.25},
{1.50, 3.00, 2.00, 2.50, 2.50, 2.00, 3.00, 1.50},
{1.75, 2.50, 3.50, 3.00, 3.00, 3.50, 2.50, 1.75},
{2.00, 2.25, 3.00, 4.00, 4.00, 3.00, 2.50, 2.00},
{2.00, 2.25, 3.00, 4.00, 4.00, 3.00, 2.50, 2.00},
{1.75, 2.25, 3.50, 3.00, 3.00, 3.50, 2.25, 1.75},
{1.50, 2.25, 2.50, 2.50, 2.50, 2.50, 2.25, 1.50},
{1.25, 1.50, 1.75, 2.00, 2.00, 1.75, 1.50, 1.25}};