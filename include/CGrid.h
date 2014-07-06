#ifndef INC_CGRID_H
#define INC_CGRID_H
#include <iostream>

typedef struct _COORDS{
	unsigned int x, y;
}coords;


class CGrid{

	public:
		CGrid(int grid_width, int grid_height, int tile_width, int tile_height, int start_x, int start_y );
		~CGrid();
	
		// Return x and y coordinates for a given grid reference	
		coords* getXY( unsigned int x, unsigned int y );
		
		unsigned int getWidth(){ return m_grid_width; }	
		unsigned int getHeight(){ return m_grid_height; }

		// Get and set flags for each tile in the grid
		void setTileFlag( unsigned int x, unsigned int y, int flag_code );
		int getTileFlag( unsigned int x, unsigned int y);
		

	private:
		
		// Structure for each grid element
		typedef struct _GRID
		{
			// Actual screen coordinates
			int x, y;
			// Status flag for generic use
			int flag;

		}m_sGrid;
		
		// Pointer to new grid structure
		m_sGrid* pNewGrid;	
		
		// Store grid width and height set by create grid and useful for member functions
		int m_grid_width, m_grid_height;

		// Store the tile width used for returning metrics about specific tiles
		unsigned int m_tileWidth, m_tileHeight;
	
};

#endif // INC_CGRID_H
