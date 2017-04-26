#ifndef UTILITIES_CONSTANTS_H
#define UTILITIES_CONSTANTS_H

namespace constants
{
    /*
     * Declare global constants in this file.
     *
     * To use them:
     * include "constants.h"
     * ...
     * int x = constants::ELEMENT_ROOT;
     */

    // Tree
    const int ELEMENT_ROOT = 0;
    const int ELEMENT_CUT = 1;
    const int ELEMENT_STATEMENT = 2;
    const int ELEMENT_PLACEHOLDER = 3;

    // Keybind modes
    const int MODE_DEFAULT = 0;
    const int MODE_Q = 1;
    const int MODE_SELECT = 2;
    const int MODE_POUNCE = 3;
    const int MODE_MOD = 4;

    // Types of modifications
    const int STEP_DC_ADD = 0;
    const int STEP_DC_REMOVE = 1;
    const int STEP_ITERATE = 2;
    const int STEP_DEITERATE = 3;

    // Types of ProofTreeNodes
    const int PT_PREMISE = 0;
    const int PT_GOAL = 1;
    const int PT_INNER = 2;
}

#endif // UTILITIES_CONSTANTS_H
