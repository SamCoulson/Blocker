#include "../include/CGamePiece.h"
#include <stdlib.h>
#include <iostream>

CGamePiece::CGamePiece( unsigned int startPosX, unsigned int startPosY )
{
	InitGamePieces();
	
	currentPosX = startPosX;
	currentPosY = startPosY;

	m_direction = 0;

	m_current_rotation = 0;
	m_current_piece = 0;
		
	pCurrentPiece = NULL;
	
	// Choose new random piece for pre-piece
	srand( (unsigned int )time( NULL) );
	m_current_piece = rand() % 6;

	m_current_rotation = rand() % 3;

	// Get pointer to char string that describes the piece
	pCurrentPiece = &GamePieces[ m_current_piece ][ m_current_rotation ];

}

CGamePiece::~CGamePiece()
{
	//if( pCurrentPiece != NULL )
	//	delete pCurrentPiece;
}

void CGamePiece::setRotation( unsigned int rotation ){
	pCurrentPiece = &GamePieces[ m_current_piece ][ rotation ];		
}

void CGamePiece::RotatePiece()
{
	// Could do some position calculations here to allow rotation to look correct i.e. the bar is wrong at the moment

	if( m_current_rotation < 3 ){
		// Set piece rotation to next rotation
		m_current_rotation++;
		pCurrentPiece = &GamePieces[ m_current_piece ][ m_current_rotation ];	
	}
	else{
		// Reset the piece rotation
		m_current_rotation = 0;
		pCurrentPiece = &GamePieces[ m_current_piece ][ m_current_rotation ];
	}

	// Adjust position for lines
	// Should rotate around the sec block in from the left in horizontal position
	if( pCurrentPiece->colour == YELLOW ){

		switch( m_current_rotation ){
			case 0:
				currentPosX += 1;
				break;
			case 1:
				currentPosX -= 1;
				break;
			case 2:
				currentPosX += 1;
				break;
			case 3:
				currentPosX -= 1;
				break;
			default:
				break;
		}
	}
}



void CGamePiece::InitGamePieces( void )
{
	// Using (char*) to get rid of GCC 'deprecated conversion from string constant to char*' 

	// Line shape
	// Position 1
	CGamePiece::GamePieces[0][0].piece_width = 1;
	CGamePiece::GamePieces[0][0].piece_height = 4;
	CGamePiece::GamePieces[0][0].colour = YELLOW;
	CGamePiece::GamePieces[0][0].pPiece = (char*)"1"
					      "1"
					      "1"
					      "1";
	
	//Position 2
	CGamePiece::GamePieces[0][1].piece_width = 4;
	CGamePiece::GamePieces[0][1].piece_height = 1;
	CGamePiece::GamePieces[0][1].colour = YELLOW;
	CGamePiece::GamePieces[0][1].pPiece = (char*)"1111";
							
						
	//Position 3
	CGamePiece::GamePieces[0][2] = CGamePiece::GamePieces[0][0];

	//Position 4
	CGamePiece::GamePieces[0][3] = CGamePiece::GamePieces[0][1];

	// Right L Shape
	// Position 1
	CGamePiece::GamePieces[1][0].piece_width = 2;
	CGamePiece::GamePieces[1][0].piece_height = 3;
	CGamePiece::GamePieces[1][0].colour = RED;
	CGamePiece::GamePieces[1][0].pPiece = (char*)"10"
					      "10"
					      "11";
	// Position 2
	CGamePiece::GamePieces[1][1].piece_width = 3;
	CGamePiece::GamePieces[1][1].piece_height = 2;
	CGamePiece::GamePieces[1][1].colour = RED;
	CGamePiece::GamePieces[1][1].pPiece = (char*)"001"
					      "111";
	// Position 3
	CGamePiece::GamePieces[1][2].piece_width = 2;
	CGamePiece::GamePieces[1][2].piece_height = 3;
	CGamePiece::GamePieces[1][2].colour = RED;
	CGamePiece::GamePieces[1][2].pPiece = (char*)"11"
					      "01"
					      "01";
	// Position 4
	CGamePiece::GamePieces[1][3].piece_width = 3;
	CGamePiece::GamePieces[1][3].piece_height = 2;
	CGamePiece::GamePieces[1][3].colour = RED;
	CGamePiece::GamePieces[1][3].pPiece = (char*)"111"
					      "100";

	// Triangle shape
	//Position 1
	CGamePiece::GamePieces[2][0].piece_width = 3;
	CGamePiece::GamePieces[2][0].piece_height = 2;
	CGamePiece::GamePieces[2][0].colour = BLUE;
	CGamePiece::GamePieces[2][0].pPiece = (char*)"010"
					      "111";
	//Position 2
	CGamePiece::GamePieces[2][1].piece_width = 2;
	CGamePiece::GamePieces[2][1].piece_height = 3;
	CGamePiece::GamePieces[2][1].colour = BLUE;
	CGamePiece::GamePieces[2][1].pPiece = (char*)"10"
					      "11"
					      "10";
	//Position 3
	CGamePiece::GamePieces[2][2].piece_width = 3;
	CGamePiece::GamePieces[2][2].piece_height = 2;
	CGamePiece::GamePieces[2][2].colour = BLUE;
	CGamePiece::GamePieces[2][2].pPiece = (char*)"111"
					      "010";
	//Position 4
	CGamePiece::GamePieces[2][3].piece_width = 2;
	CGamePiece::GamePieces[2][3].piece_height = 3;
	CGamePiece::GamePieces[2][3].colour = BLUE;
	CGamePiece::GamePieces[2][3].pPiece = (char*)"01"
					      "11"
					      "01";
	// Right hand Jagged shape
	//Position 1
	CGamePiece::GamePieces[3][0].piece_width = 2;
	CGamePiece::GamePieces[3][0].piece_height = 3;
	CGamePiece::GamePieces[3][0].colour = GREEN;
	CGamePiece::GamePieces[3][0].pPiece = (char*)"01"
					      "11"
					      "10";
	//Position 2
	CGamePiece::GamePieces[3][1].piece_width = 3;
	CGamePiece::GamePieces[3][1].piece_height = 2;
	CGamePiece::GamePieces[3][1].colour = GREEN;
	CGamePiece::GamePieces[3][1].pPiece = (char*)"110"
					      "011";
	//Position 3
	CGamePiece::GamePieces[3][2].piece_width = 2;
	CGamePiece::GamePieces[3][2].piece_height = 3;
	CGamePiece::GamePieces[3][2].colour = GREEN;
	CGamePiece::GamePieces[3][2].pPiece = (char*)"01"
					      "11"
					      "10";
	
	//Position 4
	CGamePiece::GamePieces[3][3].piece_width = 3;
	CGamePiece::GamePieces[3][3].piece_height = 2;
	CGamePiece::GamePieces[3][3].colour = GREEN;
	CGamePiece::GamePieces[3][3].pPiece = (char*)"110"
					      "011";

	//Block shape
	//Position 1
	CGamePiece::GamePieces[4][0].piece_width = 2;
	CGamePiece::GamePieces[4][0].piece_height = 2;
	CGamePiece::GamePieces[4][0].colour = PURPLE;
	CGamePiece::GamePieces[4][0].pPiece = (char*)"11"
					      "11";
	//Position 2
	CGamePiece::GamePieces[4][1] = CGamePiece::GamePieces[4][0];
	CGamePiece::GamePieces[4][1].colour = PURPLE;

	//Position 3
	CGamePiece::GamePieces[4][2] = CGamePiece::GamePieces[4][0];
	CGamePiece::GamePieces[4][2].colour = PURPLE;

	//Position 4
	CGamePiece::GamePieces[4][3] = CGamePiece::GamePieces[4][0];
	CGamePiece::GamePieces[4][3].colour = PURPLE;

	//Left L Shape
	//Position 1
	CGamePiece::GamePieces[5][0].piece_width = 2;
	CGamePiece::GamePieces[5][0].piece_height = 3;
	CGamePiece::GamePieces[5][0].colour = RED;
	CGamePiece::GamePieces[5][0].pPiece = (char*)"01"
					      "01"
					      "11";
	//Position 2
	CGamePiece::GamePieces[5][1].piece_width = 3;
	CGamePiece::GamePieces[5][1].piece_height = 2;
	CGamePiece::GamePieces[5][1].colour = RED;
	CGamePiece::GamePieces[5][1].pPiece = (char*)"100"
					      "111";
	//Position 3
	CGamePiece::GamePieces[5][2].piece_width = 2;
	CGamePiece::GamePieces[5][2].piece_height = 3;
	CGamePiece::GamePieces[5][2].colour = RED;
	CGamePiece::GamePieces[5][2].pPiece = (char*)"11"
					      "10"
					      "10";
	//Position 4
	CGamePiece::GamePieces[5][3].piece_width = 3;
	CGamePiece::GamePieces[5][3].piece_height = 2;
	CGamePiece::GamePieces[5][3].colour = RED;
	CGamePiece::GamePieces[5][3].pPiece = (char*)"111"
					      "001";
	//Left hand jagged shape
	//Position 1
	CGamePiece::GamePieces[6][0].piece_width = 2;
	CGamePiece::GamePieces[6][0].piece_height = 3;
	CGamePiece::GamePieces[6][0].colour = GREEN;
	CGamePiece::GamePieces[6][0].pPiece = (char*)"10"
					      "11"
					      "01";
	//Position 2
	CGamePiece::GamePieces[6][1].piece_width = 3;
	CGamePiece::GamePieces[6][1].piece_height = 2;
	CGamePiece::GamePieces[6][1].colour = GREEN;
	CGamePiece::GamePieces[6][1].pPiece = (char*)"011"
					      "110";
	//Position 3
	CGamePiece::GamePieces[6][2] = CGamePiece::GamePieces[6][0];

	//Position 4
	CGamePiece::GamePieces[6][3] = CGamePiece::GamePieces[6][1];
}




