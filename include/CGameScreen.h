#ifndef INC_CGAMEAREA_H
#define INC_CGAMEAREA_H

#include "../include/CSDLGraphics.h"
#include "../include/CGrid.h"
#include "../include/CGamePiece.h"
#include "../include/IScreen.h"

// Should be a width of 10, least 22 with anoything aobve 20 hidden
// according to http://tetris.wikia.com/wiki/Tetris_Guideline

#define GAMEAREAWIDTH 10
#define GAMEAREAHEIGHT 20 
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

	unsigned int m_GameSpeed;

	void ClearGrid();
	int IsFullLine();
	int OnLineFull(int iFullRowIndex);
	int IsGameOver();
	const char* GetScore();
	int GetIntScore();
	void CheckandSetLevel();
	const char* GetLevel();

private:

	CSDLGraphics *graphics;
	CGrid *gameGrid;
	
	SDL_Surface *gameframe;
	SDL_Surface *gametile;
	
	bool quit;
	bool paused;
	
	CGamePiece* currentPiece;

	unsigned int keyPressed;

	// Char buffer for score output
	char score[10];
	char level[2];
	// Write numerical score to char buffer
	const char* ConvertScoretoChar();
	// Score storage
	int m_GameScore;
	int m_GameLevel;

	bool collide( int m_direction );

};

#endif // INC_CGAMEAREA_H
