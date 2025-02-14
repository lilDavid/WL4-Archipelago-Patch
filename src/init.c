#include <gba.h>

#include "unsorted/variables.h"
#include "game_state.h"
#include "multiworld.h"

void InitializeVariables() {
    MultiworldState = 0;
    TextTimer = 0;
    VblkStatus = 0;

    // From vanilla
    KeyPressContinuous = 0;
    KeyPressPrevious = 0;
    usTrg_KeyPress1Frame = 0;
    sGameSeq = 0;
}
