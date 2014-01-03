#include "../include/main.h" // All SDL apps need
#include <vector>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 440
#define SCREEN_BPP    32

#define FPS 60

using namespace std;

void OnRenderScoreScreen();
void OnUpdateScoreScreen();

typedef struct _SCOREDATA
	{
		string scorename;
		int score;
	}scoredata;

void LoadTopScoresFromFile();

// Array data for top scores
scoredata topscores[10];

// SDL event structure
SDL_Event event; 

// Declare objects
CSDLGraphics sdlgraphics;
CSDLGraphics* graphics;

CGamePiece CPlayerPiece;

// Declare SDL surfaces

SDL_Surface *gameoverframe = NULL;
SDL_Surface *topscoreframe = NULL;

bool startGame = false;
bool scoreEntered = false;
bool g_bPaused = false;
bool g_bGameOver = false;

SDL_Rect srcrect;

int main( int argc, char* args[])
{	
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

	// Create all the screens	
	IScreen *introScreen = new CIntroScreen( *graphics );
	IScreen *gameScreen = new CGameArea( *graphics );

	// Hold references to all the screens
	std::vector< IScreen* > screens;

	// Add the screens in order
	screens.push_back( gameScreen );
	screens.push_back( introScreen );
	
	// gameoverframe = g_pResource->LoadImageFromFile("./images/GameOverFrame00.bmp",true);
	// topscoreframe = g_pResource->LoadImageFromFile("./images/TopScoreScreenFrame.bmp", true);	

	IScreen* currentScreen = NULL;
	bool isInitialised = false;
	startTime = SDL_GetTicks();

	currentScreen = screens.back();

	while( gameQuit != true)
	{
		if( isInitialised == false ){
			if( currentScreen->init() == false ){
				cout << "Failed to initalise screen!" << endl;
				gameQuit = true;
			}	
			isInitialised = true;	
		}	

		SDL_PollEvent( &event );
	
		// Get the time at the beginning of the frame	
		startTime = SDL_GetTicks();// NOTE: Have the game speed be alterable by taking the FPS constant
		// and modifying it either by adding or subtracting to speed up/ slow down speed
		// or could do some dela time trick to avoid alter the overall FPS 
		
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
			screens.pop_back();
			currentScreen = screens.back();
			isInitialised = false;
		}
	}
			
	SDL_Quit();

	return 0;
}

void OnRenderScoreScreen()
{
//	SDL_Color color = { 255, introTextColour, introTextColour};

	int leftAlign = 5;

	char scoreinfo[5];

	// Clear screen
	SDL_FillRect(SDL_GetVideoSurface(), NULL, SDL_MapRGB(SDL_GetVideoSurface()->format, 0, 0, 0 ));
/*
	sdlObj.DrawImage(0, 0, topscoreframe, sdlObj.screenSurface, NULL);
	
	for(int i = 0; i < 9; i++)
	{
		sdlObj.DrawImage(g_pScreenGrid->GetSDLGridXY(7, leftAlign).x, g_pScreenGrid->GetSDLGridXY(7, leftAlign).y, g_pResource->Text(topscores[i].scorename.c_str(), color)->pTileset,
		sdlObj.screenSurface, NULL);

		sprintf(scoreinfo, "%d", topscores[i].score);

		sdlObj.DrawImage(g_pScreenGrid->GetSDLGridXY(10, leftAlign).x, g_pScreenGrid->GetSDLGridXY(10, leftAlign).y, g_pResource->Text(scoreinfo, color)->pTileset,
		sdlObj.screenSurface, NULL);

	leftAlign += 2;
	}
*/	
	// Show the screen
	// Or can use update rects
//	sdlObj.UpdateScreen();

}

void OnUpdateScoreScreen()
{

}

void LoadTopScoresFromFile()
{
	int scoreIndex, currentScore = 0;
	int sortIndex = 0;

	// top score file object Open file for input and output access
	fstream topscorefile("TopScore.txt");

	// Is file valid
	if(!topscorefile)
	{
		cout << "Could not open top score file" << endl;
	}
	
	// Read in 10 scores scores from file
	for(scoreIndex = 0; scoreIndex < 10; scoreIndex++)
	{
		topscorefile >> topscores[scoreIndex].scorename
			>> topscores[scoreIndex].score;
	}

	// Sort the scores
	// Bubble sort taken from http://www.cplusplus.com/forum/beginner/39428/ dont fully understand how it works

	// Iterate back from 10 repeating the process 10 times over
	for(int sortIndex = 10; sortIndex >=0; sortIndex--)
	{
		// Iterate up through the array elements, keep going through and testing each element to see if it is still greater
		for(int j = 0; j < 9; j++)
		{
			// If current element is greater than the next element 
			if(topscores[j].score > topscores[j+1].score)
			{
				// Store next element temporarly
				int temp = topscores[j+1].score;

				// Make the next element the current element, move it up one in the array list
				topscores[j+1].score = topscores[j].score;

				// Swap the names in the list, tracks the name with the score sort
				topscores[j+1].scorename.swap(topscores[j].scorename);
				
				// Copy the value that as greater in to the current array element location.
				topscores[j].score = temp;
				
			}
		}

	}
	/*
	// Take current score and iterate through the list and compare to each listing
	currentScore = CPlayArea.GetIntScore();

	int i = 0;

	// Step through the list to find were the current score ranks
	while(currentScore > topscores[i].score && i < 9)
	{
		i++;
	}

	// If score makes it in to the top 10 move every other entry down and insert new score 
	if(i > 0)
	{

		// ** Prompt for user input ** 
		// Only Activate input for under these circumstances. 

		// Remove the last entry in the array
		topscores[9].score = 0;

		// Take current location and iterate down the list moving everything down one
		for(int j = 9; j < i; j--)
		{
			topscores[j].score = topscores[j+1].score;
		}
		// Insert the new score
		topscores[i].score = currentScore;
	}

	*/
	// Iterate through list and display each score to the screen.
}

// Handle input of name taken from Lazt Foo's tutorials
void OnTopScoreInput()
{
	// Enable UNICODE for all characters
	SDL_EnableUNICODE( SDL_ENABLE );

	// String to hold the user name
	string userName;

	// Get user input
	if(event.type == SDL_KEYDOWN)
	{
		if(userName.length() < 3)
		{
			// if the key is a space
			if(event.key.keysym.unicode == (Uint16)' ')
			{
				// Append the character
				userName += (char)event.key.keysym.unicode;
			}
			// If the key is a number
			else if( ( event.key.keysym.unicode >= (Uint16)'0') && (event.key.keysym.unicode <= (Uint16)'9') )
			{
				// Append the character
				userName += (char)event.key.keysym.unicode;
			}
			else if( ( event.key.keysym.unicode >= (Uint16)'A') && (event.key.keysym.unicode <= (Uint16)'Z') )
			{
				// Append the character
				userName += (char)event.key.keysym.unicode;
			}
			else if( ( event.key.keysym.unicode >= (Uint16)'a') && (event.key.keysym.unicode <= (Uint16)'z') )
			{
				// Append the character
				userName += (char)event.key.keysym.unicode;
			}

		}
		// If back space was pressed and the string isnt empty
		if( (event.key.keysym.unicode == SDLK_BACKSPACE) && ( userName.length() != 0 ))
		{
			userName.erase( userName.length() -1);
		}

	}


}

int InitTopScores()
{
	return 0;

}
