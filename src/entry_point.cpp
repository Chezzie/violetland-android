/**
Attention!
This file has Oxygine initialization stuff.
If you just started you don't need to understand it exactly you could check it later.
You could start from example.cpp and example.h it has main functions being called from there
*/

#ifdef WIN32
#include "core/oxygine.h"
#endif
#include "Stage.h"
#include "DebugActor.h"
#include "SDL_main.h"

extern "C"
{
    int main(int argc, char* argv[])
    {
		extern int __main(int argc, char** argv);
        __main(argc, argv);
        return 0;
    }
};
