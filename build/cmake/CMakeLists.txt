cmake_minimum_required(VERSION 3.11)
project(nana-creator)

#SET Default build type:
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
STRING "Choose the type of build." FORCE)
# Set the possible values of build type for cmake-gui
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

#Define C++ standard:
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}
    ../../libs/pugixml/pugixml.cpp
    ../../src/filemanager.cpp
    ../../src/nana_extra/propertygrid.cpp
    ../../src/main.cpp
    ../../src/new_project.cpp
    ../../src/ctrls/categorize.cpp
    ../../src/assetspanel.cpp
    ../../src/ctrls/label.cpp
    ../../src/creator.cpp
    ../../src/ctrls/tabbar.cpp
    ../../src/ctrls/ctrl.cpp
    ../../src/propertygrid_helper.cpp
    ../../src/ctrls/toolbar.cpp
    ../../src/info.cpp
    ../../src/scrollablecanvas.cpp
    ../../src/objectspanel.cpp
    ../../src/ctrls/form.cpp
    ../../src/nana_extra/color_helper.cpp
    ../../src/propertiespanel.cpp
    ../../src/ctrls/box_model.cpp
    ../../src/ctrls/progress.cpp
    ../../src/ctrls/menubar.cpp
    ../../src/items_dialog.cpp
    ../../src/nana_extra/adi_place.cpp
    ../../src/ctrls/panel.cpp
    ../../src/ctrls/checkbox.cpp
    ../../src/ctrls/notebook.cpp
    ../../src/ctrls/button.cpp
    ../../src/ctrls/property.cpp
    ../../src/imagemanager.cpp
    ../../src/ctrls/combox.cpp
    ../../src/ctrls/textbox.cpp
    ../../src/codegenerator.cpp
    ../../src/ctrls/field.cpp
    ../../src/pgitems_creator.cpp
    ../../src/ctrls/picture.cpp
    ../../src/nana_extra/pgitems.cpp
    ../../src/ctrls/date_chooser.cpp
    ../../src/ctrls/spinbox.cpp
    ../../src/guimanager.cpp
    ../../src/ctrls/slider.cpp
    ../../src/ctrls/treebox.cpp
    ../../src/ctrls/custom.cpp
    ../../src/namemanager.cpp
    ../../src/ctrls/splitterbar.cpp
    ../../src/ctrls/listbox.cpp
    ../../src/ctrls/group.cpp
    ../../src/inifile.cpp
    ../../src/ctrls/page.cpp
    )

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libs
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src
    )

target_link_libraries(${PROJECT_NAME}
    nana
    pthread
    X11
    Xft
    fontconfig
    stdc++fs
    png
    jpeg
    Xcursor
)

add_executable(test-ctrls
    ../../tests/ctrls/main.cpp
    ../../tests/ctrls/ctrls.ncp
    ../../tests/ctrls/ctrls.h
    )
target_link_libraries(test-ctrls nana pthread X11 Xft fontconfig stdc++fs)

add_test(NAME test-ctrls COMMAND test-ctrls)

include(CTest)
