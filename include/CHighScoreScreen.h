#ifndef INC_CHIGHSCORESCREEN_H
#define INC_CHIGHSCORESCREEN_H

#include "../include/CSDLGraphics.h"
#include "../include/IScreen.h"

class CHighScoreScreen : public IScreen{

	public:
		CHighScoreScreen( CSDLGraphics& graphics );
		~CHighScoreScreen(){};

		bool init();
		void processEvents( SDL_Event* );
		void update();
		void render();
		void cleanUp(){};

		bool requestQuit(){};
		bool isPaused(){};

	private:
		
		CSDLGraphics* graphics;

		SDL_Surface *gameoverframe;
		SDL_Surface *topscoreframe;

		void loadTopScoresFromFile();

		typedef struct _SCOREDATA
			{
				std::string scorename;
				int score;
			}scoredata;

		// Array data for top scores
		scoredata topscores[10];


};

#endif // INC_CHIGHSCORESCREEN_H 
