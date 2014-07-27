#include "../include/CIntroScreen.h"

CIntroScreen::CIntroScreen( CSDLGraphics& graphics, CGlobalGameData& globalGameData ) : IScreen( graphics, globalGameData ){
	
	this->graphics = &graphics;	
	introscreenframe = NULL;

}

CIntroScreen::~CIntroScreen(){
	if( introscreenframe != NULL ){
		delete introscreenframe;
	}
}
bool CIntroScreen::init(){
	
	introscreenframe = graphics->loadImageFromFile( "./images/IntroScreenFrame01.bmp", 255, 0, 255 );

	introTextColour = 1;
	fontFile = "FreeSans.ttf";

	quit = false;
}

void CIntroScreen::processEvents( SDL_Event *event ){
	
	if( event->type == SDL_KEYDOWN)
	{
		switch(event->key.keysym.sym)
		{
			case SDLK_SPACE:
				quit = true;
			break;
			case SDLK_DOWN:	
			break;
			case SDLK_ESCAPE:
				quit = true;
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

void CIntroScreen::update(){

}

void CIntroScreen::render(){

	// Clear screen
	graphics->clearScreen( 0, 0, 0 );

	// Draw game frame	
	graphics->draw( 0, 0, introscreenframe, SDL_GetVideoSurface(), NULL);
	
	graphics->drawText( "Press space to start", 70, 300, fontFile, 30, 255, introTextColour, introTextColour );
	
	if(introTextColour == 255)
	{
		introTextColour = 0;
	}
	else
	{
		introTextColour +=1;
	}
	
	graphics->update(); // Should actually happen in top level loop

}

