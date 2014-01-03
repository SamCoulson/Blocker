#ifndef H_CSCREENBASE_INC
#define H_CSCREENBASE_INC

#include "CSDLGraphics.h"

class IScreen{
	public:
		IScreen( CSDLGraphics& ){};
		virtual ~IScreen(){};

		// Initilaise the sprites and values for the screen
		virtual bool init() = 0;

		// Free up any memory and reset values
		virtual void cleanUp() = 0;

		// Called to update the (model) data, or screen objects, also does collisions.
		virtual void update() = 0;

		// Render the sprites on the screen
		virtual void render() = 0;

		// Process any events i.e. input
		virtual void processEvents( SDL_Event* ) = 0;

		// Notify caller that an exit has been requested
		virtual bool requestQuit() = 0;

		// Notify caller that the scren should be paused
		virtual bool isPaused() = 0;
		
	private:

};

#endif // H_CSCREENBASE_INC
