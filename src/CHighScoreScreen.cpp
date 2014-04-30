#include "../include/CHighScoreScreen.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

CHighScoreScreen::CHighScoreScreen( CSDLGraphics& graphics ) : IScreen( graphics ){
	
	this->graphics = &graphics;		
	topscoreframe = NULL;
	quit = false;
}

bool CHighScoreScreen::init(){

	topscoreframe = graphics->loadImageFromFile("./images/TopScoreScreenFrame.bmp", 255, 0, 255 );	

	loadTopScoresFromFile();

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
		switch(event->key.keysym.sym)
		{	
			case SDLK_ESCAPE:
				quit = true;
			break;
			case SDLK_RETURN:

			break;
			default:
				break;
		}
		event->type = SDL_KEYUP;
		/*
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
		*/
	}
}

void CHighScoreScreen::update(){

}

void CHighScoreScreen::render(){

	std::ostringstream scoreAsString;
	
	// Clear screen
	graphics->clearScreen( 0, 0, 0);

	// Draw frame and high score title
	graphics->draw( 0, 0, topscoreframe, SDL_GetVideoSurface(), NULL);
	
	std::vector< scoredata >::iterator it;

	// Y location placement for score
	int rowSpacing = 90;
	int i = 1;
	// Loop through the 
	for(it = topScores.begin(); it < topScores.end(); it++)
	{
		scoreAsString << i << ". " << (*it).scorename << "..............." << (*it).score;
		graphics->drawText( scoreAsString.str(), 100, rowSpacing, "tunga.ttf", 255, 0, 0 );
		rowSpacing += 35;
		scoreAsString.str("");
		i++;
	}

	// Show the screen
	graphics->update();
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
		// Read in name and score in to score data structure
		topscorefile >> topScore.scorename >> topScore.score;
		topScores.push_back( topScore );
	}

	// Sort in descending order 
	std::sort(topScores.begin(), topScores.end(), scorecmp );

}

bool CHighScoreScreen::scorecmp( const scoredata& left, const scoredata& right ){
	return left.score > right.score;
}

