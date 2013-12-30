#ifndef INC_INTROSCREEN_H
#define INC_INTROSCREEN_H

#include "../include/CSDLGraphics.h"

class CIntroScreen{
	public:
		CIntroScreen( CSDLGraphics *graphics );
		~CIntroScreen();

		bool init();

		void input( SDL_Event *event );
		void update();
		void render();
		
		bool isQuit(){ return quit; }

	private:
		
		CSDLGraphics *graphics;
		SDL_Surface *introscreenframe;

		int introTextColour;

		bool quit;

};

#endif // INC_INTROSCREEN_H 
