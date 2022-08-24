// Application.h
/*
                     GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
 ...
*/

#pragma once

namespace VTFViewer {

    class Application final
    {
    public:
        Application();
        ~Application();

        void Run(int argc, char* argv[]);
        void Shutdown();
    private:
        bool m_Running;
    };

}