//
//  RubiksCube.hpp
//  Rubiks Cube Solver
//
//  Created by Morris Chen on 2017-07-26.
//  Copyright © 2017 Morris Chen. All rights reserved.
//

#ifndef RubiksCube_hpp
#define RubiksCube_hpp

#include "Cube.hpp"

#define KOCIEMBA	// Selects either Kociemba's or Winter's approach
                    //   to the choice algorithm (comment out to choose
                    //   the latter).  If this is changed, be sure to
                    //   regenerate the tables involving "choice".

class RubiksCube : public Cube {
public:
    // Number of possiblities for each axis of the two triples
    enum {
        Twists = (3*3*3*3*3*3*3), // 3^7 = 2187
        Flips = (2*2*2*2*2*2*2*2*2*2*2), // 2^11 = 2048
        Choices = 495, // 12 choose 4 = 495
        CornerPermutations = (8*7*6*5*4*3*2*1), // 8! = 40320
        NonMiddleSliceEdgePermutations = (8*7*6*5*4*3*2*1), // 8! = 40320
        MiddleSliceEdgePermutations = (4*3*2*1) // 4! = 24
    };
    
    RubiksCube();
    ~RubiksCube();
    
    // Phase 1 triple
    
    // Corner orientations (3^7 = 2187)
    int Twist();
    void Twist(int twist);
    // Edge orientations (2^11 = 2048)
    int Flip();
    void Flip(int flip);
    // Four middle slice edge positions (12 choose 4 = 495)
    int Choice();
    void Choice(int choice);
    
    // Phase 2 triple
    
    // Permutation of the 8 corners (8! = 40320)
    int CornerPermutation();
    void CornerPermutation(int ordinal);
    // Permutation of the 8 non-middle slice edges (8! = 40320)
    int NonMiddleSliceEdgePermutation();
    void NonMiddleSliceEdgePermutation(int ordinal);
    // Permutation of the 4 middle slice edges (4! = 24)
    int MiddleSliceEdgePermutation();
    void MiddleSliceEdgePermutation(int ordinal);
    
private:
    enum {
        // A 12 bit number, 1 bit for each edge
        NumberOfEncodedChoicePermutations = 4096,
        // 12 choose 4 edges
        NumberOfChoiceOrdinals = 495
    };
    
    // Predicate to determine if a cubie is a middle slice edge cubie
    static int IsMiddleSliceEdgeCubie(int cubie);
    
#ifdef KOCIEMBA
    // Compute the choice ordinal from the choice permutation
    static int ChoiceOrdinal(int* choicePermutation);
    
    // Compute the choice permutation from the choice ordinal
    static void ChoicePermutation(int choiceOrdinal, int* choicePermutation);
    
#else // Dik Winter's method
    // Alternate approach to the above
    static int ChoiceOrdinal(int* choicePermutation);
    static void ChoicePermutation(int choiceOrdinal, int* choicePermutation);
    
    static void InitializeChoice();
    static int CountBits(int value);
    static void DecodeChoicePermutation(int encodedChoicePermutation, int* decodedChoicePermutation);
    
    static int EncodedChoicePermutationToChoiceOrdinal[NumberOfEncodedChoicePermutations];
    // Maps the ordinal back to the edge permutation
    static int ChoiceOrdinalToEncodedChoicePermutation[NumberOfChoiceOrdinals];
    // Maps a bit number in the range (0...12) to a bit mask
    static unsigned int BitNumberToBitMask[12];
#endif // KOCIEMBA
    
};

#endif /* RubiksCube_hpp */

