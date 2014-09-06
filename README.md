Blocker
=======

Blocker is another Tetris clone written for the sole purpose of learning how to program in C++, write games and use SDL.

This game requires my gamelibrary in order to run, see 'gamelibrary' repository.  At the moment i statically link all the code together, once i figure out and become comfortbale with dynamic linking i'll just include the .dll into the project instead requiring the 'gamelibrary' as well. 

All graphics have been created by the author.

Will probably add sounds soon.

The freeSans true type font file is from https://savannah.gnu.org/projects/freefont/

The build environment is:
Mingw32 + gmake + gvim
gcc (GCC) 4.4.7 20111023 (prerelease) [svn/rev.180339 - mingw-w64/oz]

Developed on:
Windows Vista - Home Premium, SP2

To clone use:

git clone --recursive https://github.com/SamCoulson/blocker.git
NOTE: recursive option will also clone the needed GameLibrary files.

To build:

gmake

To run:

blocker.exe

The game and gamelibrary requires SDL-1.2.15 library files to compile and SDL .dll runtime files to be present in either the blocker.exe directory or windows32 directory.

Available at: https://www.libsdl.org/download-1.2.php

Additional to this the image and font libraries are also required:

SDL_image-1.2.10
Available at: https://www.libsdl.org/projects/SDL_image/release-1.2.html

SDL_ttf-2.0.11
Available at: http://www.libsdl.org/projects/SDL_ttf/release-1.2.html




