#ifndef INC_CHIGHSCORESCREEN_H
#define INC_CHIGHSCORESCREEN_H

#include "../include/CSDLGraphics.h"
#include "../include/IScreen.h"
#include <vector>

class CHighScoreScreen : public IScreen{

	public:
		CHighScoreScreen( CSDLGraphics& graphics );
		~CHighScoreScreen(){};

		bool init();
		void processEvents( SDL_Event* );
		void update();
		void render();
		void cleanUp(){};

		bool requestQuit(){ return quit; }
		bool isPaused(){};

	private:
		
		CSDLGraphics* graphics;
	
		SDL_Surface *topscoreframe;

		bool quit;	

		typedef struct _SCOREDATA
		{
			std::string scorename;
			int score;
		}scoredata;

		scoredata topScore;

		// Vector for top scores
		std::vector<scoredata> topScores;
		
		void loadTopScoresFromFile();

		// Sorting predicate function used in sort, note: function must be static
		static bool scorecmp( const scoredata& left, const scoredata& right );
};

#endif // INC_CHIGHSCORESCREEN_H 
