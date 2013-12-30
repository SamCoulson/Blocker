#include "../include/CGameScreen.h"

using namespace std;

CGameArea::CGameArea( CSDLGraphics *graphics ){

	this->graphics = graphics;
	m_GameScore = 0;
	m_GameSpeed = 300;

	keyPressed = 0;

	memset(score, 0, sizeof( score ));
}

CGameArea::~CGameArea(){

	// Free surface
	if( gameframe != NULL ){
		SDL_FreeSurface( gameframe );
	}

	if( gametile != NULL ){
		SDL_FreeSurface( gametile );
	}

	if( gameGrid != NULL ){
		delete gameGrid;
	}
}

bool CGameArea::init(){	
 	
	// Create grid just for the game area 
	gameGrid = new CGrid( GAMEAREAWIDTH, GAMEAREAHEIGHT, GAMEAREA_TILE_SIZE, GAMEAREA_TILE_SIZE, 20, 20 ); 
	if( gameGrid == NULL )
	{
		std::cout << "Cannot create GameGrid Object" << std::endl;
		return false;
	}	
	
	// Load in bitmaps for game areas
	gameframe = graphics->loadImageFromFile( "./images/GameFrameRounded.bmp", 255, 0, 255 );
	
	// Loadsprite sheet *add error checking*
	gametile = graphics->loadImageFromFile("./images/TileSet.bmp", 255, 0, 255 );

	// Spawn first piece
	currentPiece = new CGamePiece();	

	quit = false;

	return true;
}

void CGameArea::input( SDL_Event *event )
{
	if( event->type == SDL_KEYDOWN)
	{
		switch(event->key.keysym.sym)
		{
			case SDLK_RIGHT:
				keyPressed = RIGHT;
			break;
			case SDLK_LEFT:
				keyPressed = LEFT;
			break;
			case SDLK_SPACE:
				keyPressed = ROTATE;
			break;
			case SDLK_DOWN:
				//CPlayerPiece.Move(DOWN);
			break;
			case SDLK_ESCAPE:
				quit = true;
			break;
			case SDLK_F11:
				/*if(g_bPaused == true)
				{
					g_bPaused = false;
				}
				else
				{
					g_bPaused = true;
				}*/
			break;
			default:
				break;
		}
	}
	// If window has been X'ed
	if( event->type == SDL_QUIT)
	{
		quit = true;
	}

	// return key to up state to prevent multiple moves per game loop
	event->type = SDL_KEYUP;

}


void CGameArea::update(){
	
	// Set all ISGAMEBLOCK in logical game grid to CLEAR.  This clears the grid of the old curgamepiece position, else it will be drawn also.
	// Also clears the next piece box.
	bool staticMode = false;

	ClearGrid();

	// Save current position and rotation
	unsigned int posX = currentPiece->getPosX();
	unsigned int posY = currentPiece->getPosY();


	// Move as dictated by control or game
	// Check for collision if collide then reset position and set as static

	switch( keyPressed ){
		case RIGHT:
			//if( ( currentPiece->getPosX() + currentPiece->getWidth() ) < GAMEAREAWIDTH ){
				currentPiece->setPosX( currentPiece->getPosX()+1 );
			//}
			break;
		case LEFT:
			//if( ( currentPiece->getPosX() ) > 0 ){
				currentPiece->setPosX( currentPiece->getPosX()-1 );		
			//}
			break;
		case ROTATE:
			currentPiece->RotatePiece();
			break;
		default:
		break;
	}

	keyPressed = 0; // Reset keypressed	
	
	//if( ( currentPiece->getPosY() + currentPiece->getHeight() ) < GAMEAREAHEIGHT ){
		currentPiece->setPosY( currentPiece->getPosY()+1 );	
//	}else{
		// staticMode = true;	
//	}
	
	if( ( ( currentPiece->getPosX() + currentPiece->getWidth() ) > GAMEAREAWIDTH ) ||
	( currentPiece->getPosX() > GAMEAREAWIDTH+1 ) ){
		// Collided restore old position	
		currentPiece->setPosY( posX );
	}else if( ( currentPiece->getPosY() + currentPiece->getHeight() ) > GAMEAREAHEIGHT ){
		currentPiece->setPosX( posY );
		staticMode = true;	
	}


	unsigned int blockType = 0;
	if( staticMode ){
		blockType = currentPiece->getColor() + 100;	
	}else{
		blockType = currentPiece->getColor();	
	}

	unsigned int current_block_piece = 0;		

	// Iterate through the current block data and draw those block that are visible
	for( int row_index = 0; row_index < currentPiece->getHeight(); row_index++)
	{
		for( int column_index = 0; column_index < currentPiece->getWidth(); column_index++)
		{
			if( currentPiece->getPieceData( current_block_piece ) == '1')
			{
				gameGrid->setTileFlag( currentPiece->getPosX() + column_index, 
						currentPiece->getPosY() + row_index, blockType );
			}
		// Next block piece in block array
		current_block_piece++;
		}
	}
	// Block must be at bottom of screen so set the cells it occupies to static	
	// Spawn new block
	if( staticMode == true ){
		delete currentPiece;
		currentPiece = new CGamePiece();	
	}

	// Doesnt because piece is never spawned beyond this point
	//if( ( CPlayerPiece.GetPos_y() + CPlayerPiece.pCurrentPiece->piece_height ) < 2 )
	//	g_bPaused = true;

	// Can alter this to change game speed is also the continual movement heart beat
	//if( GetTickCount() - start_time > CPlayArea.m_GameSpeed) 
	//{	
	//	CPlayerPiece.Move(DOWN);
	//	start_time=GetTickCount();
	//}
	// Increase the speed if next level has been reached
	//CPlayArea.CheckandSetLevel();

	// Draw piece for every frame both lines
	//CPlayerPiece.PutPiece(CPlayerPiece.pCurrentPiece, CPlayerPiece.GetPos_x(), CPlayerPiece.GetPos_y(), ISBLOCKPIECE, g_pGameGrid);

	// Put pre-piece
	//CPlayerPiece.PutPiece(CPlayerPiece.pPrePiece, 17, 23, ISBLOCKPIECE, g_pScreenGrid);

	// Reset pre-piece area *doesnt work properly*
	//g_pScreenGrid->SetSDLGridBitmapRepeated(g_pScreenGrid->GetSDLGridXY(15, 11).x, g_pScreenGrid->GetSDLGridXY(15, 11).y, 
	//	g_pScreenGrid->GetSDLGridXY(20, 17).x, g_pScreenGrid->GetSDLGridXY(20, 17).y, NULL, NULL);

	// Are any lines full if so adjust blocks
	//CPlayArea.IsFullLine();

	//if(CPlayArea.IsGameOver() == 1)
	//{
	//	g_bPaused = true;
	//	g_bGameOver = true;
	//}
	
}

void CGameArea::render(){
	
	// Clear screen
	graphics->clearScreen( 0, 0, 0 );

	// Draw game frame	
	graphics->draw( 0 ,0 , gameframe, SDL_GetVideoSurface(), NULL );
	
	// Draw all the blocks in the game area	
	SDL_Rect redTile;
	redTile.x = 40;
	redTile.y = 0;
	redTile.w = 20;
	redTile.h = 20;
	
	SDL_Rect blueTile;
	blueTile.x = 20;
	blueTile.y = 0;
	blueTile.w = 20;
	blueTile.h = 20;
	
	SDL_Rect greenTile;
	greenTile.x = 60;
	greenTile.y = 0;
	greenTile.w = 20;
	greenTile.h = 20;
	
	SDL_Rect purpleTile;
	purpleTile.x = 80;
	purpleTile.y = 0;
	purpleTile.w = 20;
	purpleTile.h = 20;
	
	SDL_Rect yellowTile;
	yellowTile.x = 0;
	yellowTile.y = 0;
	yellowTile.w = 20;
	yellowTile.h = 20;

	// Draw game area with player piece
	for( int i = 0; i < GAMEAREAWIDTH; i++ ){
		for( int j = 0; j < GAMEAREAHEIGHT; j++ ){
			switch( gameGrid->getTileFlag( i, j) ){
			case RED:
			case STATIC_RED:					
				graphics->draw( gameGrid->getXY(i, j)->x, gameGrid->getXY( i, j)->y, 
						gametile, SDL_GetVideoSurface(), &redTile );
				
				break;
			case BLUE:
			case STATIC_BLUE:
				graphics->draw( gameGrid->getXY(i, j)->x, gameGrid->getXY( i, j)->y, 
						gametile, SDL_GetVideoSurface(), &blueTile );
				break;
			case GREEN:
			case STATIC_GREEN:
				graphics->draw( gameGrid->getXY(i, j)->x, gameGrid->getXY( i, j)->y, 
						gametile, SDL_GetVideoSurface(), &greenTile );
				break;
			case PURPLE:
			case STATIC_PURPLE:
				graphics->draw( gameGrid->getXY(i, j)->x, gameGrid->getXY( i, j)->y, 
						gametile, SDL_GetVideoSurface(), &purpleTile );
				break;
			case YELLOW:
			case STATIC_YELLOW:
				graphics->draw( gameGrid->getXY(i, j)->x, gameGrid->getXY( i, j)->y, 
						gametile, SDL_GetVideoSurface(), &yellowTile );
				break;
			default:
				break;
			}
		}
	}
	
	//Draw score  
	//g_pScreenGrid->SetGridSDLImage(15, 3, g_pResource->Text(CPlayArea.GetScore(), color));
	
	//g_pScreenGrid->SetGridSDLImage(15, 10, g_pResource->Text(CPlayArea.GetLevel(), color));
	
	//if(g_bGameOver == true)
	//{
		//sdlObj.DrawImage(g_pScreenGrid->GetSDLGridXY(0,10).x, g_pScreenGrid->GetSDLGridXY(0,10).y, gameoverframe, sdlObj.screenSurface, NULL);
	//}

	// Show the screen
	// Or can use update rects
	graphics->update();
	
}


void CGameArea::CheckandSetLevel()
{
	if( m_GameScore >= 1000 )
	{
		m_GameSpeed = 250;
		m_GameLevel = 1;
	}
	if( m_GameScore >= 2000 )
	{
		m_GameSpeed = 200;
		m_GameLevel = 2;
	}
	if( m_GameScore >= 3000 )
	{
		m_GameSpeed = 150;
		m_GameLevel = 3;
	}
	if( m_GameScore >= 4000 )
	{
		m_GameSpeed = 100;
		m_GameLevel = 4;
	}
	if( m_GameScore >= 5000 )
	{
		m_GameSpeed = 50;
		m_GameLevel = 5;
	}
}

const char* CGameArea::GetScore()
{ 
	ConvertScoretoChar();
	return score;
}

int CGameArea::GetIntScore()
{
	return m_GameScore;
}

const char* CGameArea::GetLevel()
{
	sprintf(this->level,"%d", m_GameLevel);
	
	return level;
}

const char* CGameArea::ConvertScoretoChar()
{
	sprintf(this->score,"%d", m_GameScore);
	
	return score;
}

int CGameArea::IsGameOver()
{
	int widthindex = 0;
	for(widthindex = 0; widthindex < GAMEAREAWIDTH; widthindex++)
	{
//		if(g_pGameGrid->GetGridStatus(widthindex, 1) == STATIC)
			return 1;
	}
	return 0;
}

// Resets all logical and image values for grid blocks that are the player piece to clear.  Ready for next frame. 
void CGameArea::ClearGrid()
{
	int row_index, column_index;

	for( column_index = 0; column_index < GAMEAREAHEIGHT; column_index++ )
	{
		for( row_index = 0; row_index < GAMEAREAWIDTH; row_index++ )
		{
			if( gameGrid->getTileFlag( row_index, column_index) < 100 )
			{
				gameGrid->setTileFlag( row_index, column_index, CLEAR );
			}
		}
	}

	// The below is a semi hack to get the next piece box clear.

	for( column_index = 20; column_index < 30; column_index ++)
	{
		for( row_index = 10; row_index < 23; row_index++ )
		{
/*			if(g_pScreenGrid->GetGridStatus(row_index, column_index) == ISBLOCKPIECE)
			{
				g_pScreenGrid->SetGridStatus(row_index, column_index, CLEAR);
				g_pScreenGrid->SetGridSDLImage(row_index, column_index, NULL, NULL);
			}
*/		}
	}
	
}

// Scans whole grid for full lines
int CGameArea::IsFullLine()
{
	int column_index, row_index = 0;

	// Iterate from top to bottom
	for(row_index = 0; row_index < GAMEAREAHEIGHT; row_index++)
	{
		// Iterate from left to right 
		for(column_index = 0; column_index < GAMEAREAWIDTH; column_index++)
		{
			// If current block is Static check to see if you reach the end of the row if so full line
/*			if(g_pGameGrid->GetGridStatus(column_index, row_index) == STATIC)
			{
				// Column index reached the width of the game area a line must be full
				if(column_index == GAMEAREAWIDTH - 1)
				{
					// Delete line
					OnLineFull(row_index);
				}
			}
			else
			{
				// On first non static block stop looking
				break;
			}
*/		}
	}
	// Nothing found
	return 1;
}

// Deletes the row and moves all the rows above down one and increase score.
int CGameArea::OnLineFull(int iFullRowIndex)
{
	
	int row_index, column_index = 0;

	// First delete the row
	for(column_index = 0; column_index < GAMEAREAWIDTH; column_index++)
	{
//		g_pGameGrid->SetGridStatus(column_index, iFullRowIndex, CLEAR);
//		g_pGameGrid->SetGridSDLImage(column_index, iFullRowIndex, NULL, NULL);
	}
	column_index = 0;
	// Iterate up through rows and set all static pieces to plus 1 on the y coord
	for(row_index = iFullRowIndex; row_index > 0; row_index--)
	{
		for(column_index = 0; column_index < GAMEAREAWIDTH; column_index++)
		{
			// If current block is static shift it down one.
/*			if(g_pGameGrid->GetGridStatus(column_index, row_index) == STATIC)
			{
				
				// If its the very bottom row dont draw blocks. They'll be out of the screen. Else shift it down one.
				if(row_index != GAMEAREAHEIGHT)
				{

				// Set the space below to static
				g_pGameGrid->SetGridStatus(column_index, row_index + 1, STATIC);

				// Get the color and image from above and set the next row down to the same
				g_pGameGrid->SetGridSDLImage(column_index, row_index + 1, 
				g_pGameGrid->GetGridSDLClipRect(column_index, row_index), 
				g_pGameGrid->GetGridSDLImage(column_index, row_index));	

				// Clear its original location
				g_pGameGrid->SetGridStatus(column_index, row_index, CLEAR);
				g_pGameGrid->SetGridSDLImage(column_index, row_index, NULL, NULL);
				}
			}
*/		}	
	}
	// Increment score
	m_GameScore += 100;
	
	return 0;
}

// Checks if piece colides in given m_direction. Returns true or flase.
bool CGameArea::collide( int m_direction ){

	unsigned int iWidthIndex = 0, iHeightIndex = 0;
	int curPos_x, curPos_y = 0;
	int currentBlockPiece = 0;

	bool collide = false;

	//Save current positions
	curPos_x = currentPiece->getPosX();
	curPos_y = currentPiece->getPosY();

	// Modify according to m_direction
	switch(m_direction)
	{
		case LEFT:
			currentPiece->setPosX( currentPiece->getPosX() - 1);
			break;
		case RIGHT: 
			currentPiece->setPosX( currentPiece->getPosX() + 1);
			break;
		case DOWN:
			currentPiece->setPosY( currentPiece->getPosY() + 1 );
		default:
			break;
	}
	
	// Loop through piece 
	// Check specific block
	for( iHeightIndex = 0; iHeightIndex < currentPiece->getHeight(); iHeightIndex++ )
	{
		// Iterate from left to right
		for( iWidthIndex = 0;  iWidthIndex < currentPiece->getWidth(); iWidthIndex++ )
			{
				if( ( currentPiece->getPieceData( currentBlockPiece ) == '1' )
				&&( gameGrid->getTileFlag( currentPiece->getPosX() + currentPiece->getWidth(), 
						currentPiece->getPosY() + currentPiece->getHeight() ) > 100 ))
				{
					collide = true;	
				}
				currentBlockPiece++;
			}
		
	}
	//Return values to original 
	currentPiece->setPosX( curPos_x );
	currentPiece->setPosY( curPos_y );

	if( collide == true )
	{
		return true;
	}
	return false;
}

