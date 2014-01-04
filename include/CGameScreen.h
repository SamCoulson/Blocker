#ifndef INC_CGAMEAREA_H
#define INC_CGAMEAREA_H

#include "../include/CSDLGraphics.h"
#include "../include/CGrid.h"
#include "../include/CGamePiece.h"
#include "../include/IScreen.h"

// Should be a width of 10, least 22 with anoything aobve 20 hidden
// according to http://tetris.wikia.com/wiki/Tetris_Guideline

#define GAMEAREAWIDTH 12
#define GAMEAREAHEIGHT 22 
#define GAMEAREA_TILE_SIZE 20

#define RIGHT 10
#define LEFT 11
#define DOWN 12
#define ROTATE 13

using namespace std;
// Could put a method in Grid for clear grid then inherit this class from grid and add the clear grid method for this.

class CGameArea : public IScreen{
public:
	CGameArea( CSDLGraphics& graphics );
	~CGameArea();

	bool init();

	void processEvents( SDL_Event *event );
	void update();
	void render();

	bool requestQuit(){ return quit; }

	bool isPaused(){ return paused; }

	void cleanUp(){};	

	void ClearGrid();		

	
private:

	CSDLGraphics *graphics;
	CGrid *gameGrid;
	
	SDL_Surface *gameframe;
	SDL_Surface *gametile;
	SDL_Surface *gameoverframe;
	
	bool quit;
	bool paused;
	
	CGamePiece* currentPiece;

	unsigned int keyPressed;

	unsigned int gameSpeed;

	// Char buffer for score output
	char score[10];
	char level[2];

	const char* GetScore();

	int GetIntScore();	

	const char* GetLevel();
	
	bool gameOver;

	// Score storage
	int gameScore;
	int gameLevel;

	int elapsedTime;

	bool collide();
	void isFullLine();
	void removeLine( int iFullRowIndex );
	void checkAndSetLevel();
	bool isGameOver();
};

#endif // INC_CGAMEAREA_H
