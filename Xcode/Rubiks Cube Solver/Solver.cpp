//
//  Solver.cpp
//  Rubiks Cube Solver
//
//  Created by Morris Chen on 2017-07-27.
//  Copyright © 2017 Morris Chen. All rights reserved.
//

#include "Solver.hpp"

#include <iostream>

using namespace std;

Solver::Solver(void)
// Phase 1 move mapping tables
: twistMoveTable(cube), flipMoveTable(cube), choiceMoveTable(cube),
// Phase 2 move mapping tables
cornerPermutationMoveTable(cube), nonMiddleSliceEdgePermutationMoveTable(cube), middleSliceEdgePermutationMoveTable(cube),

// Phase 1 pruning tables
TwistAndFlipPruningTable(
                         twistMoveTable, flipMoveTable,
                         cube.Twist(), cube.Flip()),
TwistAndChoicePruningTable(
                           twistMoveTable, choiceMoveTable,
                           cube.Twist(), cube.Choice()),
FlipAndChoicePruningTable(
                          flipMoveTable, choiceMoveTable,
                          cube.Flip(), cube.Choice()),
// Phase 2 pruning tables
CornerAndSlicePruningTable(
                           cornerPermutationMoveTable, middleSliceEdgePermutationMoveTable,
                           cube.CornerPermutation(), cube.MiddleSliceEdgePermutation()),
EdgeAndSlicePruningTable(
                         nonMiddleSliceEdgePermutationMoveTable, middleSliceEdgePermutationMoveTable,
                         cube.NonMiddleSliceEdgePermutation(), cube.MiddleSliceEdgePermutation())
{
    minSolutionLength = Huge;	// Any solution disovered will look better than this one!
}

Solver::~Solver()
{
}

void Solver::InitializeTables(void)
{
    // Phase 1 move mapping tables
    
    cout << "Initializing TwistMoveTable" << endl;
    twistMoveTable.Initialize("Twist.mtb");
    cout << "Size = " << twistMoveTable.SizeOf() << endl;
    
    cout << "Initializing FlipMoveTable" << endl;
    flipMoveTable.Initialize("Flip.mtb");
    cout << "Size = " << flipMoveTable.SizeOf() << endl;
    
    cout << "Initializing ChoiceMoveTable" << endl;
    choiceMoveTable.Initialize("Choice.mtb");
    cout << "Size = " << choiceMoveTable.SizeOf() << endl;
    
    // Phase 2 move mapping tables
    
    cout << "Initializing CornerPermutationMoveTable" << endl;
    cornerPermutationMoveTable.Initialize("CrnrPerm.mtb");
    cout << "Size = " << cornerPermutationMoveTable.SizeOf() << endl;
    
    cout << "Initializing NonMiddleSliceEdgePermutationMoveTable" << endl;
    nonMiddleSliceEdgePermutationMoveTable.Initialize("EdgePerm.mtb");
    cout << "Size = " << nonMiddleSliceEdgePermutationMoveTable.SizeOf() << endl;
    
    cout << "Initializing MiddleSliceEdgePermutationMoveTable" << endl;
    middleSliceEdgePermutationMoveTable.Initialize("SlicPerm.mtb");
    cout << "Size = " << middleSliceEdgePermutationMoveTable.SizeOf() << endl;
    
    // Phase 1 pruning tables
    
    cout << "Initializing TwistAndFlipPruningTable" << endl;
    TwistAndFlipPruningTable.Initialize("TwstFlip.ptb");
    cout << "Size = " << TwistAndFlipPruningTable.SizeOf() << endl;
    
    cout << "Initializing TwistAndChoicePruningTable" << endl;
    TwistAndChoicePruningTable.Initialize("TwstChce.ptb");
    cout << "Size = " << TwistAndChoicePruningTable.SizeOf() << endl;
    
    cout << "Initializing FlipAndChoicePruningTable" << endl;
    FlipAndChoicePruningTable.Initialize("FlipChce.ptb");
    cout << "Size = " << FlipAndChoicePruningTable.SizeOf() << endl;
    
    // Phase 2 pruning tables
    
    // Obviously a CornerAndEdgePruningTable doesn't make sense as it's size
    //   would be extremely large (i.e. 8!*8!)
    
    cout << "Initializing CornerAndSlicePruningTable" << endl;
    CornerAndSlicePruningTable.Initialize("CrnrSlic.ptb");
    cout << "Size = " << CornerAndSlicePruningTable.SizeOf() << endl;
    
    cout << "Initializing EdgeAndSlicePruningTable" << endl;
    EdgeAndSlicePruningTable.Initialize("EdgeSlic.ptb");
    cout << "Size = " << EdgeAndSlicePruningTable.SizeOf() << endl;
}

int Solver::Solve(RubiksCube& scrambledCube)
{
    int iteration = 1;
    int result = NOT_FOUND;
    
    // Make a copy of the scrambled cube for use later on
    cube = scrambledCube;
    
    // Establish initial cost estimate to goal state
    threshold1 = Phase1Cost(cube.Twist(), cube.Flip(), cube.Choice());
    
    nodes1 = 1;		// Count root node here
    solutionLength1 = 0;
    
    do
    {
        cout << "threshold(" << iteration
        << ") = " << threshold1 << endl;
        
        newThreshold1 = Huge;	// Any cost will be less than this
        
        // Perform the phase 1 recursive IDA* search
        result = Search1(cube.Twist(), cube.Flip(), cube.Choice(), 0);
        
        // Establish a new threshold for a deeper search
        threshold1 = newThreshold1;
        
        // Count interative deepenings
        iteration++;
    } while (result == NOT_FOUND);
    
    cout << "Phase 1 nodes = " << nodes1 << endl;
    return result;
}

int Solver::Search1(int twist, int flip, int choice, int depth)
{
    int cost, totalCost;
    int move;
    int power;
    int twist2, flip2, choice2;
    int result;
    
    // Compute cost estimate to phase 1 goal state
    cost = Phase1Cost(twist, flip, choice);	// h
    
    if (cost == 0)	// Phase 1 solution found...
    {
        solutionLength1 = depth;	// Save phase 1 solution length
        
        // We need an appropriately initialized cube in order
        //   to begin phase 2.  First, create a new cube that
        //   is a copy of the initial scrambled cube.  Then we
        //   apply the phase 1 move sequence to that cube.  The
        //   phase 2 search can then determine the initial
        //   phase 2 coordinates (corner, edge, and slice
        //   permutation) from this cube.
        //
        //   Note: No attempt is made to merge moves of the same
        //   face adjacent to the phase 1 & phase 2 boundary since
        //   the shorter sequence will quickly be found.
        
        RubiksCube phase2Cube = cube;
        for (int i = 0; i < solutionLength1; i++)
        {
            for (power = 0; power < solutionPowers1[i]; power++)
                phase2Cube.ApplyMove(solutionMoves1[i]);
        }
        // Invoke Phase 2
        (void)Solve2(phase2Cube);
    }
    
    // See if node should be expanded
    totalCost = depth + cost;	// g + h
    
    if (totalCost <= threshold1)	// Expand node
    {
        // If this happens, we should have found the
        //   optimal solution at this point, so we
        //   can exit indicating such.  Note: the first
        //   complete solution found in phase1 is optimal
        //   due to it being an addmissible IDA* search.
        if (depth >= minSolutionLength-1)
            return OPTIMUM_FOUND;
        
        for (move = Cube::Move::R; move <= Cube::Move::B; move++)
        {
            if (Disallowed(move, solutionMoves1, depth)) continue;
            
            twist2  = twist;
            flip2   = flip;
            choice2 = choice;
            
            solutionMoves1[depth] = move;
            for (power = 1; power < 4; power++)
            {
                solutionPowers1[depth] = power;
                twist2  = twistMoveTable[twist2][move];
                flip2   = flipMoveTable[flip2][move];
                choice2 = choiceMoveTable[choice2][move];
                nodes1++;
                // Apply the move
                if((result = Search1(twist2, flip2, choice2, depth+1)))
                    return result;
            }
        }
    }
    else	// Maintain minimum cost exceeding threshold
    {
        if (totalCost < newThreshold1)
            newThreshold1 = totalCost;
    }
    return NOT_FOUND;
}

int Solver::Solve2(RubiksCube& cube)
{
    int iteration = 1;
    int result = NOT_FOUND;
    
    // Establish initial cost estimate to goal state
    threshold2 = Phase2Cost(
                            cube.CornerPermutation(),
                            cube.NonMiddleSliceEdgePermutation(),
                            cube.MiddleSliceEdgePermutation());
    
    nodes2 = 1;		// Count root node here
    solutionLength2 = 0;
    
    do
    {
        newThreshold2 = Huge;	// Any cost will be less than this
        
        // Perform the phase 2 recursive IDA* search
        result = Search2(
                         cube.CornerPermutation(),
                         cube.NonMiddleSliceEdgePermutation(),
                         cube.MiddleSliceEdgePermutation(), 0);
        
        // Establish a new threshold for a deeper search
        threshold2 = newThreshold2;
        
        // Count interative deepenings
        iteration++;
    } while (result == NOT_FOUND);
    
    //	cout << "Phase 2 nodes = " << nodes2 << endl;
    return result;
}

int Solver::Search2(int cornerPermutation, int nonMiddleSliceEdgePermutation, int middleSliceEdgePermutation, int depth) {
    int cost, totalCost;
    int move;
    int power, powerLimit;
    int	cornerPermutation2;
    int nonMiddleSliceEdgePermutation2;
    int middleSliceEdgePermutation2;
    int result;
    
    // Compute cost estimate to goal state
    cost = Phase2Cost(cornerPermutation, nonMiddleSliceEdgePermutation, middleSliceEdgePermutation);	// h
    
    if (cost == 0) {	// Solution found...
        solutionLength2 = depth;	// Save phase 2 solution length
        if (solutionLength1 + solutionLength2 < minSolutionLength)
            minSolutionLength = solutionLength1 + solutionLength2;
        PrintSolution();
        return FOUND;
    }
    
    // See if node should be expanded
    totalCost = depth + cost;	// g + h
    
    if (totalCost <= threshold2) {	// Expand node
        // No point in continuing to search for solutions of equal or greater
        //   length than the current best solution
        if (solutionLength1 + depth >= minSolutionLength-1) return ABORT;
        
        for (move = Cube::Move::R; move <= Cube::Move::B; move++) {
            if (Disallowed(move, solutionMoves2, depth)) continue;
            
            cornerPermutation2 = cornerPermutation;
            nonMiddleSliceEdgePermutation2 = nonMiddleSliceEdgePermutation;
            middleSliceEdgePermutation2 = middleSliceEdgePermutation;
            
            solutionMoves2[depth] = move;
            powerLimit = 4;
            if (move != Cube::Move::U && move != Cube::Move::D) powerLimit=2;
            
            for (power = 1; power < powerLimit; power++) {
                cornerPermutation2 =
                cornerPermutationMoveTable[cornerPermutation2][move];
                nonMiddleSliceEdgePermutation2 =
                nonMiddleSliceEdgePermutationMoveTable[nonMiddleSliceEdgePermutation2][move];
                middleSliceEdgePermutation2 =
                middleSliceEdgePermutationMoveTable[middleSliceEdgePermutation2][move];
                
                solutionPowers2[depth] = power;
                
                nodes2++;
                // Apply the move
                if((result = Search2(cornerPermutation2, nonMiddleSliceEdgePermutation2, middleSliceEdgePermutation2, depth+1)))
                    return result;
            }
        }
    } else {	// Maintain minimum cost exceeding threshold
        if (totalCost < newThreshold2)
            newThreshold2 = totalCost;
    }
    return NOT_FOUND;
}

int Solver::Phase1Cost(int twist, int flip, int choice) {
    // Combining admissible heuristics by taking their maximum
    //   produces an improved admissible heuristic.
    int cost = TwistAndFlipPruningTable.GetValue(twist*flipMoveTable.SizeOf()+flip);
    int cost2 = TwistAndChoicePruningTable.GetValue(twist*choiceMoveTable.SizeOf()+choice);
    if (cost2 > cost) cost = cost2;
    cost2 = FlipAndChoicePruningTable.GetValue(flip*choiceMoveTable.SizeOf()+choice);
    if (cost2 > cost) cost = cost2;
    return cost;
}

int Solver::Phase2Cost(int cornerPermutation, int nonMiddleSliceEdgePermutation, int middleSliceEdgePermutation) {
    // Combining admissible heuristics by taking their maximum
    //   produces an improved admissible heuristic.
    int cost = CornerAndSlicePruningTable.GetValue(cornerPermutation*middleSliceEdgePermutationMoveTable.SizeOf()+middleSliceEdgePermutation);
    int cost2 = EdgeAndSlicePruningTable.GetValue(nonMiddleSliceEdgePermutation*middleSliceEdgePermutationMoveTable.SizeOf()+middleSliceEdgePermutation);
    if (cost2 > cost) cost = cost2;
    return cost;
}

int Solver::Disallowed(int move, int *solutionMoves, int depth) {
    if (depth > 0) {
        // Disallow successive moves of a single face (RR2 is same as R')
        if (solutionMoves[depth-1] == move)
            return 1;
        
        //   Disallow a move of an opposite face if the current face
        //     moved is B,L, or D. (BF, LR, DU are same as FB,RL,UD)
        if((move == Cube::Move::F) && solutionMoves[depth-1] == Cube::Move::B)
            return 1;
        if((move == Cube::Move::R) && solutionMoves[depth-1] == Cube::Move::L)
            return 1;
        if((move == Cube::Move::U) && solutionMoves[depth-1] == Cube::Move::D)
            return 1;
        
        // Disallow 3 or more consecutive moves of opposite faces
        //   (UDU is same as DU2 and U2D)
        if((depth > 1) && solutionMoves[depth-2] == move &&
            solutionMoves[depth-1] == Cube::OpposingFace(move))
            return 1;
    }
    return 0;	// This move is allowed
}

void Solver::PrintSolution(void) {
    for(int i = 0; i < solutionLength1; i++)
        cout << Cube::NameOfMove(TranslateMove(solutionMoves1[i], solutionPowers1[i], 0)) << " ";
    cout << ". "; // Separates phase1 and phase2 portion of the solution
    for(int i = 0; i < solutionLength2; i++)
        cout << Cube::NameOfMove(TranslateMove(solutionMoves2[i], solutionPowers2[i], 1)) << " ";
    cout << "(" << solutionLength1 + solutionLength2 << ")" << endl;
}

int Solver::TranslateMove(int move, int power, int phase2) {
    int translatedMove = move;
    
    if (phase2 && move != Cube::Move::U && move != Cube::Move::D)
        power = 2;
    
    if (power == 2)
        translatedMove = Cube::QuarterTurnToHalfTurnMove(move);
    else if (power == 3)
        translatedMove = Cube::InverseOfMove(move);
    else ;
    
    return translatedMove;
}


