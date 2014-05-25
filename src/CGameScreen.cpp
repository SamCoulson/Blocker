#include "../include/CGameScreen.h"

using namespace std;

CGameArea::CGameArea( CSDLGraphics& graphics, CGlobalGameData& gameData ) : IScreen( graphics, gameData ){

	this->graphics = &graphics;
	this->gameData = &gameData;	
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
	gameGrid = new CGrid( GAMEAREAWIDTH, GAMEAREAHEIGHT, GAMEAREA_TILE_SIZE, GAMEAREA_TILE_SIZE, 0, 0 ); 
	if( gameGrid == NULL )
	{
		std::cout << "Cannot create GameGrid Object" << std::endl;
		return false;
	}	
	
	// Load in bitmaps for game areas
	gameframe = graphics->loadImageFromFile( "./images/GameFrameRounded.bmp", 255, 0, 255 );
	
	// Loadsprite sheet *add error checking*
	gametile = graphics->loadImageFromFile("./images/TileSet.bmp", 255, 0, 255 );

	gameoverframe = graphics->loadImageFromFile( "./images/GameoverFrame00.bmp", 255, 0, 255 );	
	
	// Spawn first piece
	currentPiece = new CGamePiece();	

	quit = false;

	paused = false;

	gameScore = 4000;
	gameSpeed = 300;  // Specified in ms
	gameLevel = 0;
	keyPressed = 0;

	gameOver = false;

	memset( score, 0, sizeof( score ) );

	return true;
}

void CGameArea::processEvents( SDL_Event *event )
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
				keyPressed = DOWN;
			break;
			case SDLK_ESCAPE:
				quit = true;
			break;
			case SDLK_F11:
				if( paused == true )
				{
					paused = false;
				}
				else
				{
					paused = true;
				}
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
	
	if( isGameOver() ){
		paused = true;
		gameData->setScore( gameScore );	
		gameOver = true;
		quit = true;
	}else if( isPaused() ){
		paused = true;	
	}else{

		// Set all ISGAMEBLOCK in logical game grid to CLEAR.  This clears the grid of the old curgamepiece position, else it will be drawn also.
		// Also clears the next piece box.
		bool staticMode = false;

		ClearGrid();

		// Save current position and rotation
		unsigned int posX = currentPiece->getPosX();
		unsigned int posY = currentPiece->getPosY();

		// Detect full lines and later the game area as necessary before moving the current piece
		isFullLine();

		// Move as dictated by control or game
		// Check for collision if collide then reset position and set as static

		// Move piece according to input first
		switch( keyPressed ){
			case RIGHT:
				currentPiece->setPosX( currentPiece->getPosX()+1 );
				break;
			case LEFT:
				currentPiece->setPosX( currentPiece->getPosX()-1 );		
				break;
			case ROTATE:
				currentPiece->RotatePiece();
				break;
			case DOWN:
				currentPiece->setPosY( currentPiece->getPosY()+1 );
				break;
			default:
				// Automatically move piece down according to game speed
				if( ( SDL_GetTicks() - elapsedTime ) > gameSpeed ){
					// Progesss down at a rate according to game speed.	
					currentPiece->setPosY( currentPiece->getPosY()+1 );
					// Store elpased in order to be able to make a comparison next time at this point
					elapsedTime = SDL_GetTicks();			
				}
			break;
		}
		keyPressed = 0; // Reset keypressed	
	
		// Collision detection for sides	
		if( ( ( currentPiece->getPosX() + currentPiece->getWidth() ) > GAMEAREAWIDTH-1 ) ||
		( currentPiece->getPosX() < 1 ) || collide() ){
			// Collided restore old position	
			currentPiece->setPosX( posX );
		}	
		
		// Collision detection for bottom of area and other blocks	
		if( ( ( currentPiece->getPosY() + currentPiece->getHeight() ) > GAMEAREAHEIGHT-1 ) || collide() ) {
			currentPiece->setPosY( posY );
			staticMode = true;
		}

		// Prepare the block to be set into the game area as static or not
		unsigned int blockType = 0;
		if( staticMode ){
			blockType = currentPiece->getColor() + 100;	
		}else{
			blockType = currentPiece->getColor();	
		}

		unsigned int current_block_piece = 0;		

		// Iterate through the current block data and set those block for drawing into the grid that are visible
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

		// If in static mode then block is being drawn into grid which must mean a new piece is needed.	
		// Spawn new block
		if( staticMode == true ){
			delete currentPiece;
			currentPiece = new CGamePiece();	
		}
		
		// Increase the speed if next level has been reached
		checkAndSetLevel();
	}
	// Put pre-piece
	// CPlayerPiece.PutPiece(CPlayerPiece.pPrePiece, 17, 23, ISBLOCKPIECE, g_pScreenGrid);

	// Reset pre-piece area *doesnt work properly*
	// g_pScreenGrid->SetSDLGridBitmapRepeated(g_pScreenGrid->GetSDLGridXY(15, 11).x, g_pScreenGrid->GetSDLGridXY(15, 11).y, 
	// g_pScreenGrid->GetSDLGridXY(20, 17).x, g_pScreenGrid->GetSDLGridXY(20, 17).y, NULL, NULL);
		
}

void CGameArea::render(){

	// NOTE: Could make this render according to states	
	
	// Clear screen
	graphics->clearScreen( 0, 0, 0 );
		
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

	// Draw game frame	
	graphics->draw( 0 ,0 , gameframe, SDL_GetVideoSurface(), NULL );	

	// Draw the game over window if the player has lost *Must be drawn before text, could be a bug* 	
	if( gameOver == true ){
		graphics->draw( 5, 100, gameoverframe, SDL_GetVideoSurface(), NULL);
	}

	if( paused == true ){
		graphics->drawText( "Paused", 80, 140, "tunga.ttf", 255, 0, 0  );
	}

	//Draw score and level number 
	graphics->drawText( GetScore(), 270, 40, "tunga.ttf", 255, 0, 0  );
	
	graphics->drawText( GetLevel(), 270, 170, "tunga.ttf", 255, 0, 0  );	
	
	// Show the screen
	// Or can use update rects
	graphics->update();
	
}


void CGameArea::checkAndSetLevel()
{
	if( gameScore >= 1000 )
	{
		gameSpeed = 250;
		gameLevel = 1;
	}
	if( gameScore >= 2000 )
	{
		gameSpeed = 200;
		gameLevel = 2;
	}
	if( gameScore >= 3000 )
	{
		gameSpeed = 150;
		gameLevel = 3;
	}
	if( gameScore >= 4000 )
	{
		gameSpeed = 100;
		gameLevel = 4;
	}
	if( gameScore >= 5000 )
	{
		gameSpeed = 50;
		gameLevel = 5;
	}
}

const char* CGameArea::GetScore()
{ 
	sprintf( this->score,"%05d", gameScore );
	return score;
}
/*
int CGameArea::GetIntScore()
{
	return gameScore;
}
*/
const char* CGameArea::GetLevel()
{
	sprintf( this->level,"%02d", gameLevel );
	return level;
}

bool CGameArea::isGameOver()
{
	// Scan the top line	
	for( int widthindex = 1; widthindex < GAMEAREAWIDTH-1; widthindex++ ){
		// If there is a static block in this row then game over
		if( gameGrid->getTileFlag( widthindex, 2 ) > 100 ){
			return true;
		}
	}

	return false;
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
void CGameArea::isFullLine()
{
	int column_index, row_index = 0;

	// Iterate from top row to bottom row
	for( row_index = 1; row_index < GAMEAREAHEIGHT - 1; row_index++ )
	{
		// Check each block in line
		if( ( gameGrid->getTileFlag( 1, row_index ) > 100 ) && 
		( gameGrid->getTileFlag( 2, row_index ) > 100 ) &&
		( gameGrid->getTileFlag( 3, row_index ) > 100 ) &&
		( gameGrid->getTileFlag( 4, row_index ) > 100 ) &&
		( gameGrid->getTileFlag( 5, row_index ) > 100 ) &&
		( gameGrid->getTileFlag( 6, row_index ) > 100 ) &&
		( gameGrid->getTileFlag( 7, row_index ) > 100 ) &&
		( gameGrid->getTileFlag( 8, row_index ) > 100 ) &&
		( gameGrid->getTileFlag( 9, row_index ) > 100 ) &&
		( gameGrid->getTileFlag( 10, row_index ) > 100 ) ){
			// Increment score
			gameScore += 50;		
			//  Remove line and move all blocks above down one
			removeLine( row_index );
		}
	}
}

// Deletes the row and moves all the rows above down one and increase score.
void CGameArea::removeLine( int iFullRowIndex )
{
	int row_index, column_index = 0;

	// First delete the row
	for(column_index = 0; column_index < GAMEAREAWIDTH; column_index++){
		gameGrid->setTileFlag( column_index, iFullRowIndex, CLEAR );	
	}

	column_index = 0;

	// Iterate up through rows and set all static pieces to plus 1 on the y coord
	for( row_index = iFullRowIndex; row_index > 1; row_index-- )
	{
		for( column_index = 1; column_index < GAMEAREAWIDTH-1; column_index++ )
		{
			// If current block is static shift it down one.
			if( gameGrid->getTileFlag( column_index, row_index ) > 100 )
			{
				// If its the very bottom row just set as clear, else shift it down one.
				if( row_index != GAMEAREAHEIGHT-1 ){

					// Set the space below to static
					gameGrid->setTileFlag( column_index, row_index + 1, 
							gameGrid->getTileFlag( column_index, row_index ) );
	
					// Clear its original location
					gameGrid->setTileFlag( column_index, row_index, CLEAR );
					
				}
			}
		}	
	}
	
}

// Checks if piece colides with any static blocks in the game area
bool CGameArea::collide(){

	unsigned int iWidthIndex = 0, iHeightIndex = 0;	
	int currentBlockPiece = 0;

	bool collide = false;
	
	// Loop through piece 
	// Check specific block
	for( iHeightIndex = 0; iHeightIndex < currentPiece->getHeight(); iHeightIndex++ )
	{
		// Iterate from left to right
		for( iWidthIndex = 0;  iWidthIndex < currentPiece->getWidth(); iWidthIndex++ )
			{
				if( ( currentPiece->getPieceData( currentBlockPiece ) == '1' )
				&&( gameGrid->getTileFlag( currentPiece->getPosX() + iWidthIndex, 
						currentPiece->getPosY() + iHeightIndex ) > 100 ))
				{
					collide = true;	
				}
				currentBlockPiece++;
			}
		
	}
	
	if( collide == true )
	{
		return true;
	}
	return false;
}

