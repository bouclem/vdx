cmake_minimum_required(VERSION 3.16)
project(vdx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(vdx
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/interpreter.cpp
    src/modules/fs.cpp
    src/modules/math.cpp
    src/modules/graph.cpp
)

target_include_directories(vdx PRIVATE src)

# ── Windows MSI Installer (CPack + WiX) ──
install(TARGETS vdx RUNTIME DESTINATION bin)

set(CPACK_PACKAGE_NAME "VDX")
set(CPACK_PACKAGE_VENDOR "Voidware")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "VDX Programming Language")
set(CPACK_PACKAGE_VERSION "0.1.4")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "4")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "VDX")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

# WiX 4 settings
set(CPACK_GENERATOR "WIX")
set(CPACK_WIX_VERSION "4")
set(CPACK_WIX_UPGRADE_GUID "A1B2C3D4-E5F6-7890-ABCD-EF1234567890")
set(CPACK_WIX_PROGRAM_MENU_FOLDER "VDX")
set(CPACK_WIX_UI_REF "VDX_InstallDir")

# Add vdx to system PATH optionally via installer dialog
set(CPACK_WIX_PATCH_FILE "${CMAKE_SOURCE_DIR}/installer/wix_path_patch.xml")
set(CPACK_WIX_EXTRA_SOURCES "${CMAKE_SOURCE_DIR}/installer/wix_path_dialog.wxs")

# WiX extensions (WiX 4 naming)
set(CPACK_WIX_BUILD_EXTENSIONS "WixToolset.Util.wixext")

include(CPack)
