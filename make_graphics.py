#!/usr/bin/env python3

import itertools
from pathlib import Path
from typing import Mapping, Sequence
from PIL import Image
import sys


# This script converts a PNG image into the uncompressed 4bpp indexed format
# Wario Land 4 uses. To do that, it uses a .palmap text file which maps RGB/RGBA
# colors to a 4-bit color index.
#
# The image is converted into 8x8 pixel tiles, which are then stored in a 1D
# array format in row major order, as in the simplified example here:
#
# 00000000 44444444
# 11111111 55555555
# 22222222 66666666
# 33333333 77777777
#
# 88888888 CCCCCCCC
# 99999999 DDDDDDDD
# AAAAAAAA EEEEEEEE
# BBBBBBBB FFFFFFFF


def pixels_to_tiles(pixels: int):
    if pixels % 8 != 0:
        raise ValueError(pixels)
    return pixels // 8


# https://docs.python.org/3.11/library/itertools.html
def batches(iterable, n):
    if n < 1:
        raise ValueError
    it = iter(iterable)
    while batch := tuple(itertools.islice(it, n)):
        yield batch


def main():
    in_path = Path(sys.argv[1]).with_suffix('.png')
    out_path = in_path.with_suffix('.gfx')

    source_image = Image.open(in_path)
    if source_image.palette is None:
        raise ValueError("Image is not in indexed color format")

    width, height = map(pixels_to_tiles, source_image.size)
    pixels = []
    for row in range(height):
        for column in range(width):
            for y in range(8):
                for x in range(8):
                    color = source_image.getpixel((8 * column + x, 8 * row + y))
                    pixels.append(color & 0xF)

    with open(out_path, 'wb') as out_file:
        out_file.write(bytes(map(lambda p: p[1] << 4 | p[0], batches(pixels, 2))))


if __name__ == '__main__':
    main()
