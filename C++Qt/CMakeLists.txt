cmake_minimum_required(VERSION 3.16)
project(MultReplacerApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Automatically handle .ui, .qrc, and moc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add our source files
add_executable(MultReplacerApp
    main.cpp
    mainwindow.h mainwindow.cpp
    replacementrow.h replacementrow.cpp
    confirmationdialog.h confirmationdialog.cpp
)

# Link against Qt libraries
target_link_libraries(MultReplacerApp PRIVATE Qt6::Widgets)

# Set output directory
set_target_properties(MultReplacerApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)