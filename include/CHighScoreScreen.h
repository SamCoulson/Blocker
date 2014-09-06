#ifndef INC_CHIGHSCORESCREEN_H
#define INC_CHIGHSCORESCREEN_H
#include <vector>
#include <sstream>
#include "../gamelibrary/include/CSDLGraphics.h"
#include "../gamelibrary/include/IScreen.h"
#include "../include/CGlobalGameData.h"

class CHighScoreScreen : public IScreen{

	public:
		CHighScoreScreen( CSDLGraphics& graphics, CGlobalGameData& gameData );
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
		CGlobalGameData* gameData;
	
		SDL_Surface *topscoreframe;

		std::string fontFile;

		bool quit;

		bool inEditMode;

		bool nameComplete;

		std::stringstream newName;	

		typedef struct _SCOREDATA
		{
			std::string scorename;
			int score;
			bool isNew;
		}scoredata;

		scoredata topScore;

		// Vector for top scores
		std::vector<scoredata> topScores;
		
		void loadTopScoresFromFile();
		void saveTopScoresToFile();

		// Sorting predicate function used in sort, note: function must be static
		static bool scorecmp( const scoredata& left, const scoredata& right );
};

#endif // INC_CHIGHSCORESCREEN_H 
