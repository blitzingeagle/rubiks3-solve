//
//  Combinatorics.hpp
//  Rubiks Cube Solver
//
//  Created by Morris Chen on 2017-07-27.
//  Copyright © 2017 Morris Chen. All rights reserved.
//

#ifndef Combinatorics_hpp
#define Combinatorics_hpp

// Combinatorial algorithms

int nCr(int n, int r);
int PermutationToOrdinal(int *vector, int n);
void OrdinalToPermutation(int Ordinal, int* vector, int n, int offset);

#endif /* Combinatorics_hpp */
