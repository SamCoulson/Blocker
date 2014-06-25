#include "../include/CHighScoreScreen.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

CHighScoreScreen::CHighScoreScreen( CSDLGraphics& graphics, CGlobalGameData& gameData ) : IScreen( graphics, gameData ){
	
	this->graphics = &graphics;
	this->gameData = &gameData;	
	topscoreframe = NULL;
	inEditMode = false;
	nameComplete = false;
	quit = false;
}

bool CHighScoreScreen::init(){

	// Frame for score board
	topscoreframe = graphics->loadImageFromFile("./images/TopScoreScreenFrame00.png", 255, 0, 255 );	

	// Load in the existing scores from file
	// Needs error checking
	loadTopScoresFromFile();

	int newScore = gameData->getScore(); 

	// Determine if new high score has been reached
	if( newScore >= topScores.back().score ){
		std::cout << "New High Score" << std::endl;	
		inEditMode = true;

		// Iterator for scores read in from file	
		std::vector< scoredata >::iterator it;

		// Determine where in the rank order the new score fits start from top and work down
		for(it = topScores.begin(); it < topScores.end(); it++){
	
			// If new score is greater than current score overwrite it and enter edit mode
			if( newScore > (*it).score ){
				(*it).score = newScore;
				(*it).isNew = true;
				inEditMode = true;
				break;
			}		
		}
	}

	return true;
}

// Handle input of name taken from Lazy Foo's tutorials
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
				// Finshed entering name
				inEditMode = false;
				saveTopScoresToFile(); // Should be in update function
			break;
			default:
				break;
		}
		event->type = SDL_KEYUP;
			
		if( newName.str().size() < 3 && inEditMode ){
			// if the key is a space
			if(event->key.keysym.unicode == (Uint16)' ')
			{
				// Append the character
				newName << (char)event->key.keysym.unicode;
				std::cout << "Letter key pressed" << (char)event->key.keysym.unicode << std::endl;
			}
			// If the key is a number
			else if( ( event->key.keysym.unicode >= (Uint16)'0') && ( event->key.keysym.unicode <= (Uint16)'9') )
			{
				// Append the character
				newName << (char)event->key.keysym.unicode;
				std::cout << "Letter key pressed" << (char)event->key.keysym.unicode << std::endl;

			}
			else if( ( event->key.keysym.unicode >= (Uint16)'A') && ( event->key.keysym.unicode <= (Uint16)'Z') )
			{
				// Append the character
				newName << (char)event->key.keysym.unicode;
				std::cout << "Letter key pressed" << (char)event->key.keysym.unicode << std::endl;

			}
			else if( ( event->key.keysym.unicode >= (Uint16)'a') && ( event->key.keysym.unicode <= (Uint16)'z') )
			{
				// Append the character
				newName << (char)event->key.keysym.unicode;
				std::cout << "Letter key pressed" << (char)event->key.keysym.unicode << std::endl;

			}
		}
		
		// If back space was pressed and the string isnt empty
		if( (event->key.keysym.unicode == SDLK_BACKSPACE) /*&& ( newName.size() != 0 )*/)
		{
			std::string name = newName.str();
			name = name.erase( name.length()-1 ); 
			newName.str( name );
		}
	
	}
}

void CHighScoreScreen::update(){
	
		
}

void CHighScoreScreen::render(){
	
	// String stream for holding interger score as string
	std::ostringstream rankNumAsString;
	std::ostringstream scoreAsString;
	
	// Clear screen
	graphics->clearScreen( 0, 0, 0);

	// Draw frame and high score title
	graphics->draw( 0, 0, topscoreframe, SDL_GetVideoSurface(), NULL);

	// Iterator for scores read in from file	
	std::vector< scoredata >::iterator it;

	// Inital Y location placement for score
	int rowSpacing = 70;

	// Rank in score list
	int rankNum = 1;

	int greenVal = 10;

	// Loop through the scores vector and output each name and score on a new line
	for(it = topScores.begin(); it < topScores.end(); it++){

		// Concatentate numbers and charatcers to form the current line
		rankNumAsString << rankNum << ". ";
		scoreAsString << (*it).score;

		// Draw the line
		graphics->drawText( rankNumAsString.str(), 100, rowSpacing, "tunga.ttf", 26, 255, greenVal+=10, 0 );
		if( (*it).isNew == true ){
			// If less than 3 chars append a blinking underscore	
			graphics->drawText( newName.str(), 130, rowSpacing, "tunga.ttf", 26, 255, greenVal, 0 );
		}else{
			graphics->drawText( (*it).scorename, 130, rowSpacing, "tunga.ttf", 26, 255, greenVal, 0 );
		}
		graphics->drawText( "..........", 170, rowSpacing, "tunga.ttf", 26, 255, greenVal, 0 );
		graphics->drawText( scoreAsString.str(), 245, rowSpacing, "tunga.ttf", 26, 255,  greenVal, 0 );

		// Move down enough for next line
		rowSpacing += 35;

		// Clear the string buffer for next line
		scoreAsString.str("");
		rankNumAsString.str("");

		// Increment place in score list number
		rankNum++;
	}

	// Show the screen
	graphics->update();
}

void CHighScoreScreen::loadTopScoresFromFile(){
	
	// Open top score text file for input and output access
	std::fstream topscorefile("TopScore.txt");

	// Is file valid
	if(!topscorefile){
		std::cout << "Could not open top score file" << std::endl;
		return;
	}
	
	// Read in 10 scores from file
	for(int scoreIndex = 0; scoreIndex < 10; scoreIndex++){
		// Read in name and score in to score data structure
		topscorefile >> topScore.scorename >> topScore.score;
		topScores.push_back( topScore );
	}

	// Sort in descending order 
	std::sort(topScores.begin(), topScores.end(), scorecmp );

	// Close the file
	topscorefile.close();
}

void CHighScoreScreen::saveTopScoresToFile(){
	
	// Open top score text file for input and output access
	std::fstream topscorefile("TopScore.txt");

	// Is file valid
	if(!topscorefile){
		std::cout << "Could not open top score file" << std::endl;
		return;
	}

	// Iterator for scores read in from file	
	std::vector< scoredata >::iterator it;
	
	// Read in 10 scores from file
	for(it = topScores.begin(); it < topScores.end(); it++){

		// Find the changed name and apply it
		if( (*it).isNew == true ){
			(*it).scorename = newName.str();
		}

		// Read in name and score in to score data structure
		topscorefile << (*it).scorename << std::endl; 
	       	topscorefile << (*it).score << std::endl;	
	}

	// Sort in descending order 
	//std::sort(topScores.begin(), topScores.end(), scorecmp );

	// Close the file
	topscorefile.close();
}

bool CHighScoreScreen::scorecmp( const scoredata& left, const scoredata& right ){
	return left.score > right.score;
}

