#ifndef INC_INTROSCREEN_H
#define INC_INTROSCREEN_H

#include "../gamelibrary/include/CSDLGraphics.h"
#include "../gamelibrary/include/IScreen.h"
#include "../include/CGlobalGameData.h"

class CIntroScreen : public IScreen{
	public:
		CIntroScreen( CSDLGraphics&, CGlobalGameData& );
		~CIntroScreen();

		bool init();

		void processEvents( SDL_Event *event );
		void update();
		void render();
		
		bool requestQuit(){ return quit; }
		bool isPaused(){ return false; }

		void cleanUp(){};

	private:
		
		CSDLGraphics *graphics;
		SDL_Surface *introscreenframe;

		std::string fontFile;

		int introTextColour;

		bool quit;

};

#endif // INC_INTROSCREEN_H 
