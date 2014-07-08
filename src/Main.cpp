#include "../include/main.h" // All SDL apps need
#include <vector>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 440
#define SCREEN_BPP    32

#define FPS 60

int main( int argc, char* args[])
{
	// Global objects
	CSDLGraphics sdlgraphics;
	CSDLGraphics* graphics;
	
	// SDL event structure
	SDL_Event event; 		
	
	bool gameQuit = false;
	DWORD startTime;

	// Launch graphics sub system
	graphics = &sdlgraphics;

	// Initialiase SDL
	if( graphics->initialise( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE, "Blocker" ) != true )
	{
			cout << "Failed to initialiase SDL" << endl; // Can add |SDL_FULLSCREEN which actaully works!
			return 1;
	}

	// Create global game object to store game data
	CGlobalGameData *gameData = new CGlobalGameData();

	// Create all the screens	
	IScreen *introScreen = new CIntroScreen( *graphics, *gameData );
	IScreen *gameScreen = new CGameArea( *graphics, *gameData );
	IScreen *highScoreScreen = new CHighScoreScreen( *graphics, *gameData );

	// Hold references to all the screens
	std::vector< IScreen* > screens;
	std::vector< IScreen* >::iterator it;
	

	// Add the screens in order
	screens.push_back( introScreen );
	screens.push_back( gameScreen );
	screens.push_back( highScoreScreen );
	
	IScreen* currentScreen = NULL;
	bool isInitialised = false;
	startTime = SDL_GetTicks();

	// Load up the last screen in the screens vector, the introscreen.
	it = screens.begin();
	currentScreen = (*it);

	
	// Game loop
	while( gameQuit != true)
	{
		// The the screen has not been initialised then load it up.
		if( isInitialised == false ){
			if( currentScreen->init() == false ){
				cout << "Failed to initialise screen!" << endl;
				gameQuit = true;
			}	
			isInitialised = true;	
		}	

		// Grab keyboard and system events
		SDL_PollEvent( &event );
	
		// Get the time at the beginning of the frame	
		startTime = SDL_GetTicks();
		
		currentScreen->processEvents( &event );
		currentScreen->update();
		currentScreen->render();
		
		// Check how much time has been taken to render and update.
		// If still inside the alloted time per frame i.e. 60 / 1000 = 16.66ms per frame
		// calculate remaining time and sleep for this time.
		if( ( SDL_GetTicks() - startTime ) < 1000 / FPS ){
			DWORD endTime = startTime + 1000 / FPS;
			SDL_Delay( endTime - SDL_GetTicks() );
		}

		// On a screen quitting check what conditions it quits under
		if( ( currentScreen->requestQuit() ) && ( event.key.keysym.sym == SDLK_ESCAPE ) ){
			gameQuit = true;
		}else if( currentScreen->requestQuit() ){
			// Progress vector pointer to next screen
			++it;

			// If last screen go back to intro screen
			if( it == screens.end() ){
				it = screens.begin();
			}

			// Make current screen the screen pointed to in vector
			currentScreen = (*it);

			// Make sure screen is initialised or re-initialised
			isInitialised = false;	
		}
	}
			
	SDL_Quit();

	return 0;
}




