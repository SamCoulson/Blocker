#ifndef INC_CGAMEPIECE_H
#define INC_CGAMEPIECE_H

#include "CGrid.h"
#include <time.h>

#define CLEAR 			0
#define STATIC 			9

#define RIGHT 10
#define LEFT 11
#define DOWN 12
#define ROTATE 13

#define RED    20
#define BLUE   30
#define GREEN  40
#define PURPLE 50
#define YELLOW 60

#define STATIC_RED    120
#define STATIC_BLUE   130
#define STATIC_GREEN  140
#define STATIC_PURPLE 150
#define STATIC_YELLOW 160


class Grid;

class CGamePiece
{
public:
	CGamePiece();
	~CGamePiece();

	void SpawnNewPiece(void);
	void RotatePiece();
	
	unsigned int getPosX(){ return currentPosX; }
	unsigned int getPosY(){ return currentPosY; }
	
	void setPosX( unsigned int pos ){ currentPosX = pos; }
	void setPosY( unsigned int pos ){ currentPosY = pos; }

	unsigned int getWidth(){ return pCurrentPiece->piece_width; }
	unsigned int getHeight(){ return pCurrentPiece->piece_height; }

	unsigned int getPieceData( unsigned int pos ){ return pCurrentPiece->pPiece[pos]; }
	unsigned int getColor(){ return pCurrentPiece->colour; }
	
private:
	// Current X and Y in grid coords 
	unsigned int m_current_piece, m_prepiece, m_current_rotation, m_direction;

	// Track current piece position
	unsigned int currentPosX, currentPosY;  

	// Structure to store data to construct piece
	typedef struct PIECES
	{
		unsigned int piece_width;
		unsigned int piece_height;
		char *pPiece;
		unsigned int colour;
	}m_Piece_s;	
	
	// Structures for all piece combinations
	m_Piece_s GamePieces[7][4];
	
	// Set up each piece 
	void InitGamePieces( void );

	m_Piece_s *pCurrentPiece;
	m_Piece_s *pPrePiece;
	
};



#endif // INC_CGAMEPIECE_H
