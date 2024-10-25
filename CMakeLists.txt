cmake_minimum_required(VERSION 3.5.0)
project(in_NotSoFatso VERSION 0.1.0 LANGUAGES C CXX)

set(DFC
    DFC.h
    DFC_Array.cpp
    DFC_Array.h
    DFC_Dialog.cpp
    DFC_String.cpp
    DFC_String.h
    DFC_Wnd.cpp
)

source_group("DFC" FILES ${DFC})

set(HEADER_FILES
    NSF.h
    Winamp.h
)

source_group("Header Files" FILES ${HEADER_FILES})

set(HEADER_FILES_GUI
    AboutDlg.h
    ChannelsDlg.h
    ConfigDlg.h
    FileInfoDlg.h
    FME07Dlg.h
    GeneralFileInfoDlg.h
    MainControlDlg.h
    MiniPlayControlDlg.h
    MMC5Dlg.h
    N106Dlg.h
    PlayControlDlg.h
    PlaylistDlg.h
    TagInfoDlg.h
    TrackInfoDlg.h
    VRC6Dlg.h
    VRC7Dlg.h
)

source_group("Header Files/GUI" FILES ${HEADER_FILES_GUI})

set(NSF_CORE
    fmopl.c
    fmopl.h
    NSF_6502.cpp
    NSF_Core.cpp
    NSF_Core.h
    NSF_File.cpp
    NSF_File.h
    Wave_FDS.h
    Wave_FME07.h
    Wave_MMC5.h
    Wave_N106.h
    Wave_Square.h
    Wave_TND.h
    Wave_VRC6.h
    Wave_VRC7.cpp
)

source_group("NSF Core" FILES ${NSF_CORE})

set(RESOURCE_FILES
    NotSoLogo.bmp
    resource.h
    Resource.rc
    wannabewinamp.ico
)

source_group("Resource Files" FILES ${RESOURCE_FILES})

set(SOURCE_FILES_
    NotsoFatso.def
    NSF.cpp
    Winamp.cpp
)

source_group("Source Files" FILES ${SOURCE_FILES_})

set(SOURCE_FILES_GUI
    AboutDlg.cpp
    ChannelsDlg.cpp
    ConfigDlg.cpp
    FileInfoDlg.cpp
    FME07Dlg.cpp
    GeneralFileInfoDlg.cpp
    MainControlDlg.cpp
    MiniPlayControlDlg.cpp
    MMC5Dlg.cpp
    N106Dlg.cpp
    PlayControlDlg.cpp
    PlaylistDlg.cpp
    TagInfoDlg.cpp
    TrackInfoDlg.cpp
    VRC6Dlg.cpp
    VRC7Dlg.cpp
)

source_group("Source Files/GUI" FILES ${SOURCE_FILES_GUI})

add_library(in_NotSoFatso SHARED
    ${DFC}
    ${HEADER_FILES}
    ${HEADER_FILES_GUI}
    ${NSF_CORE}
    ${RESOURCE_FILES}
    ${SOURCE_FILES_}
    ${SOURCE_FILES_GUI}
    Readme.txt
)

target_link_libraries(in_NotSoFatso PRIVATE comctl32)
