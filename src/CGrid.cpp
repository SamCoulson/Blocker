#include "../include/CGrid.h"

// ** Modified for SDL usage**

// Grid Class constructor
// Creates a grid given the start xy in screen pixels, height and width in tilesize and the size of the tiles
// *Maybe move to init function becasue memory is allocated here. If fails then dead memory.
CGrid::CGrid( int grid_width, int grid_height, int tile_width, int tile_height, int start_x, int start_y )
{
	// Could set these in a initialiser list	
	m_grid_width = 0;
	m_grid_height = 0;
	pNewGrid = NULL;

	int iNewGridSize = 0;
	int grid_widthIndex, grid_heightIndex = 0;
	const int fRowStart_x = start_x;

	// Work out how many elements for the new grid array are needed by multiplying given width by height
	iNewGridSize = grid_width * grid_height;

	// Allocate memory for new grid.  Use new as this is how C++ allocates memory. Watch out for memory allocation problems
	pNewGrid = new m_sGrid[iNewGridSize];

	// Assigning in order of row then height
	// NOTE Puttin <= causes an out of bounds buffer access and a leak, try to remember this.
	for(grid_heightIndex = 0; grid_heightIndex < grid_height; grid_heightIndex++)
	{
		for(grid_widthIndex = 0; grid_widthIndex < grid_width; grid_widthIndex++)
		{
			pNewGrid[(grid_heightIndex*grid_width) + grid_widthIndex].x = start_x;
			pNewGrid[(grid_heightIndex*grid_width) + grid_widthIndex].y = start_y;
			pNewGrid[(grid_heightIndex*grid_width) + grid_widthIndex].flag = 0;
			start_x += tile_width; // Increment start_x by tile size to plot out X where each column starts. 
		}
		start_x = fRowStart_x; // Reset start_x back to original value, to begin next row, the left edge of the grid starts on each column assignment. 
		start_y += tile_height; // Increment start_y by tile size to plot out Y where each row starts.
	}

	// Store metrics for getter functions
	m_grid_width = grid_width;
	m_grid_height = grid_height;

	m_tileWidth = tile_width;
	m_tileHeight = tile_height;

}


CGrid::~CGrid()
{
	delete [] pNewGrid;
}

coords* CGrid::getXY( unsigned int x, unsigned int y ){

	coords* xy = new coords;

	// Do input parameter exceed the width and height of the constructed grid
	if( ( x > m_grid_width) || ( y > m_grid_height) ){
		std::cout << "Requested grid x and y coordinates out of range: x = " 
			<< xy->x << " y = " << xy->y << std::endl;
	}

	xy->x = pNewGrid[( y * m_grid_width) + x ].x;
	xy->y = pNewGrid[( y * m_grid_width) + x ].y;

	return xy;
}

// Set the status variable for a particular grid square. 
// Grid status is an int to give enough possibilites for flag types.
void CGrid::setTileFlag( unsigned int x, unsigned int y, int flag_code ){

	// Do input parameter exceed the width and height of the constructed grid
	if( ( x > m_grid_width ) || ( y > m_grid_height) )
	{
		std::cout << "Cannot set grid x and y flag, coordinates out of range: x = " 
			<< x << " y = " << y << std::endl;
	}

	pNewGrid[( y * m_grid_width ) + x ].flag = flag_code;
}

// Returns the grid status int value
int CGrid::getTileFlag( unsigned int x, unsigned int y){
	return pNewGrid[( y * m_grid_width) + x ].flag;
}



