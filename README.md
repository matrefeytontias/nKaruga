# nKaruga

nKaruga originated on the TI-Nspire calculator series as a C++ game that I started writing order to get better at programming games. The now outdated code to this version is freely readable at [its own repository](https://github.com/matrefeytontias/nKaruga-deprecated-Nspire-version-). It is not out of the question that I port this game back to its original platform when I am done with it.

## Playing nKaruga

The latest released version of nKaruga is playable for free [on GameJolt](http://gamejolt.com/games/nkaruga/92026), and it is highly advised to do so if you just want to play the game. The code on this repository **is not** the code for the latest playable build.
Everything you need to know to play the game is explained there.

## Building nKaruga

nKaruga uses its own graphics engine, n2DLib, built on top of SDL2. It also uses SDL2_mixer to handle sound, thus, those two libraries are necessary.

~In this repository, I have included everything one should need to build nKaruga out of the box, including the SDL2 and SDL2_mixer binaries for Windows and Linux systems, regardless of their platform (i686 or x86_64). **However**, this means that the licences in this repository are not uniform : the SDL2 and SDL2_mixer binaries, SO and include files are subject to their own licences (check the SDL2 and SDL2_mixer websites for more info).~

The entirety of the nKaruga source files, apart from the aforementionned files, is under the MIT licence.

*27/08/2022* : I have revamped the build system to use CMake with SDL2 2.24.0 and SDL2_mixer 2.6.2. For now, everything will be Windows-only until I get around to trying things out with the other platforms.

### Building on Windows

In order to build nKaruga on Windows, you will need CMake and the generator of your choice (I use Microsoft Visual Studio 2019). Building should be as simple as running CMake in the root directory and following the usual steps.
