# Wario Land 4 Archipelago base patch

Base patch for the Archipelago implementation for Wario Land 4: https://github.com/lilDavid/Archipelago-Wario-Land-4

## Building

To build the base patch, you will need:
- A Wario Land 4 ROM (English or Japanese is fine)
- `armips` and `bsdiff` on your path
- `python3-venv`

First, install devkitPro pacman using their setup instructions for your platform:
- https://devkitpro.org/wiki/Getting_Started
- https://devkitpro.org/wiki/devkitPro_pacman

Then install the Game Boy Advance toolchain:
```sh
sudo dkp-pacman -S gba-dev
```

Armips, the assembler and linker this project uses, can be found at https://github.com/Kingcom/armips/releases

After cloning the repository, set up a virtual environment for pillow to build the custom sprites:
```sh
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install pillow
```

Place your Wario Land 4 ROM in the root of the repository with the name `Wario Land 4.gba`, and finally run `make`, which will create the files `basepatch.bsdiff` and `basepatch.sym` in the `build` directory.
