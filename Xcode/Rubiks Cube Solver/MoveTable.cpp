//
//  MoveTable.cpp
//  Rubiks Cube Solver
//
//  Created by Morris Chen on 2017-07-27.
//  Copyright © 2017 Morris Chen. All rights reserved.
//

#include "MoveTable.hpp"

#include <iostream>
#include <iomanip>

using namespace std;

MoveTable::MoveTable(Cube& cube, int tableSize, int phase2)
: TheCube(cube), TableSize(tableSize), Phase2(phase2) {
    // Allocate table storage
    Table = (int (*)[Cube::Move::NumberOfClockwiseQuarterTurnMoves]) // the cast
    (new int[TableSize*Cube::Move::NumberOfClockwiseQuarterTurnMoves]); // the allocation
}

MoveTable::~MoveTable() {
    delete [] Table;
}

void MoveTable::Initialize(char* fileName) {
    ifstream infile(fileName, ios::in|ios::binary);
    if (!infile) {	// If the move mapping table file is absent...
        // Generate the table and save it to a file
        cout << "Generating" << endl;
        Generate();
        cout << "Saving" << endl;
        Save(fileName);
        cout << "Done Saving" << endl;
    } else { // The move mapping table file exists
        // Load the existing file
        cout << "Loading" << endl;
        Load(infile);
    }
}

// Generate the table
void MoveTable::Generate() {
    int ordinal;
    int move, move2;
    
    // Insure the cubies are in their proper slice
    TheCube.BackToHome();
    
    // Initialize each table entry
    for (ordinal = 0; ordinal < TableSize; ordinal++) {
        // Establish the proper cube state for the current ordinal
        OrdinalToCubeState(ordinal);
        
        // Initialize the possible moves for each entry
        for (move = Cube::Move::R; move <= Cube::Move::B; move++) {
            // Apply this move
            
            // Phase 1 is the group spanned by <U,D,R,L,F,B>
            // Phase 2 is the group spanned by <U,D,R2,L2,F2,B2>
            move2 = move;
            if (Phase2 && move != Cube::Move::U && move != Cube::Move::D)
                move2 = Cube::QuarterTurnToHalfTurnMove(move);
            
            TheCube.ApplyMove(move2);
            
            // Compute a new ordinal from the new cube state
            Table[ordinal][move] = OrdinalFromCubeState();
            // Unapply this move
            TheCube.ApplyMove(Cube::InverseOfMove(move2));
        }
    }
}

int* MoveTable::operator[](int index) {
    return Table[index];
}

void MoveTable::Save(char* fileName) {
    ofstream outfile(fileName, ios::out|ios::binary);
    int index;
    for (index = 0; index < TableSize; index++)
        outfile.write((char*)&Table[index], Cube::Move::NumberOfClockwiseQuarterTurnMoves*sizeof(int));
}

void MoveTable::Load(ifstream &infile) {
    for(int index = 0; index < TableSize; index++)
        infile.read((char*)&Table[index], Cube::Move::NumberOfClockwiseQuarterTurnMoves*sizeof(int));
}

// Output the move table in human readable form
void MoveTable::Dump() {
    int ordinal;
    int move, move2;
    // For each table entry...
    for(ordinal = 0; ordinal < TableSize; ordinal++) {
        cout << setw(8) << ordinal << ": ";
        // For each possible move...
        for(move = Cube::Move::R; move <= Cube::Move::B; move++) {
            move2 = move;
            if(Phase2) {
                if (move != Cube::Move::U && move != Cube::Move::D)
                    move2 = Cube::QuarterTurnToHalfTurnMove(move);
            }
            cout << Cube::NameOfMove(move2) << ":" << setw(8) << Table[ordinal][move] << " ";
        }
        cout << endl;
    }
}
