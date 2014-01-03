#ifndef INC_INTROSCREEN_H
#define INC_INTROSCREEN_H

#include "../include/CSDLGraphics.h"
#include "../include/IScreen.h"

class CIntroScreen : public IScreen{
	public:
		CIntroScreen( CSDLGraphics& );
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

		int introTextColour;

		bool quit;

};

#endif // INC_INTROSCREEN_H 
