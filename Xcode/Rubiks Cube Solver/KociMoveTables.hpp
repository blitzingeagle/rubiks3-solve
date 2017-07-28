//
//  KociMoveTables.hpp
//  Rubiks Cube Solver
//
//  Created by Morris Chen on 2017-07-27.
//  Copyright © 2017 Morris Chen. All rights reserved.
//

#ifndef KociMoveTables_hpp
#define KociMoveTables_hpp

//
// Subclasses of "MoveTable" for each of the Kociemba
// move mapping tables.  Functions for converting between
// an ordinal and its associated cube state are specified
// for each class via overrides.
//

#include "MoveTable.hpp"
#include "RubiksCube.hpp"

// Phase 1 move mapping table classes

class TwistMoveTable : public MoveTable {
public:
    TwistMoveTable(RubiksCube& cube)
    : MoveTable(cube, RubiksCube::Twists, 0),
		  TheCube(cube) {}
private:
    inline int  OrdinalFromCubeState(void)
    { return TheCube.Twist(); }
    inline void OrdinalToCubeState(int ordinal)
    { TheCube.Twist(ordinal); }
    RubiksCube& TheCube;
};

class FlipMoveTable : public MoveTable
{
public:
    FlipMoveTable(RubiksCube& cube)
    : MoveTable(cube, RubiksCube::Flips, 0),
		  TheCube(cube) {}
private:
    inline int  OrdinalFromCubeState(void)
    { return TheCube.Flip(); }
    inline void OrdinalToCubeState(int ordinal)
    { TheCube.Flip(ordinal); }
    RubiksCube& TheCube;
};

class ChoiceMoveTable : public MoveTable
{
public:
    ChoiceMoveTable(RubiksCube& cube)
    : MoveTable(cube, RubiksCube::Choices, 0),
		  TheCube(cube) {}
private:
    inline int  OrdinalFromCubeState(void)
    { return TheCube.Choice(); }
    inline void OrdinalToCubeState(int ordinal)
    { TheCube.Choice(ordinal); }
    RubiksCube& TheCube;
};

// Phase 2 move mapping table classes

class CornerPermutationMoveTable : public MoveTable
{
public:
    CornerPermutationMoveTable(RubiksCube& cube)
    : MoveTable(cube, RubiksCube::CornerPermutations, 1),
		  TheCube(cube) {}
private:
    inline int  OrdinalFromCubeState(void)
    { return TheCube.CornerPermutation(); }
    inline void OrdinalToCubeState(int ordinal)
    { TheCube.CornerPermutation(ordinal); }
    RubiksCube& TheCube;
};

class NonMiddleSliceEdgePermutationMoveTable : public MoveTable
{
public:
    NonMiddleSliceEdgePermutationMoveTable(RubiksCube& cube)
    : MoveTable(cube, RubiksCube::NonMiddleSliceEdgePermutations, 1),
		  TheCube(cube) {}
private:
    inline int  OrdinalFromCubeState(void)
    { return TheCube.NonMiddleSliceEdgePermutation(); }
    inline void OrdinalToCubeState(int ordinal)
    { TheCube.NonMiddleSliceEdgePermutation(ordinal); }
    RubiksCube& TheCube;
};

class MiddleSliceEdgePermutationMoveTable : public MoveTable
{
public:
    MiddleSliceEdgePermutationMoveTable(RubiksCube& cube)
    : MoveTable(cube, RubiksCube::MiddleSliceEdgePermutations, 1),
		  TheCube(cube)	{}
private:
    inline int  OrdinalFromCubeState(void)
    { return TheCube.MiddleSliceEdgePermutation(); }
    inline void OrdinalToCubeState(int ordinal)
    { TheCube.MiddleSliceEdgePermutation(ordinal); }
    RubiksCube& TheCube;
};

#endif /* KociMoveTables_hpp */
