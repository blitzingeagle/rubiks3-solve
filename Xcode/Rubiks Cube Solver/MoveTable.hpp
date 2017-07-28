//
//  MoveTable.hpp
//  Rubiks Cube Solver
//
//  Created by Morris Chen on 2017-07-27.
//  Copyright © 2017 Morris Chen. All rights reserved.
//

#ifndef MoveTable_hpp
#define MoveTable_hpp

#include "Cube.hpp"

#include <fstream>

using namespace std;

class MoveTable {
public:
    // The constructor must be provided with a cube to be
    //   manipulated during table generation, the size
    //   of the table (number of entries), and whether
    //   or not the table is a phase 2 table.  If the
    //   table is a phase 2 table, then only quarter
    //   turn moves are allowed for F,B,L, and R.
    MoveTable(Cube &cube, int tableSize, int phase2=0);
    
    virtual ~MoveTable();
    
    // Initialize the pruning table by either generating it
    //   or loading it from an existing file
    virtual void Initialize(char* fileName);
    
    // Overloaded subscript operator allows standard C++ indexing
    //   (i.e. MoveTable[i][j]) for accessing table values.
    virtual int* operator[](int index);
    
    // Obtain the size of the table (number of logical entries)
    virtual int SizeOf() { return TableSize; }
    
    // Dump table contents
    virtual void Dump();
    
protected:
    // These functions must be overloaded in the derived
    //   class in order to provide the appropriate mapping
    //   between ordinal and cube state.
    virtual int OrdinalFromCubeState() = 0;
    virtual void OrdinalToCubeState(int ordinal) = 0;
    
private:
    // Generate the table
    void Generate();
    // Save the table to a file
    void Save(char* fileName);
    // Load the table from a file
    void Load(ifstream& infile);
    
    // Copies of important variables
    Cube& TheCube;
    // Number of entries in the pruning table
    int TableSize;
    int Phase2;
    // The table pointer
    int (*Table)[Cube::Move::NumberOfClockwiseQuarterTurnMoves];
};


#endif /* MoveTable_hpp */
