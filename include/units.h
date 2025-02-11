#include <gba.h>

// Amount of subpixels in one visible pixel
#define PIXEL_SIZE 4
// Amount of pixels in one block
#define BLOCK_SIZE_PIXELS 16

// Size of a block in subpixels
#define BLOCK_SIZE (BLOCK_SIZE_PIXELS * PIXEL_SIZE)
// Half the size of a block, in subpixels
#define HALF_BLOCK_SIZE (BLOCK_SIZE / 2)
// One quarter the size of a block, in subpixels
#define QUARTER_BLOCK_SIZE (BLOCK_SIZE / 4)

#define SUBPIXELS_FROM_PIXELS(pixels) ((pixels) * PIXEL_SIZE)
#define SUBPIXELS_FROM_BLOCKS(blocks) ((blocks) * BLOCK_SIZE)

#define PIXELS_FROM_SUBPIXELS(subpixels) ((subpixels) / PIXEL_SIZE)
#define PIXELS_FROM_BLOCKS(blocks) ((blocks) * BLOCK_SIZE_PIXELS)

#define BLOCKS_FROM_SUBPIXELS(subpixels) ((subpixels) / BLOCK_SIZE)
#define BLOCKS_FROM_PIXELS(pixels) ((pixels) / BLOCK_SIZE_PIXELS)

// Number of points per internal unit of score
#define SCORE_RATIO 10
// Convert an amount of points for storage
#define CONVERT_SCORE(points) ((points) / SCORE_RATIO)

// Target FPS
#define FRAME_RATE 60
// Convert a floating-point value of seconds into frames
#define CONVERT_SECONDS(seconds) ((u32) ((seconds) * FRAME_RATE))
