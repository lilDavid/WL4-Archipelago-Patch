#include <gba.h>

#include "vanilla/variables.h"
#include "multiworld.h"

void InitializeVariables() {
    DeathLinkEnabled = DeathLinkFlag;
    MultiworldState = 0;
    TextTimer = 0;

    // From vanilla
    KeyPressContinuous = 0;
    KeyPressPrevious = 0;
    usTrg_KeyPress1Frame = 0;
    sGameSeq = 0;
}
