#include "CASlice.h"



Grid::Grid(unsigned int rows, unsigned int columns, int bt):
m_numRows(rows),m_numColumns(columns),borderType(bt){
	/* size of char allocation needed to represent COLUMNS bits */
	m_allocCharSizeRow = (m_numColumns / CABLOCKTYPE_BIT_SIZE);
    m_allocCharSizeRow += ((m_numColumns % CABLOCKTYPE_BIT_SIZE) == 0)?0:1;
    /* add 1 bit of empty padding on the left, and at least 1 bit on the right */
    m_allocCharSizeRow += (((m_allocCharSizeRow*CABLOCKTYPE_BIT_SIZE) - m_numColumns) < 2)?1:0;
	gridSliceAlloc( );
}
Grid::Grid(const Grid& g):
m_numRows(g.m_numRows),m_numColumns(g.m_numColumns),borderType(g.borderType),m_allocCharSizeRow(g.m_allocCharSizeRow){
	gridSliceAlloc( );
	for(int j=0; j < this->m_numRows+2;j++)
		for(int i=0; i<this->m_allocCharSizeRow;i++)
			mem[j][i] = g.mem[j][i];
}

void Grid::gridSliceAlloc( void ){
    mem = new CABLOCKTYPE*[m_numRows + 2];
	for(int i=0; i<m_numRows+2;i++){
		CABLOCKTYPE* N = new CABLOCKTYPE[m_allocCharSizeRow];
        mem[i] = N;         
        for(int j=0; j < m_allocCharSizeRow; j++)
			if((i==0)||(i==m_numRows+1))
            mem[i][j] = ((this->borderType == BORDER_1BOUNDED)? 0xFF:0x00);
                else
                    if((j==0)&&(this->borderType == BORDER_1BOUNDED))
                        mem[i][j] = 0x80;
                    else
                        if((j==m_allocCharSizeRow-1)&&(this->borderType == BORDER_1BOUNDED))
                            mem[i][j] = 0x01; 
                        else
                            mem[i][j] = 0x00;    
        }
}


Slice::Slice(unsigned int rows, unsigned int columns, int bt):
grid(rows, columns, bt),m_evolveNumber(0){
} 
Slice::Slice(Grid g, unsigned int en):
grid(g),m_evolveNumber(en){
} 

unsigned int Slice::getRows() const {return grid.m_numRows;}
unsigned int Slice::getCols() const {return grid.m_numColumns;}
void Slice::turnCellOn(unsigned int ith_Row, unsigned int jth_column){


        /*****************************************
        (0, 0)          (0 , j)
        ------------->
        |
        |
        |
        |
        (i, 0) \ /
        ******************************************/

   // if(inBoundCheck(ith_Row, jth_column)){
            unsigned char mask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
            int step = (jth_column+1)/CABLOCKTYPE_BIT_SIZE;
            grid.mem[ith_Row+1][step] = grid.mem[ith_Row+1][step] | mask[(jth_column+1)%CABLOCKTYPE_BIT_SIZE];
    /*}
    else
        throw "Slice Size Xception at Slice::turnCellOn";*/
}
bool Slice::isOn(int ith_Row, int jth_column) const{
	/*return true;
    if(inBoundCheck(ith_Row, jth_column)){   */ 
            unsigned char mask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
            int step = (jth_column+1)/CABLOCKTYPE_BIT_SIZE;
            return (bool)(grid.mem[ith_Row+1][step] & mask[(jth_column+1)%CABLOCKTYPE_BIT_SIZE]);
    /*}
    else throw "Slice Size Xception at Slice::isOn method";*/
}
Slice* Slice::evolve(const Rule& rule) const{
	Slice* top = new Slice(grid, m_evolveNumber+1);
    for(int i=0; i < grid.m_numRows; i++)
		for(int j=0;j<grid.m_numColumns;j++){
			if(rule.subRule(this->getConfig(i, j))) top->turnCellOn(i, j);
             }
        return top;
}
unsigned short int Slice::getConfig(unsigned int ith_Row, unsigned int jth_column) const{
    if(!((ith_Row >= 0)&&(ith_Row < this->grid.m_numRows))) throw "RowSize Xception at Slice::getConfig";
    if(!((jth_column >= 0)&&(jth_column < this->grid.m_numColumns))) throw "ColSize Xception at Slice::getConfig";;
    if(!grid.borderType) return getConfigUnbounded(ith_Row, jth_column);
    return getConfigBounded(ith_Row, jth_column);
}
unsigned short int Slice::getConfigUnbounded(unsigned int ith_Row, unsigned int jth_column) const{  
    int i=  ith_Row+1;
    if(ith_Row == 0) return getConfigMiddleRow(jth_column, grid.mem[grid.m_numRows], grid.mem[i],grid.mem[i+1]);
    if(ith_Row == grid.m_numRows - 1) return getConfigMiddleRow(jth_column, grid.mem[i-1],grid.mem[i],grid.mem[1]);
    return getConfigMiddleRow(jth_column, grid.mem[i-1], grid.mem[i],grid.mem[i+1]);
}
unsigned short int Slice::getConfigBounded(unsigned int ith_Row, unsigned int jth_column)const{
    int i=  ith_Row+1;
    return getConfigMiddleRow(jth_column, grid.mem[i-1], grid.mem[i],grid.mem[i+1]);    
}
unsigned short int Slice::getConfigMiddleRow(unsigned int jth_column, CABLOCKTYPE* PrevRow, CABLOCKTYPE* MiddRow, CABLOCKTYPE* NextRow) const{

    unsigned short int config = 0;
    int j = (jth_column+1)/CABLOCKTYPE_BIT_SIZE;
    int k = (jth_column+1)%CABLOCKTYPE_BIT_SIZE;
    
    if((jth_column == 0)&&(!grid.borderType)){
        
        config = ((unsigned short int)(PrevRow[j-1] & 0x01))<<8;
        // 0 0 0 0    0 0 0 *    0 0 0 0    0 0 0 0

        config |= ((unsigned short int)(PrevRow[j] & 0xC0));
        // 0 0 0 0    0 0 0 #    * * 0 0    0 0 0 0
    
        config |= ((unsigned short int)(MiddRow[j-1] & 0x01))<<5;
        // 0 0 0 0    0 0 0 #    # # * 0    0 0 0 0

        config |= (unsigned short int)((MiddRow[j] & 0xC0)>>3);
        // 0 0 0 0    0 0 0 #    # # # *    * 0 0 0

        config |= ((unsigned short int)(NextRow[j-1] & 0x01))<<2;
        // 0 0 0 0    0 0 0 #    # # # #    # * 0 0

        config |= (unsigned short int)((NextRow[j] & 0xC0)>>6);
        // 0 0 0 0    0 0 0 #    # # # #    # # * *

    }
    
/*    if((jth_column == numColumns-1)&&(!borderType)){
    
        return ...
    }*/
    
    /**********************************
    Are we on the first bit of the char?
    ************************************/
    if(k==0){
        /********************************
        otherwise, start putting 2gether the configuration
        using the last bit of the previous char on
        all 3 rows
        ********************************/
        config = ((unsigned short int)(PrevRow[j-1] & 0x01))<<8;
        // 0 0 0 0    0 0 0 *    0 0 0 0    0 0 0 0

        config |= ((unsigned short int)(PrevRow[j] & 0xC0));
        // 0 0 0 0    0 0 0 #    * * 0 0    0 0 0 0
    
        config |= ((unsigned short int)(MiddRow[j-1] & 0x01))<<5;
        // 0 0 0 0    0 0 0 #    # # * 0    0 0 0 0

        config |= (unsigned short int)((MiddRow[j] & 0xC0)>>3);
        // 0 0 0 0    0 0 0 #    # # # *    * 0 0 0

        config |= ((unsigned short int)(NextRow[j-1] & 0x01))<<2;
        // 0 0 0 0    0 0 0 #    # # # #    # * 0 0

        config |= (unsigned short int)((NextRow[j] & 0xC0)>>6);
        // 0 0 0 0    0 0 0 #    # # # #    # # * *

    }//end if(k==0)
    else 
        if((1<=k)&&(k<=6)){
            /******************************************
            Get the 3 bits of the preceding,
            current and next rows at char j
            and shift accordingly
            ******************************************/
            unsigned char mask[6] = {0xE0, 0x70, 0x38, 0x1C, 0x0E, 0x07};
            
            config = ((unsigned short int) ((PrevRow[j] & mask[k-1]) << (k-1)))<<1 ;
            // 0 0 0 0    0 0 0 *    * * 0 0    0 0 0 0

            config  |= ((unsigned short int)((MiddRow[j] & mask[k-1])<< (k - 1)))>>2;
            // 0 0 0 0    0 0 0 #    # # * *    * 0 0 0

            config  |= ((unsigned short int) ((NextRow[j] & mask[k-1]) << (k-1)))>>5;
            // 0 0 0 0    0 0 0 #    # # # #    # * * *
        }//end if((1<=k)&&(k<=6))
        else{
                /******************************************
                on the last k (k==7)
                Get the first 2 bits of the preceding,
                current and next rows at char j
                and shift accordingly
                ******************************************/
                config = ((unsigned short int) (PrevRow[j] & 0x03))<<7;
                // 0 0 0 0    0 0 0 *  * 0 0 0  0 0 0 0
                
                config  |= (unsigned short int)((PrevRow[j+1] & 0x80)>>1);
                // 0 0 0 0    0 0 0 #  # * 0 0   0 0 0 0

                config  |= ((unsigned short int)(MiddRow[j] & 0x03))<<4;
                // 0 0 0 0    0 0 0 #  # # * *   0 0 0 0
                
                config  |= (unsigned short int) ((MiddRow[j+1] & 0x80)>>4);
                // 0 0 0 0    0 0 0 #  # # # #   * 0 0 0
                
                config  |= (unsigned short int)((NextRow[j] & 0x03)<<1);
                // 0 0 0 0    0 0 0 #  # # # #   # * * 0

                config  |= ((unsigned short int)(NextRow[j+1] & 0x80))>>7;
                // 0 0 0 0    0 0 0 #  # # # #   # # # *
        }//end else

    return config;
}
Slice::~Slice(void)
{
}
