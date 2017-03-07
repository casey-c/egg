#ifndef ALLCOMMANDS_H
#define ALLCOMMANDS_H

/*
 * This is a convenience header to include all commands with a single .h
 * include.
 *
 * TODO: multiple convienience headers, one for each object that the group of
 * commands acts on (e.g. one for all TreeState commands, etc.)
 */

// Add commands
#include "Utilities/Command/ctreestateaddstatement.h"
#include "Utilities/Command/ctreestateaddcut.h"
#include "Utilities/Command/ctreestateaddbiconditionaltemplate.h"
#include "Utilities/Command/ctreestateaddconditionaltemplate.h"
#include "Utilities/Command/ctreestateaddortemplate.h"
#include "Utilities/Command/ctreestateadddoublecut.h"

// Highlight commands
#include "Utilities/Command/ctreestatehighlightchild.h"
#include "Utilities/Command/ctreestatehighlightleft.h"
#include "Utilities/Command/ctreestatehighlightparent.h"
#include "Utilities/Command/ctreestatehighlightright.h"
#include "Utilities/Command/ctreestatehighlightroot.h"
#include "Utilities/Command/ctreestatepounce.h"

// Surround commands
#include "Utilities/Command/ctreestatesurroundwithcut.h"
#include "Utilities/Command/ctreestatesurroundwithdoublecut.h"

// Select commands
#include "Utilities/Command/ctreestateclearselection.h"
#include "Utilities/Command/ctreestatedeselecthighlighted.h"
#include "Utilities/Command/ctreestateselectchildren.h"
#include "Utilities/Command/ctreestateselecthighlighted.h"
#include "Utilities/Command/ctreestaterevertselection.h"

// Remove/detach commands
#include "Utilities/Command/ctreestatedetachnode.h"
#include "Utilities/Command/ctreestatedetachnodebutsavechildren.h"

#endif // ALLCOMMANDS_H
