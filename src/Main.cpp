// Main.cpp
/*
                     GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
 ...
*/

#ifndef _WINDOWS
#error Windows only!
#endif // _WINDOWS

#include "Application.h"
#include "Utility.h"

#include <Windows.h>

#ifdef _DEBUG
int main(int argc, char* argv[])
#elif defined _RELEASE
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
    LOG("Starting VTFViewer...");

    VTFViewer::Application* mainApp = new VTFViewer::Application();
    mainApp->Run(argc, argv);
    delete mainApp;

    LOG("Closing VTFViewer...");
}