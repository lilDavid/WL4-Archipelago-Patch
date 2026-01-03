#!/usr/bin/env python3

from argparse import ArgumentParser
import itertools
import math
from pathlib import Path
import struct
from typing import cast
from PIL import Image


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


def convert_color(r: int, g: int, b: int):
    r >>= 3
    g >>= 3
    b >>= 3
    return (b << 5 | g) << 5 | r


# https://docs.python.org/3.11/library/itertools.html
def batches(iterable, n):
    if n < 1:
        raise ValueError
    it = iter(iterable)
    while batch := tuple(itertools.islice(it, n)):
        yield batch


def create_gfx(in_path: Path):
    out_path = in_path.with_suffix(".gfx")

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
                    pixels.append(cast(int, color) & 0xF)

    with open(out_path, "wb") as out_file:
        out_file.write(bytes(map(lambda p: p[1] << 4 | p[0], batches(pixels, 2))))


def create_pal(in_path: Path):
    out_path = in_path.with_suffix(".pal")

    source_image = Image.open(in_path)
    if source_image.palette is None:
        raise ValueError("Image is not in indexed color format")

    palette = [
        convert_color(*color[:3]) for color in source_image.palette.colors.keys()
    ]
    size = math.ceil(len(palette) / 16) * 16
    palette.extend(0 for _ in range(size - len(palette)))

    with open(out_path, "wb") as out_file:
        out_file.write(struct.pack(f"<{size}H", *palette))


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("-p", "--palette", action="store_true")
    parser.add_argument("input_file", type=Path)
    args = parser.parse_args()

    if args.palette:
        create_pal(args.input_file)
    else:
        create_gfx(args.input_file)
