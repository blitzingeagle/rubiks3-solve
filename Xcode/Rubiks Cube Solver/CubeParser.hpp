//
//  CubeParser.hpp
//  Rubiks Cube Solver
//
//  Created by Morris Chen on 2017-07-26.
//  Copyright © 2017 Morris Chen. All rights reserved.
//

#ifndef CubeParser_hpp
#define CubeParser_hpp

//
// Parse the cube state passed in on the command line
// checking for gross syntax errors.  For example, all
// 9 facelet markings for each of the six sides must
// be specified, and markings must be a printable ASCII
// character.  If the parse was successful a "FaceletCube"
// is initialized.  The FaceletCube represents the cube
// by the markings of the 54 individual facelets.  The
// FaceletCube can then be asked to validate the cube
// to determine if it is in a legal, and thus solvable,
// configuration.
//

#include "FaceletCube.hpp"

#include <string>

using namespace std;

class CubeParser {
public:
    // Parser return codes
    enum {
        VALID,
        INVALID_FACE,
        INVALID_MARKER,
        INCOMPLETE_INPUT,
        SYNTAX_ERROR,
        NumberOfErrors
    };
    
    CubeParser();
    ~CubeParser();
    
    // Parse the input and initialize a FaceletCube
    int parseFacelets(string faceletStrings[], FaceletCube &faceletCube);
    
    // Return the text associated with an error return code
    string ErrorText(unsigned int error);
    
private:
    // Return the text associated with an error return code
    int parseFace(FaceletCube &faceletCube, string faceString, int &face);
    
    static string errorText[NumberOfErrors];
};

#endif /* CubeParser_hpp */
