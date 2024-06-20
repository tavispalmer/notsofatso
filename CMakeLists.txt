cmake_minimum_required(VERSION 3.5.0)
project(notsofatso VERSION 8.5.1 LANGUAGES C CXX)

# taken from SFML: https://github.com/SFML/SFML
macro(nsf_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

nsf_set_option(NSF_BUILD_WINAMP TRUE BOOL "TRUE to build the Winamp plugin")
nsf_set_option(NSF_BUILD_VLC FALSE BOOL "TRUE to build the VLC plugin")

set(DFC
    src/dfc/DFC.h
    src/dfc/DFC_Array.cpp
    src/dfc/DFC_Array.h
    src/dfc/DFC_Dialog.cpp
    src/dfc/DFC_String.cpp
    src/dfc/DFC_String.h
    src/dfc/DFC_Wnd.cpp
)
source_group("DFC" FILES ${DFC})

set(HEADER_FILES_GUI
    src/gui/AboutDlg.h
    src/gui/ChannelsDlg.h
    src/gui/ConfigDlg.h
    src/gui/FileInfoDlg.h
    src/gui/FME07Dlg.h
    src/gui/GeneralFileInfoDlg.h
    src/gui/MainControlDlg.h
    src/gui/MiniPlayControlDlg.h
    src/gui/MMC5Dlg.h
    src/gui/N106Dlg.h
    src/gui/PlayControlDlg.h
    src/gui/PlaylistDlg.h
    src/gui/TagInfoDlg.h
    src/gui/TrackInfoDlg.h
    src/gui/VRC6Dlg.h
    src/gui/VRC7Dlg.h
)
source_group("Header Files/GUI" FILES ${HEADER_FILES_GUI})

set(HEADER_FILES
    src/NSF.h
    src/Winamp.h
)
source_group("Header Files" FILES ${HEADER_FILES})

set(NSF_CORE
    src/core/fmopl.c
    src/core/fmopl.h
    src/core/NSF_6502.cpp
    src/core/NSF_Core.cpp
    src/core/NSF_Core.h
    src/core/NSF_File.cpp
    src/core/NSF_File.h
    src/core/Wave_FDS.h
    src/core/Wave_FME07.h
    src/core/Wave_MMC5.h
    src/core/Wave_N106.h
    src/core/Wave_Square.h
    src/core/Wave_TND.h
    src/core/Wave_VRC6.h
    src/core/Wave_VRC7.cpp
)
source_group("NSF Core" FILES ${NSF_CORE})

set(RESOURCE_FILES
    resources/NotSoLogo.bmp
    resources/resource.h
    resources/Resource.rc
    resources/wannabewinamp.ico
)
source_group("Resource Files" FILES ${RESOURCE_FILES})

set(SOURCE_FILES_GUI
    src/gui/AboutDlg.cpp
    src/gui/ChannelsDlg.cpp
    src/gui/ConfigDlg.cpp
    src/gui/FileInfoDlg.cpp
    src/gui/FME07Dlg.cpp
    src/gui/GeneralFileInfoDlg.cpp
    src/gui/MainControlDlg.cpp
    src/gui/MiniPlayControlDlg.cpp
    src/gui/MMC5Dlg.cpp
    src/gui/N106Dlg.cpp
    src/gui/PlayControlDlg.cpp
    src/gui/PlaylistDlg.cpp
    src/gui/TagInfoDlg.cpp
    src/gui/TrackInfoDlg.cpp
    src/gui/VRC6Dlg.cpp
    src/gui/VRC7Dlg.cpp
)
source_group("Source Files/GUI" FILES ${SOURCE_FILES_GUI})

set(SOURCE_FILES_
    src/NotsoFatso.def
    src/NSF.cpp
    src/Winamp.cpp
)
source_group("Source Files" FILES ${SOURCE_FILES_})

source_group("" FILES Readme.txt)

if(NSF_BUILD_VLC)
    add_library(notsofatso_plugin SHARED
        src/vlc/vlc_stream.cpp
        src/vlc/vlc.cpp
    )
    target_include_directories(notsofatso_plugin PRIVATE lib/vlc/include)
    target_link_directories(notsofatso_plugin PRIVATE .)
    target_link_libraries(notsofatso_plugin vlccore)
endif()

if(NSF_BUILD_WINAMP)
    add_library(in_NotSoFatso SHARED
        ${DFC}
        ${HEADER_FILES_GUI}
        ${HEADER_FILES}
        ${NSF_CORE}
        ${RESOURCE_FILES}
        ${SOURCE_FILES_GUI}
        ${SOURCE_FILES_}
        Readme.txt
    )
    # target_include_directories(in_NotSoFatso PRIVATE "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\km")
    target_link_libraries(in_NotSoFatso comdlg32 comctl32 ntdll)
endif()
