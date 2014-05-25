#ifndef INC_GLOBAL_GAMEDATA_H
#define INC_GLOBAL_GAMEDATA_H

// CGlobalGameData holds global game data that needs to be accessed between game states
// NOTE: An alternative and possibly better way would be to have the file IO functionality
// for reading and writing the save file abstracted so that each class can choose to read/write
// to it. This way there will be no need to be passing the class to every state. 
class CGlobalGameData{

	public:
		CGlobalGameData(){};
		~CGlobalGameData(){};

		void setScore( int score );
		int getScore();

	private:
		
		int score;

};

#endif 
