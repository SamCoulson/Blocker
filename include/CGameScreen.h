#ifndef INC_CGAMEAREA_H
#define INC_CGAMEAREA_H

#include "../gamelibrary/include/CSDLGraphics.h"
#include "../gamelibrary/include/CGrid.h"
#include "../gamelibrary/include/IScreen.h"
#include "../include/CGamePiece.h"
#include "../include/CGlobalGameData.h"

// Should be a width of 10, least 22 with anything above 20 hidden
// according to http://tetris.wikia.com/wiki/Tetris_Guideline

#define GAMEAREAWIDTH 12
#define GAMEAREAHEIGHT 22 
#define GAMEAREA_TILE_SIZE 20

#define NEXTPIECEWIDTH 6
#define NEXTPIECEHEIGHT 6 

#define RIGHT 10
#define LEFT 11
#define DOWN 12
#define ROTATE 13

using namespace std;
// Could put a method in Grid for clear grid then inherit this class from grid and add the clear grid method for this.

class CGameArea : public IScreen{
public:
	CGameArea( CSDLGraphics& graphics, CGlobalGameData& gameData );
	~CGameArea();

	bool init();

	void processEvents( SDL_Event *event );
	void update();
	void render();

	bool requestQuit(){ return quit; }

	bool isPaused(){ return paused; }

	void cleanUp(){};	

	void clearGrid( CGrid* );		
	
private:

	CSDLGraphics *graphics;
	CGlobalGameData *gameData;

	// Rectangles areas for each block
	SDL_Rect redTile, blueTile, greenTile, purpleTile, yellowTile;

	// Logical grids for main game area and next piece
	CGrid *gameGrid;
	CGrid *nextPieceGrid;

	// Surfaces for visual elements
	SDL_Surface *gameframe;
	SDL_Surface *gametile;
	SDL_Surface *gameoverframe;
	SDL_Surface *pausedframe;

	string fontFile;
	// Game states	
	bool quit;
	bool paused;
	bool gameOver;
	unsigned int gameSpeed;

	// Popinters to current and next game pieces	
	CGamePiece* currentPiece;
	CGamePiece* nextPiece; 

	// Keyboard flags
	unsigned int keyPressed;	

	// Char buffer for score output
	char score[10];
	char level[2];
	char lines[6];

	// Utility function to output int numbers to strings for rendering 
	const char* GetScore();
	const char* GetLevel();
	const char* GetLines();

	// Score counter
	int gameScore;

	// Level counter
	int gameLevel;

	// Lines counter
	unsigned int gameLines;
        unsigned int levelLines;	

	int elapsedTime;

	// Methods used in game logic
	bool collide();
	void isFullLine();
	void removeLine( int iFullRowIndex );
	void checkAndSetLevel();
	bool isGameOver();

	void putPiece( CGamePiece*, CGrid* grid, bool staticMode );
	void renderGrid( CGrid* );
};

#endif // INC_CGAMEAREA_H
