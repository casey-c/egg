#ifndef ALLCOMMANDS_H
#define ALLCOMMANDS_H

/*
 * This is a convenience header to include all commands with a single .h
 * include.
 *
 * TODO: multiple convienience headers, one for each object that the group of
 * commands acts on (e.g. one for all TreeState commands, etc.)
 */

#include "Utilities/Command/ctreestateaddstatement.h"
#include "Utilities/Command/ctreestateaddcut.h"
#include "Utilities/Command/ctreestateselectachild.h"
#include "Utilities/Command/ctreestateselectleft.h"
#include "Utilities/Command/ctreestateselectparent.h"
#include "Utilities/Command/ctreestateselectright.h"
#include "Utilities/Command/ctreestateselectroot.h"
#include "Utilities/Command/ctreestateaddbiconditionaltemplate.h"
#include "Utilities/Command/ctreestateaddconditionaltemplate.h"
#include "Utilities/Command/ctreestateaddortemplate.h"
#include "Utilities/Command/ctreestateadddoublecut.h"

#endif // ALLCOMMANDS_H
