cmake_minimum_required(VERSION 3.16)
project(myxampp VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network DBus)
set(SOURCES
    src/main.cpp src/mainwindow.cpp src/servicemanager.cpp
    src/logviewer.cpp src/configeditor.cpp
    src/portchecker.cpp src/ftpserver.cpp
)
set(HEADERS
    src/mainwindow.h src/servicemanager.h src/logviewer.h
    src/configeditor.h src/portchecker.h src/ftpserver.h
)
add_executable(myxampp ${SOURCES} ${HEADERS})
target_link_libraries(myxampp PRIVATE Qt6::Core Qt6::Widgets Qt6::Network Qt6::DBus)
target_compile_definitions(myxampp PRIVATE
    MYXAMPP_VERSION="${PROJECT_VERSION}"
    MYXAMPP_BASE_DIR="/opt/myxampp"
)
install(TARGETS myxampp DESTINATION /opt/myxampp/bin)
install(DIRECTORY conf/ DESTINATION /opt/myxampp/etc)
install(PROGRAMS myxamppctl DESTINATION /opt/myxampp/bin)
install(PROGRAMS scripts/setup_dirs.sh scripts/init_db.sh DESTINATION /opt/myxampp/scripts)
