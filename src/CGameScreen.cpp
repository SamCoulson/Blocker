#include "../include/CGameScreen.h"

using namespace std;

CGameArea::CGameArea( CSDLGraphics& graphics, CGlobalGameData& gameData ) : IScreen( graphics, gameData ){

	this->graphics = &graphics;
	this->gameData = &gameData;

	// Define all block cooridates in image file	
	redTile.x = 40;
	redTile.y = 0;
	redTile.w = 20;
	redTile.h = 20;
	
	blueTile.x = 20;
	blueTile.y = 0;
	blueTile.w = 20;
	blueTile.h = 20;
	
	greenTile.x = 60;
	greenTile.y = 0;
	greenTile.w = 20;
	greenTile.h = 20;
	
	purpleTile.x = 80;
	purpleTile.y = 0;
	purpleTile.w = 20;
	purpleTile.h = 20;
	
	yellowTile.x = 0;
	yellowTile.y = 0;
	yellowTile.w = 20;
	yellowTile.h = 20;

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
	if( gameGrid == NULL ){
		std::cout << "Cannot create GameGrid Object" << std::endl;
		return false;
	}

	nextPieceGrid = new CGrid( NEXTPIECEWIDTH, NEXTPIECEHEIGHT, GAMEAREA_TILE_SIZE, GAMEAREA_TILE_SIZE, 260, 310 ); 
	if( nextPieceGrid == NULL ){
		std::cout << "Cannot create nextPieceGrid Object" << std::endl;
		return false;
	}	
	
	// Load in bitmaps for game areas
	gameframe = graphics->loadImageFromFile( "./images/GameFrameRounded.bmp", 255, 0, 255 );
	
	// Loadsprite sheet *add error checking*
	gametile = graphics->loadImageFromFile("./images/TileSet.bmp", 255, 0, 255 );

	gameoverframe = graphics->loadImageFromFile( "./images/GameoverFrame00.bmp", 255, 0, 255 );	

	// Font file name
	fontFile = "FreeSans.ttf";

	// Spawn first piece
	currentPiece = new CGamePiece( 4, 0 );	

	// Spawn next piece
	nextPiece = new CGamePiece( 2, 1 );

	// Init game states
	quit = false;
	paused = false;

	// Init game metrics
	gameScore = 0;
	gameSpeed = 300;  // Specified in ms
	gameLevel = 0;
	gameLines = 0;
	levelLines = 10;

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

		// Clear all the non-static blocks i.e. current game piece blocks from the grid ready for the next
		// placment of blocks
		clearGrid( gameGrid );
		clearGrid( nextPieceGrid );

		// Save current position and rotation
		unsigned int posX = currentPiece->getPosX();
		unsigned int posY = currentPiece->getPosY();
		unsigned int rotation = currentPiece->getRotation();

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

				// Enable 'wall-kick' mode, this mean the piece will move to accomodate the space
				// the piece needs if the player rotates the piece next to a wall
				if( currentPiece->getPosX() + currentPiece->getWidth() > GAMEAREAWIDTH-1 ){
					currentPiece->setPosX( (GAMEAREAWIDTH-1) - currentPiece->getWidth() );
				}

				// If block has rotated into static blcks then disallow the rotation
				if( collide() ){
					currentPiece->setRotation( rotation );
				}

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

		// Put current piece into the current game area
		putPiece( currentPiece, gameGrid, staticMode );

		// If in static mode then block is being drawn into grid which must mean a new piece is needed.	
		// Spawn new block
		if( staticMode == true ){
			// Remove current piece
			delete currentPiece;

			// Set the current piece to the next piece
			currentPiece = nextPiece;
			currentPiece->setPosX(4);
			currentPiece->setPosY(0);

			// Spawn a new next piece
			
			// NOTE: Need to calculate correct positioning of piece in next piece box
			// so that is appers more or less central in the box
			nextPiece = new CGamePiece( 2, 1);		
		}
		
		// Increase the speed if next level has been reached
		checkAndSetLevel();
	}

	// Put next game piece into the next piece window
	putPiece( nextPiece, nextPieceGrid, false );
			
}

void CGameArea::render(){	
	
	// Clear screen
	graphics->clearScreen( 0, 0, 0 );
	

	// Draw game area with player piece and next piece
	renderGrid( gameGrid );
	renderGrid( nextPieceGrid );
	
	// Draw game frame	
	graphics->draw( 0 ,0 , gameframe, SDL_GetVideoSurface(), NULL );	

	// Draw the game over window if the player has lost *Must be drawn before text, could be a bug* 	
	if( gameOver == true ){
		graphics->draw( 5, 100, gameoverframe, SDL_GetVideoSurface(), NULL);
	}

	// Draw the text 'Paused' if game is paused
	if( paused == true ){
		graphics->drawText( "Paused", 80, 140, fontFile, 40, 255, 0, 0  );
	}

	//Draw score and level number 
	graphics->drawText( GetScore(), 260, 5, fontFile, 40, 255, 0, 0  );
	
	graphics->drawText( GetLevel(), 260, 75, fontFile, 40, 255, 0, 0  );	

	graphics->drawText( GetLines(), 260, 145, fontFile, 40, 255, 0, 0  );	

	// Show the screen
	// Or can use update rects
	graphics->update();
	
}

// Check the score and set the level according to how many lines have been scored.
// On progression to the next level speed up the game.
void CGameArea::checkAndSetLevel()
{

	// If the number of lines scored so far exceed what has been set for this level i.e. 10 lines per level
	// then speed up the game by a small fraction every ten lines scored
	if( gameLines > levelLines ){
		// Reduce sleep time in loop by ten ms, however anything below 50 would be too quick
		if( gameSpeed > 50 ){
			gameSpeed -= 10;
		}else{
			// Game needs to end as players has beaten the quickest level or
			// could just fix it at ten for infitnate play
		}
		
		// Increment amount of lines needed to progess to next level
		levelLines += 10;
				
		// Increment level number
		gameLevel += 1;
	}
}

// Utiltiy function to convert game score to a string ready for render
const char* CGameArea::GetScore()
{ 
	sprintf( this->score,"%05d", gameScore );
	return score;
}

// Utiltiy function to convert game level to a string render
const char* CGameArea::GetLevel()
{
	sprintf( this->level,"%02d", gameLevel );
	return level;
}

const char* CGameArea::GetLines(){
	sprintf( this->lines,"%04d", gameLines );
	return lines;
}

// Test if the player has lost the game
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

// Resets all logical values for grid blocks that are the player piece to clear.  Ready for next frame. 
void CGameArea::clearGrid( CGrid* grid ){
	
	for( int columnIndex = 0; columnIndex < grid->getHeight(); columnIndex++ ){

		for( int rowIndex = 0; rowIndex < grid->getWidth(); rowIndex++ ){

			if( grid->getTileFlag( rowIndex, columnIndex) < STATIC_TILE ){
				grid->setTileFlag( rowIndex, columnIndex, CLEAR );
			}
		}
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
		if( ( gameGrid->getTileFlag( 1, row_index ) > STATIC_TILE ) && 
		( gameGrid->getTileFlag( 2, row_index ) > STATIC_TILE ) &&
		( gameGrid->getTileFlag( 3, row_index ) > STATIC_TILE ) &&
		( gameGrid->getTileFlag( 4, row_index ) > STATIC_TILE ) &&
		( gameGrid->getTileFlag( 5, row_index ) > STATIC_TILE ) &&
		( gameGrid->getTileFlag( 6, row_index ) > STATIC_TILE ) &&
		( gameGrid->getTileFlag( 7, row_index ) > STATIC_TILE ) &&
		( gameGrid->getTileFlag( 8, row_index ) > STATIC_TILE ) &&
		( gameGrid->getTileFlag( 9, row_index ) > STATIC_TILE ) &&
		( gameGrid->getTileFlag( 10, row_index ) > STATIC_TILE ) ){
			// Increment score
			gameScore += 50;
			// increment line counter
			gameLines++;		
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

	// Move all static blocks above the deleted line down one by iterating up through rows and setting
	// all static pieces to plus 1 on the y coord
	for( row_index = iFullRowIndex; row_index > 1; row_index-- )
	{
		for( column_index = 1; column_index < GAMEAREAWIDTH-1; column_index++ )
		{
			// If current block is static shift it down one.
			if( gameGrid->getTileFlag( column_index, row_index ) > STATIC_TILE )
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
						currentPiece->getPosY() + iHeightIndex ) > STATIC_TILE ))
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

void CGameArea::putPiece( CGamePiece* piece, CGrid* grid, bool staticMode ){
	// Prepare the block to be set into the game area as static or not
	unsigned int blockType = 0;
	if( staticMode ){
		blockType = piece->getColor() + 100;	
	}else{
		blockType = piece->getColor();	
	}	

	unsigned int currentBlockPiece = 0;		

	// Iterate through the current block data and set those block for drawing into the grid that are visible
	for( int rowIndex = 0; rowIndex < piece->getHeight(); rowIndex++)
	{
		for( int columnIndex = 0; columnIndex < piece->getWidth(); columnIndex++)
		{
			if( piece->getPieceData( currentBlockPiece ) == '1')
			{
				grid->setTileFlag( piece->getPosX() + columnIndex, 
						piece->getPosY() + rowIndex, blockType );
			}
		// Next block piece in block array
		currentBlockPiece++;
		}
	}	
}

void CGameArea::renderGrid( CGrid* grid ){
	// Draw game area with player piece
	for( int i = 0; i < grid->getWidth(); i++ ){
		for( int j = 0; j < grid->getHeight(); j++ ){
			switch( grid->getTileFlag( i, j) ){
			case RED:
			case STATIC_RED:					
				graphics->draw( grid->getXY(i, j)->x, grid->getXY( i, j)->y, 
						gametile, SDL_GetVideoSurface(), &redTile );	
				break;
			case BLUE:
			case STATIC_BLUE:
				graphics->draw( grid->getXY(i, j)->x, grid->getXY( i, j)->y, 
						gametile, SDL_GetVideoSurface(), &blueTile );
				break;
			case GREEN:
			case STATIC_GREEN:
				graphics->draw( grid->getXY(i, j)->x, grid->getXY( i, j)->y, 
						gametile, SDL_GetVideoSurface(), &greenTile );
				break;
			case PURPLE:
			case STATIC_PURPLE:
				graphics->draw( grid->getXY(i, j)->x, grid->getXY( i, j)->y, 
						gametile, SDL_GetVideoSurface(), &purpleTile );
				break;
			case YELLOW:
			case STATIC_YELLOW:
				graphics->draw( grid->getXY(i, j)->x, grid->getXY( i, j)->y, 
						gametile, SDL_GetVideoSurface(), &yellowTile );
				break;
			default:
				break;
			}
		}
	}	
}
