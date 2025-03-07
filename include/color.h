#pragma once

#define COLOR_BITS 5
#define TRUE_COLOR_BITS 8
#define COLOR_MASK ((1 << COLOR_BITS) - 1)
#define TRUE_COLOR_MASK ((1 << TRUE_COLOR_BITS) - 1)
#define CONVERT_COLOR_CHANNEL(color8b) (((color8b) >> (TRUE_COLOR_BITS - COLOR_BITS)) & COLOR_MASK)

// 15-bit GBA high color
#define COLOR(r, g, b) (((b) << (2 * COLOR_BITS)) | ((g) << COLOR_BITS) | (r))
// Convert 24-bit true color to a 15-bit high color
#define CONVERT_COLOR(r, g, b) COLOR(CONVERT_COLOR_CHANNEL(r), CONVERT_COLOR_CHANNEL(g), CONVERT_COLOR_CHANNEL(b))
// Convert 24-bit hex true color to a 15-bit high color
#define CONVERT_COLOR_HEX(hex) CONVERT_COLOR((hex >> (2 * TRUE_COLOR_BITS)) & TRUE_COLOR_MASK, (hex >> TRUE_COLOR_BITS) & TRUE_COLOR_MASK, hex & TRUE_COLOR_MASK)

#define COLOR_BLACK COLOR(0, 0, 0)
#define COLOR_WHITE COLOR(COLOR_MASK, COLOR_MASK, COLOR_MASK)

#define COLOR_AP_CYAN CONVERT_COLOR_HEX(0x00EEEE)
#define COLOR_AP_SLATEBLUE CONVERT_COLOR_HEX(0x6D8BE8)
#define COLOR_AP_PLUM CONVERT_COLOR_HEX(0xAF99EF)
#define COLOR_AP_SALMON CONVERT_COLOR_HEX(0xFA8072)
