//////////////////////////////////////////////////////////////////////////////////////////////
//
// CSDLGraphics - SDL Graphics object for setting up window, shutdown and displaying graphics
//
//////////////////////////////////////////////////////////////////////////////////////////////
#include "../include/CSDLGraphics.h"
#include <iostream>
#include <string.h>

using namespace std;

// Constructor 
CSDLGraphics::CSDLGraphics()
{
	screenSurface = NULL;
	
}

CSDLGraphics::~CSDLGraphics()
{
	
}

// Initialises the SDL library with the given arguments in the parameter list
bool CSDLGraphics::initialise( int screenWidth, int screenHeight, int screenBPP, Uint32 surfaceType, const char* titleCaption) 
{
	// Structures and macro for compile time and run time versions 
	SDL_version compver;
	SDL_VERSION(&compver);

	// Get linker version information
	const SDL_version* runver = SDL_Linked_Version();

	// Output version information to stdout
	cout << "SDL Compile-time version:" << (int)compver.major << "." << (int)compver.minor << "." << (int)compver.patch << endl;
	cout << "SDL Runtime version:" << (int)runver->major << "." << (int)runver->minor << "." << (int)runver->patch << endl;

	// Start SDL for VIDEO
	if(SDL_Init( SDL_INIT_VIDEO ) < 0){
		// Send some errors to stderr
		cout << "Failed to initialiase SDL subsystem" << endl;
		cerr << SDL_GetError() << endl;
		return false;
	}

	if( SDL_WasInit( SDL_INIT_VIDEO )){
		cout << "Video subsystem initialised"<< endl;
	}
	
	// Load the font library 
	if( TTF_Init() == -1)
	{
		cout << "TTF_Init() Failed" << endl;
		return false;
	}

	// Set the video mode
	// surfaceType can be a set of OR'd flags not bitwise but logical OR *Need to fix the logout put currently just puts 0. 
	if( !setVideoMode( screenWidth, screenHeight, screenBPP, surfaceType ) ){
		cout << "CSDL Failed to set video mode" << endl;
		return false;
	}
	
	// Set the caption for the window title
	if( titleCaption == NULL){
		// If no caption is specified set window caption to default name 
		setWindowCaption("SDL Default Caption");
	}else{
		// Set window caption to default name 
		setWindowCaption(titleCaption);
	}

	// Everything set up successfully
	screenSurface = SDL_GetVideoSurface();
	return true;
}

// shutdown - closes any systems running and outputs to log
void CSDLGraphics::shutdown(){	

	// Shutdown the font system.
	cout << "Shutting down font system" << endl;
	TTF_Quit();

	// SDL Quit
	cout << "Shutting down SDL"<< endl;
	SDL_Quit();
}

// OutputVideoModeInfo() - Outputs video info to cout. Can be called before or after the video mode is set but will contain different info  
void CSDLGraphics::outputVideoModeInfo( void ){

	char viddrvname[7];	// Buffer for name of video driver

	cout << "**************Video mode info*******************\n" << endl;

	// Get name of video driver **This can be found or included in the video info function**
	if(SDL_VideoDriverName(viddrvname, 7) == NULL)
	{
		cout << "Unable to determine video driver name\n" << endl; 
	}else{
		cout << "Driver: " << viddrvname << endl;
	}

	const SDL_VideoInfo *vidinfo = NULL; // Video capabilites

	// Get optimal video info see docs for all the settings
	vidinfo = SDL_GetVideoInfo();

	if( vidinfo != NULL){
		cout << "Available hardware video memory: " << vidinfo->video_mem <<
		"\nCurrent video resolution: " << vidinfo->current_w << " x " << vidinfo->current_h << 
		"\nHardware surfaces available: " << vidinfo->hw_available << 
		"\nCurrent video format (BitPerPixel): " << (int)vidinfo->vfmt->BitsPerPixel << 
		"\nCurrent video format (Colour Key): " << vidinfo->vfmt->colorkey << endl;
	}else{
		cout << "" << endl;
	}
}

// Returns video possible resolutions for a given video mode.  
void CSDLGraphics::outputVideoModes( void ){

	// Get available screen hardware modes, fullscreen will give a list of modes, other options many just return All resolutions possible
	// The reason for this is because any dimensions can be passed in during setup IF we want windows mode.  This will merely open a window the size of
	// the given dimensions with a front buffer of the matching size.  Fullscreen will still accept non-regular dimensions and initialise 
	// but it create a box in the middle with those dimensions. 
	// Resizing a window does not however resize the front buffer, it will just cut of the viewable area without resizing. Unless RESIZEABLE is set in which
	// case SetVideoMode is called and the new dimensions are set.
	// Changing bpp per pixel seems like a bad idea aswell, i think it may end up messing up the users desktop.
	// A better idea might be to the default as the users current settings.

	SDL_Rect **modes;					 // List of video modes

	cout << "**************Possible video modes*******************\n" << endl;

	modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);
	if( modes == (SDL_Rect**)0){
		cout << "No modes available" << endl;
		//return false;
	}else if( modes == (SDL_Rect**)-1){
		cout << "All resolutions available "<< endl;		
	}else{
		cout << "Available modes" << endl;
		for(int i=0; modes[i]; ++i){
			cout << modes[i]->w << " x " << modes[i]->h << endl;
		}
	}

}

bool CSDLGraphics::setVideoMode(int screenWidth, int screenHeight, int screenBPP, Uint32 surfaceType ){
	// Check sensible arguments have been passed
	// Is the requested screen resolution set to something 
	if( (screenHeight < 0) || (screenWidth < 0) || (screenBPP < 0) )
	{
		cout << "Invalid screen parameters passed" << endl;
		return false;
	}

	cout << "**************Current video mode*******************" << endl;

	// Check that the requested video mode is available 
	cout << "Checking requested screen mode: "<< screenWidth << " x " << screenHeight << " at " << screenBPP << " BPP" <<  endl;

	int bpp = SDL_VideoModeOK( screenWidth, screenHeight, screenBPP, surfaceType );

	if(bpp == 0){
		cout << "Requested video mode not available" << endl;
		// Could just let it go and let SDL emulate the color depth but i want know what going on
		// Alternatively add a warning to the log to say what is going on.
		return false;
	}else{
		
		// If video mode available, setup and create screen surface	
		// NOTE: screenSurface is actually a private member and so should
		// probably be named _m_screenSurface
		screenSurface = SDL_SetVideoMode( screenWidth, screenHeight, screenBPP, surfaceType );
		if(screenSurface == NULL){
			cout << "Could not set SDL video mode" << endl;
			return false;
		}

		// If HWSURFACE is not suported on the card then warn
		if( ( surfaceType & SDL_HWSURFACE ) && ( ( screenSurface->flags & SDL_HWSURFACE ) ==0 ) )
			std::cout << "**SDL_HWSURFACE requested but is not supported using SW_SURFACE**" << endl;

		// Out for startup log
		cout << "Video mode set to " << screenSurface->w << " x " << screenSurface->h << " at " << (int)screenSurface->format->BitsPerPixel << " Bpp" 
			<< " with flags ";
	
		// Determine what flags are set
		if( ( screenSurface->flags & SDL_SWSURFACE ) == 0 )
			std::cout << "SDL_SWSURFACE ";
		if( screenSurface->flags & SDL_HWSURFACE )
			std::cout << "SDL_HWSURFACE ";	
		if( screenSurface->flags & SDL_FULLSCREEN )
			std::cout << "SDL_FULLSCREEN ";
		if( screenSurface->flags & SDL_ASYNCBLIT )
			std::cout << "SDL_ASYNCBLIT ";
		if( screenSurface->flags & SDL_HWPALETTE )
			std::cout << "SDL_HWPALETTE ";
		// NEEDS OTHER FLAGS ADDING
		std::cout << endl;
	}

	return true;
}

// Set the tite name or description for the window
void CSDLGraphics::setWindowCaption( const char* windowTitle ){

	// Call the widow manager and set caption
	SDL_WM_SetCaption(windowTitle, NULL);

}

// Load and convert image to screen pixel format, as well as gets the upper left corner pixel for color key
SDL_Surface* CSDLGraphics::loadImageFromFile( std::string filename, Uint8 r, Uint8 g, Uint8 b )
{
	// Surfaces for loaded file and converted file 
	SDL_Surface *loadedImageFile = NULL;
	SDL_Surface *convertedSurface = NULL;

	// Load the image in from the file
	// NOTE: IMG_Load automatically handles color key on .PNG files.
	std::cout << "Trying to load " << filename.c_str() << endl;
	loadedImageFile = IMG_Load( filename.c_str() );
	
	// Was an image loaded in to the surface
	if(loadedImageFile == NULL)
	{
		cout << "'CSDL_Render::LoadImageFromFile' failed to load image in to SDL_Surface, ensure correct filename and path" << endl;
		return convertedSurface = NULL;
	}

	// Convert the loaded image to ** This section of the code only work if the SDL object has been initialised before hand, else SDL_DisplayFormat is unknown **
	convertedSurface = SDL_DisplayFormat( loadedImageFile );		
	if( convertedSurface == NULL )
	{
		cout << "Failed to convert SDL_Surface" << endl; 
		return convertedSurface = NULL;
	}
	else
	{
		// NOTE: Some images do not display transparency correctly may need to recreate image and save with
		// pixels correctly set to the specified color key value ( 255, 0, 255 is a common one ) 
		if( SDL_SetColorKey( convertedSurface, SDL_SRCCOLORKEY,
					SDL_MapRGB( convertedSurface->format, r, g, b ) ) != 0 )  
		{
			// Free up original surface
			SDL_FreeSurface( loadedImageFile );

			cout << "Failed to set color key" << endl;
			return convertedSurface = NULL;
			
		}else{
			// Free up original surface
			SDL_FreeSurface( loadedImageFile );

			// Return the color keyed surface
			return convertedSurface;	 
		}
		
	}

	// Free surface from loaded original file image
	SDL_FreeSurface( loadedImageFile );

	// Return uncolor keyed new image
	return convertedSurface;
}

// Returns a Uint32 color value from a given surface and positions. *Need wrap this function in surface locker (possible macro), as according
// to the docs the surface should be locked before these methods are applied*.
Uint32 CSDLGraphics::getColor(SDL_Surface * surface, int x, int y)
{
	// Find pixel format 8,16,24,32 bits-per-pxel
	int bpp = surface->format->BytesPerPixel;

	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	// bpp can be 1 byte = 8 bits, 2 bytes = 16 bits, 3 bytes = 24, 4 bytes = 32
	switch( bpp )
	{
	case 1:
		return *p;  // Return address of an Uint8 type location  
	case 2:
		return *(Uint16 *)p; // Cast p to a Uint16 pointer type
	case 3:
		// Bit shift p[0] 16 bits to the left, p[1] 8 bits to the left for SDL_LITTLE_ENDIAN
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
	case 4:
		return *(Uint32 *)p; // Cast p to a Uint32 pointer type
	default:
		return 0;
	}
}

//  Set a given pixel in a surface to a specifed colour.  *Need wrap this function in surface locker (possible macro), as according
// to the docs the surface should be locked before these methods are applied*. Only true if SDL_MUSTLOCK evaluates to 1 it 0 then can be written to any time
// Also need to be able to specify RGB not have to create Uint32 before hand.
void CSDLGraphics::putColor(SDL_Surface *surface, int x, int y, Uint32 color)
{

	int bpp = surface->format->BytesPerPixel;

	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch( bpp ) 
	{
    case 1:
        *p = color;
        break;

    case 2:
        *(Uint16 *)p = color;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
		{
            p[0] = (color >> 16) & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = color & 0xff;
        } 
		else 
		{
            p[0] = color & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = (color >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = color;
        break;
	}
}

// Wraps SDL_FillRrect
void CSDLGraphics::fillRect( unsigned int x, unsigned int y, unsigned int width, unsigned int height, 
		unsigned int r, unsigned int g, unsigned int b){

	// Convert rect to native SDL_Rect
	SDL_Rect destRect;

	destRect.w = width;
	destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	if( SDL_FillRect( screenSurface, &destRect, SDL_MapRGB( SDL_GetVideoSurface()->format, r, g, b ) ) != 0 )
	{
		std::cout << "Could not FillRect" << endl;
	}

}

// UpdateScreen swaps the back buffer to the screen
void CSDLGraphics::update()
{
	// If the screen could not be updated with flip 
	// FLip seems only to be for Hardware that supports double buffering.
	// Accroding to docs this is equivalent to calling UpdateRects(screen, 0,0,0,0)
	if(SDL_Flip(screenSurface) == -1)
	{
		cout << "SDL Render object could not update screen" << endl;
	}
}

// Fills the back buffer with the colour of the given RGB values. 
void CSDLGraphics::clearScreen( unsigned int r, unsigned int g, unsigned int b ){
	
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( SDL_GetVideoSurface()->format, r, g, b ) );
}

// Draw or puts or copies the source image to a destination surface
void CSDLGraphics::draw(int x, int y, SDL_Surface* srcimage, SDL_Surface* destSurface, SDL_Rect* srccliprect)
{
	// destination rect, used to specify where on the destination surface the image will be blitted too
	SDL_Rect destXY;

	// Take XY input and make them useable for Blit.
	destXY.x = x;
	destXY.y = y;
	destXY.w = 0; // These are ignored
	destXY.h = 0; // Can use SDL_gfx for stretching

	// Put the image to the screen or back buffer
	// Can also be drawn or put to another surface might need to do src/dest xy chekcing here
	// Need to find out what error values this returns and how to handle when it goes wrong.
	SDL_BlitSurface(srcimage, srccliprect, destSurface, &destXY);

}

void CSDLGraphics::drawText( std::string text, int pos_x, int pos_y, std::string fontName, 
			unsigned int r, unsigned int g, unsigned int b  )
{
	// Pointer to font object
	TTF_Font* m_pFont;

	// Surface for text to be drawn in to
	SDL_Surface *text_surface;

	//Set up m_pFont for text output *make settable* also ensure TTF_Init has been run
	m_pFont = TTF_OpenFont( fontName.c_str(), 26 );

	// Was m_pFont not loaded OK
	if(!m_pFont)
	{
		cout << "Could not load font file" << endl;
	}

	// Set the colour according to the arguments
	SDL_Color color = {r, g, b};

	// There are also other ways to render the text see the headerfile for fonts
	text_surface = TTF_RenderText_Blended(m_pFont, text.c_str(), color);

	if(text_surface == NULL)
	{
		cout << "Could not create render surface for m_pFont" << endl;
	}

	draw(pos_x, pos_y, text_surface, SDL_GetVideoSurface(), NULL);
	
	// Release the Font
	if( m_pFont != NULL ){
		TTF_CloseFont(m_pFont);
	}

	SDL_FreeSurface(text_surface);

}

/* This works but is commented out because of the abs error warning whencompiling with GNU
void CSDLGraphics::drawLine(int start_x, int start_y, int end_x, int end_y, UINT r, UINT g, UINT b )
{
	// Add screen limit checks and check for positive integers only

	int x0, x1, y0, y1 = 0;

	int ystep = 0; // Determine which direction the line is drawn, up or down, on the y-axis

	// Copy values for maninulation
	x0 = start_x;
	x1 = end_x;
	y0 = start_y;
	y1 = end_y;

	Uint32 color = SDL_MapRGB(SDL_GetVideoSurface()->format, r, g, b);

	bool steep = false; // Boolean for is line steeper than 45 degrees?

	int tmp1, tmp2 = 0;

	tmp1 = abs(y1 - y0);
	tmp2 = abs(x1 - x0);

	if( tmp1 > tmp2 )
	{
		steep = true;
	}
	
	// Is delta of y longer than delta of x, if so line extends past 45 degrees and values must be swapped 
	if( steep ) 
	{
		int steep_swap = 0; // Used for swapping routine

		steep = true;	  // Flag that line is steep used later during plotting

		steep_swap = y0;	  // Save state of start_y

		y0 = x0;		  // Make start_y == end_x
		x0 = steep_swap;	  // make end_x == start_y

		steep_swap = x1;    // Savew state of end_x

		x1 = y1;		  // Make end_x = end_y
		y1 = steep_swap;    // Make end_y = end_x

		
	}
	
	if( x0 > x1) // Line goes backwards 
	{
		int back_swap = 0;
		back_swap = x0;
		x0 = x1;
		x1 = back_swap;
		back_swap = y0;
		y0 = y1;
		y1 = back_swap;
	}

	float deltax = (float)x1 - (float)x0;       				// The difference in the x's
	float deltay = abs((float)y1 - (float)y0);       			// The difference in the y's NOTE: must be the absolute value i.e. without and + or - sign
												// or must be positive

	float error = 0.0;
	float deltaerr = (deltay / deltax);			// Determine slope of line or how steep the line is
	int y = y0;

	if( y0 < y1 )			// If line goes down in the Y-axis 
	{
		ystep = 1;
	}
	else					// If line goes up in the Y-axis
	{
		ystep = -1;
	}

	for (int x = x0; x <= x1; x++)				// Iterate along line on the a-axis
	{
	  if( steep )
	  {
		  PutColor(SDL_GetVideoSurface(), y, x, color);     // Draw the current pixel using swapped coordinates
	  }
	  else
	  {
		  PutColor(SDL_GetVideoSurface(), x, y, color);		// Draw the current pixel using normal coordinates
	  }

	  error += deltaerr;       							// Add slope to error to determine whether to go one down
	  if (error >= 0.5)   								// If error is over half way move down on the next square
	  {
		y = y + ystep;                					// Increase the value of y to move down a square.
		error = error - 1.0f;     						// Reset error by one unit
	  }
	}
	return 0;
}
*/


