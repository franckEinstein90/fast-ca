#pragma once
#include "rule.h"
#define BORDER_TORUS 0
#define BORDER_0BOUNDED 1
#define BORDER_1BOUNDED 2

#define CABLOCKTYPE unsigned char
#define CABLOCKTYPE_BYTE_SIZE   (sizeof(CABLOCKTYPE))
#define CABLOCKTYPE_BIT_SIZE    ((int)(CABLOCKTYPE_BYTE_SIZE*8))

class Grid{

	friend class Slice;

protected:
	CABLOCKTYPE ** mem;
	unsigned int m_numRows;
	unsigned int m_numColumns;
	unsigned int m_allocCharSizeRow;
	int borderType;
	void gridSliceAlloc( void );

public:
	Grid(unsigned int, unsigned int, int);
	Grid(const Grid&);
};



class Slice{

protected:
	Grid grid;
	unsigned int m_evolveNumber;
	unsigned short int getConfig(unsigned int, unsigned int) const;
	unsigned short int getConfigUnbounded(unsigned int, unsigned int) const;
	unsigned short int getConfigBounded(unsigned int, unsigned int) const;
	unsigned short int Slice::getConfigMiddleRow(unsigned int, CABLOCKTYPE*, CABLOCKTYPE*, CABLOCKTYPE*) const;
public:
	Slice(unsigned int, unsigned int, int);
	Slice(Grid, unsigned int);
	~Slice(void);
	Slice* evolve(const Rule& rule) const;
	unsigned int getRows() const;
	unsigned int getCols() const;
	bool isOn(int x, int y) const;
	void turnCellOn(unsigned int, unsigned int);


	
};
