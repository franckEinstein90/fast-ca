# fast-ca
Raw code to produce large 1-D cellular automata

Cell Automata Library (2D)

C++, Cellular Automata


Overview
This is fast lightweight library for 2D cellular automata generation (for example, Conway's game of life). It's meant to use a very small memory footprint and to be faster than any other engine I could find. Also standardizes the definition of a CA set of rules (there are 2^512 possible 2D CA, of which Conway's game of life is only one.

Implementations for windowsXP, 95-2000, Visual C++ (.net) 2-dimensional, k=3 (adjacent cells are colored differently), square grid, Moore Neighborhood of range 1. Rules are coded over 512 bits, 000000... - 11111... In order to write a fast CA engine, I needed to squeeze every bits out of every bytes. See the demo repo for win32 executable samples. If you don't want to dload an executable, you can also watch the blurry youtube video to get an idea of the kind of output you might expect.

Technical Details
The CA grid stored in an object of type Grid, basically a wrapper for a pointer to a pointer to an unsigned char (a matrix):

unsigned char** mem;

Which is itself wrapped in the Slice class. Because we work at the bit level, there is a variable (AllocCharSizeRow) calculated as follows:

```

/* size of char allocation needed to represent COLUMNS bits */
this->AllocCharSizeRow = (numColumns / (sizeof(unsigned char) * 8));
this->AllocCharSizeRow += (((int)numColumns % (sizeof(unsigned char) * 8)) == 0)?0:1;

/* add 1 bit of empty padding on the left, and at least 1 bit on the right */
this->AllocCharSizeRow += (((this->AllocCharSizeRow*8)-columns) < 2)?1:0;

this->gridSliceAlloc();
} ```

Finally, the most important part: moving the grid from one step to the next depending on the 512 letter rule given. Note that because the grids are so small in terms of physical memory (for example a 100x100 fits easily in the characters of this chapter ;) I can afford to keep them. Then i can do neat stuff such as as going backwards and trying different rules. This is what the class CellAutomata wraps. It's basically a vector of Slices.
