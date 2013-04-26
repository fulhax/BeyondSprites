#include "engine.h"

int main(int argc, char const* argv[])
{
    if(gEngine.Init())
        gEngine.MainLoop();
    gEngine.Shutdown();

    return 0;
}
