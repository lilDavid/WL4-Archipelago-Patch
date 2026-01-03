#include <gba.h>

#include "unsorted/functions.h"


// Get the length of the passed 0xFE-terminated, WL4 encoded string.
// Returns the number of bytes between the pointer and the byte value 0xFE, not
// counting the 0xFE byte.
u32 W4strlen(u8* w4str) {
    int i = 0;
    while (w4str[i] != 0xFE)
        i++;
    return i;
}

void LZ77UnCompVram(UNUSED const void* _src, UNUSED void* _dest) {
    __asm__("svc 18");
}
