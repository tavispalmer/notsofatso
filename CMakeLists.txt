cmake_minimum_required(VERSION 3.5.0)
project(in_NotSoFatso VERSION 0.1.0 LANGUAGES C CXX)

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

set(HEADER_FILES
    src/NSF.h
    src/Winamp.h
)

source_group("Header Files" FILES ${HEADER_FILES})

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
    resource/NotSoLogo.bmp
    resource/resource.h
    resource/Resource.rc
    resource/wannabewinamp.ico
)

source_group("Resource Files" FILES ${RESOURCE_FILES})

set(SOURCE_FILES_
    src/NotsoFatso.def
    src/NSF.cpp
    src/Winamp.cpp
)

source_group("Source Files" FILES ${SOURCE_FILES_})

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
