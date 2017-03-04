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
}

#endif // UTILITIES_CONSTANTS_H
