#include "../include/CGamePiece.h"
#include <stdlib.h>

CGamePiece::CGamePiece()
{
	InitGamePieces();
	
	currentPosX = 4;
	currentPosY = 0;

	m_direction = 0;

	m_current_rotation = 0;
	m_current_piece = 0;
	
	pPrePiece = NULL;
	pCurrentPiece = NULL;
	
	// Choose new random piece for pre-piece
	srand( (unsigned int )time( NULL) );
	m_current_piece = rand() % 6;

	// Get pointer to char string that describes the piece
	pCurrentPiece = &GamePieces[ m_current_piece ][ m_current_rotation ];

}

CGamePiece::~CGamePiece()
{

}


void CGamePiece::SpawnNewPiece()
{
	
	// Set current to piece index to what was displayed in the next piece box
	if( this->pPrePiece != NULL )
	{
		// If valid pointer assign current piece to pre-piece
		this->pCurrentPiece = &GamePieces[ m_prepiece ][ m_current_rotation ];

		// Set the piece index to current piece
		this->m_current_piece = m_prepiece;
	}
	
	// Set the pre-piece pointer to point to the next game piece, this is for the next piece box
	pPrePiece = &GamePieces[ m_prepiece ][ m_current_rotation ];
	
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
}



void CGamePiece::InitGamePieces( void )
{
	
	// Line shape
	// Position 1
	CGamePiece::GamePieces[0][0].piece_width = 1;
	CGamePiece::GamePieces[0][0].piece_height = 4;
	CGamePiece::GamePieces[0][0].colour = YELLOW;
	CGamePiece::GamePieces[0][0].pPiece = "1"
					      "1"
					      "1"
					      "1";
	
	//Position 2
	CGamePiece::GamePieces[0][1].piece_width = 4;
	CGamePiece::GamePieces[0][1].piece_height = 1;
	CGamePiece::GamePieces[0][1].colour = YELLOW;
	CGamePiece::GamePieces[0][1].pPiece = "1111";
							
						
	//Position 3
	CGamePiece::GamePieces[0][2] = CGamePiece::GamePieces[0][0];

	//Position 4
	CGamePiece::GamePieces[0][3] = CGamePiece::GamePieces[0][1];

	// Right L Shape
	// Position 1
	CGamePiece::GamePieces[1][0].piece_width = 2;
	CGamePiece::GamePieces[1][0].piece_height = 3;
	CGamePiece::GamePieces[1][0].colour = RED;
	CGamePiece::GamePieces[1][0].pPiece = "10"
					      "10"
					      "11";
	// Position 2
	CGamePiece::GamePieces[1][1].piece_width = 3;
	CGamePiece::GamePieces[1][1].piece_height = 2;
	CGamePiece::GamePieces[1][1].colour = RED;
	CGamePiece::GamePieces[1][1].pPiece = "001"
					      "111";
	// Position 3
	CGamePiece::GamePieces[1][2].piece_width = 2;
	CGamePiece::GamePieces[1][2].piece_height = 3;
	CGamePiece::GamePieces[1][2].colour = RED;
	CGamePiece::GamePieces[1][2].pPiece = "11"
					      "01"
					      "01";
	// Position 4
	CGamePiece::GamePieces[1][3].piece_width = 3;
	CGamePiece::GamePieces[1][3].piece_height = 2;
	CGamePiece::GamePieces[1][3].colour = RED;
	CGamePiece::GamePieces[1][3].pPiece = "111"
					      "100";

	// Triangle shape
	//Position 1
	CGamePiece::GamePieces[2][0].piece_width = 3;
	CGamePiece::GamePieces[2][0].piece_height = 2;
	CGamePiece::GamePieces[2][0].colour = BLUE;
	CGamePiece::GamePieces[2][0].pPiece = "010"
					      "111";
	//Position 2
	CGamePiece::GamePieces[2][1].piece_width = 2;
	CGamePiece::GamePieces[2][1].piece_height = 3;
	CGamePiece::GamePieces[2][1].colour = BLUE;
	CGamePiece::GamePieces[2][1].pPiece = "10"
					      "11"
					      "10";
	//Position 3
	CGamePiece::GamePieces[2][2].piece_width = 3;
	CGamePiece::GamePieces[2][2].piece_height = 2;
	CGamePiece::GamePieces[2][2].colour = BLUE;
	CGamePiece::GamePieces[2][2].pPiece = "111"
					      "010";
	//Position 4
	CGamePiece::GamePieces[2][3].piece_width = 2;
	CGamePiece::GamePieces[2][3].piece_height = 3;
	CGamePiece::GamePieces[2][3].colour = BLUE;
	CGamePiece::GamePieces[2][3].pPiece = "01"
					      "11"
					      "01";
	// Right hand Jagged shape
	//Position 1
	CGamePiece::GamePieces[3][0].piece_width = 2;
	CGamePiece::GamePieces[3][0].piece_height = 3;
	CGamePiece::GamePieces[3][0].colour = GREEN;
	CGamePiece::GamePieces[3][0].pPiece = "01"
					      "11"
					      "10";
	//Position 2
	CGamePiece::GamePieces[3][1].piece_width = 3;
	CGamePiece::GamePieces[3][1].piece_height = 2;
	CGamePiece::GamePieces[3][1].colour = GREEN;
	CGamePiece::GamePieces[3][1].pPiece = "110"
					      "011";
	//Position 3
	CGamePiece::GamePieces[3][2].piece_width = 2;
	CGamePiece::GamePieces[3][2].piece_height = 3;
	CGamePiece::GamePieces[3][2].colour = GREEN;
	CGamePiece::GamePieces[3][2].pPiece = "01"
										  "11"
										  "10";
	
	//Position 4
	CGamePiece::GamePieces[3][3].piece_width = 3;
	CGamePiece::GamePieces[3][3].piece_height = 2;
	CGamePiece::GamePieces[3][3].colour = GREEN;
	CGamePiece::GamePieces[3][3].pPiece = "110"
										  "011";

	//Block shape
	//Position 1
	CGamePiece::GamePieces[4][0].piece_width = 2;
	CGamePiece::GamePieces[4][0].piece_height = 2;
	CGamePiece::GamePieces[4][0].colour = PURPLE;
	CGamePiece::GamePieces[4][0].pPiece = "11"
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
	CGamePiece::GamePieces[5][0].pPiece = "01"
										  "01"
										  "11";
	//Position 2
	CGamePiece::GamePieces[5][1].piece_width = 3;
	CGamePiece::GamePieces[5][1].piece_height = 2;
	CGamePiece::GamePieces[5][1].colour = RED;
	CGamePiece::GamePieces[5][1].pPiece = "100"
										  "111";
	//Position 3
	CGamePiece::GamePieces[5][2].piece_width = 2;
	CGamePiece::GamePieces[5][2].piece_height = 3;
	CGamePiece::GamePieces[5][2].colour = RED;
	CGamePiece::GamePieces[5][2].pPiece = "11"
										  "10"
										  "10";
	//Position 4
	CGamePiece::GamePieces[5][3].piece_width = 3;
	CGamePiece::GamePieces[5][3].piece_height = 2;
	CGamePiece::GamePieces[5][3].colour = RED;
	CGamePiece::GamePieces[5][3].pPiece = "111"
					      "001";
	//Left hand jagged shape
	//Position 1
	CGamePiece::GamePieces[6][0].piece_width = 2;
	CGamePiece::GamePieces[6][0].piece_height = 3;
	CGamePiece::GamePieces[6][0].colour = GREEN;
	CGamePiece::GamePieces[6][0].pPiece = "10"
					      "11"
					      "01";
	//Position 2
	CGamePiece::GamePieces[6][1].piece_width = 3;
	CGamePiece::GamePieces[6][1].piece_height = 2;
	CGamePiece::GamePieces[6][1].colour = GREEN;
	CGamePiece::GamePieces[6][1].pPiece = "011"
					      "110";
	//Position 3
	CGamePiece::GamePieces[6][2] = CGamePiece::GamePieces[6][0];

	//Position 4
	CGamePiece::GamePieces[6][3] = CGamePiece::GamePieces[6][1];
}




