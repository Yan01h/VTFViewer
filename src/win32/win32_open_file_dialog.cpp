// win32_open_file_dialog.cpp

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#include "core/open_file_dialog.h"

#include <windows.h>
#include <commdlg.h>

#include "debug/debug.h"

bool OpenFileDialog(std::string& path)
{
    path.reserve(512);

    OPENFILENAMEA openFileName = { 0 };
    openFileName.lStructSize = sizeof(OPENFILENAMEA);
    openFileName.lpstrFilter = "Valve Texture Format\0*.vtf\0";
    openFileName.lpstrFile = path.data();
    openFileName.lpstrFile[0] = '\0';
    openFileName.nMaxFile = path.capacity();
    openFileName.nFilterIndex = 1;
    openFileName.Flags = OFN_PATHMUSTEXIST | OFN_PATHMUSTEXIST;

    BOOL ret = GetOpenFileNameA(&openFileName);
    if (!ret)
        return false;

    VTFV_LOG_INFO("OFN: Selected file: %s", path.c_str());
    return true;
}