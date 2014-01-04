#include "../include/CHighScoreScreen.h"
#include <iostream>
#include <fstream>

CHighScoreScreen::CHighScoreScreen( CSDLGraphics& graphics ) : IScreen( graphics ){
	
	this->graphics = &graphics;	
	gameoverframe = NULL;
	topscoreframe = NULL;
}

bool CHighScoreScreen::init(){

	gameoverframe = graphics->loadImageFromFile("./images/GameOverFrame00.bmp", 255, 0, 255 );
	topscoreframe = graphics->loadImageFromFile("./images/TopScoreScreenFrame.bmp", 255, 0, 255 );	

	return true;
}

// Handle input of name taken from Lazt Foo's tutorials
void CHighScoreScreen::processEvents( SDL_Event* event ){
	// Enable UNICODE for all characters
	SDL_EnableUNICODE( SDL_ENABLE );

	// String to hold the user name
	std::string userName;

	// Get user input
	if(event->type == SDL_KEYDOWN)
	{
		if(userName.length() < 3)
		{
			// if the key is a space
			if(event->key.keysym.unicode == (Uint16)' ')
			{
				// Append the character
				userName += (char)event->key.keysym.unicode;
			}
			// If the key is a number
			else if( ( event->key.keysym.unicode >= (Uint16)'0') && ( event->key.keysym.unicode <= (Uint16)'9') )
			{
				// Append the character
				userName += (char)event->key.keysym.unicode;
			}
			else if( ( event->key.keysym.unicode >= (Uint16)'A') && ( event->key.keysym.unicode <= (Uint16)'Z') )
			{
				// Append the character
				userName += (char)event->key.keysym.unicode;
			}
			else if( ( event->key.keysym.unicode >= (Uint16)'a') && ( event->key.keysym.unicode <= (Uint16)'z') )
			{
				// Append the character
				userName += (char)event->key.keysym.unicode;
			}

		}
		// If back space was pressed and the string isnt empty
		if( (event->key.keysym.unicode == SDLK_BACKSPACE) && ( userName.length() != 0 ))
		{
			userName.erase( userName.length() -1);
		}

	}


}

void CHighScoreScreen::update(){

}

void CHighScoreScreen::render(){

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

void CHighScoreScreen::loadTopScoresFromFile(){

	int scoreIndex, currentScore = 0;
	int sortIndex = 0;

	// top score file object Open file for input and output access
	std::fstream topscorefile("TopScore.txt");

	// Is file valid
	if(!topscorefile)
	{
		std::cout << "Could not open top score file" << std::endl;
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


