// win32_entry_point.cpp

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <windows.h>

int vtfvMain(int argc, char** argv);

#ifdef VTFV_DEBUG
#   define _CRTDBG_MAP_ALLOC
#   include <crtdbg.h>
#   include <stdlib.h>

int main(int argc, char** argv)
{
    int ret = vtfvMain(argc, argv);
    _CrtDumpMemoryLeaks();
    return ret;
}

#elif defined(VTFV_RELEASE)

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    return vtfvMain(__argc, __argv);
}

#endif