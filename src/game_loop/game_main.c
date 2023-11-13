#include "vanilla/variables.h"


#ifdef DEBUG

// While free movement is enabled:
//  - D-pad to move, hold R to move faster
//  - Press Start to center camera on Wario. Press Start again or exit free
//    movement to return the camera to normal.
//  - Press L + D-pad up to clear the level. RANDO: This will dupe already
//    collected items.
void Debug_PressSelectForFreeMovement() {
    if (usTrg_KeyPress1Frame & KEY_SELECT) {
        sGameSeq = 8;
    }
}

#endif
