# nKaruga

nKaruga originated on the TI-Nspire calculator series as a C++ game that I started writing order to get better at programming games. The now outdated code to this version is freely readable at [its own repository](https://github.com/matrefeytontias/nKaruga-deprecated-Nspire-version-). It is not out of the question that I port this game back to its original platform when I am done with it.

## Playing nKaruga

The latest released version of nKaruga is playable for free [on GameJolt](http://gamejolt.com/games/nkaruga/92026), and it is highly advised to do so if you just want to play the game. The code on this repository **is not** the code for the latest playable build.
Everything you need to know to play the game is explained there.

## Building nKaruga

nKaruga uses its own graphics engine, n2DLib, built on top of SDL2. It also uses SDL2_mixer to handle sound, thus, those two libraries are necessary. In this repository, I have included everything one should need to build nKaruga out of the box, including the SDL2 and SDL2_mixer binaries for Windows and Linux systems, regardless of their platform (i686 or x86_64). **However**, this means that the licences in this repository are not uniform : the SDL2 and SDL2_mixer binaries, SO and include files are subject to their own licences (check the SDL2 and SDL2_mixer websites for more info). The entirety of the nKaruga source files, apart from the aforementionned files, is under the MIT licence.

### Building on Linux

nKaruga should be buildable on Linux out of the box. Simply clone the repository in a new folder, and run `make linux`. **Building nKaruga does not require to install SDL2, SDL2-dev, SDL2_mixer or SDL2_mixer-dev.** The necessary files are included in the repository.

You will need `zip` in order to run `make deploy`.

### Building on Windows

In order to build nKaruga on Windows, you will need a functionning installation of MSYS2. Such an installation can be acquired by following the steps on the [main website](http://msys2.github.io/). Once MSYS2 is installed, you will need the following dependencies :

- gcc : `pacman -S gcc`
- make : `pacman -S make`
- Optionally, zip (to run `make deploy`) : `pacman -S zip`

Once all of this is installed, you can build nKaruga by running `make windows` or `make` in the repository's directory.

### Build targets

- `make` or `make windows` : build nKaruga for Windows platforms (works on all platforms that support MSYS2, ie Windows XP excluded onwards)
- `make linux` : build nKaruga for Linux platforms (should work on all platforms where SDL2 and SDL2_mixer work ; tested with Ubuntu 32 bits and KDE 64 bits)
- `make deploy` : package a build as `releases/nKaruga.zip` (change `RELEASE_DIR` and `OUT_NAME` in the makefile to change this)
- `make clean` : cleanup build files (includes executable, dependencies and .o obj files)
