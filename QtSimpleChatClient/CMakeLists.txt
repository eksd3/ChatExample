cmake_minimum_required(VERSION 3.3)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif()
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif()
if(POLICY CMP0057)
    cmake_policy(SET CMP0057 NEW)
endif()
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
project(QtSimpleChatClient VERSION "1.0") 
set(REQUIRED_QT_VERSION 5.7.0)
find_package(Qt5Core ${REQUIRED_QT_VERSION} REQUIRED)
find_package(Qt5Gui ${REQUIRED_QT_VERSION} REQUIRED)
find_package(Qt5Widgets ${REQUIRED_QT_VERSION} REQUIRED)
find_package(Qt5Network ${REQUIRED_QT_VERSION} REQUIRED)

set(QtSimpleChatClient_LIBS ${QtSimpleChatClient_LIBS} ${Qt5Core_LIBRARIES})
set(QtSimpleChatClient_INCLUDE ${QtSimpleChatClient_INCLUDE} ${Qt5Core_INCLUDE_DIRS})
set(QtSimpleChatClient_COMPILE_DEFINE ${QtSimpleChatClient_COMPILE_DEFINE} ${Qt5Core_COMPILE_DEFINITIONS} )

set(QtSimpleChatClient_LIBS ${QtSimpleChatClient_LIBS} ${Qt5Gui_LIBRARIES})
set(QtSimpleChatClient_INCLUDE ${QtSimpleChatClient_INCLUDE} ${Qt5Gui_INCLUDE_DIRS})
set(QtSimpleChatClient_COMPILE_DEFINE ${QtSimpleChatClient_COMPILE_DEFINE} ${Qt5Gui_COMPILE_DEFINITIONS})

set(QtSimpleChatClient_LIBS ${QtSimpleChatClient_LIBS} ${Qt5Widgets_LIBRARIES})
set(QtSimpleChatClient_INCLUDE ${QtSimpleChatClient_INCLUDE} ${Qt5Widgets_INCLUDE_DIRS})
set(QtSimpleChatClient_COMPILE_DEFINE ${QtSimpleChatClient_COMPILE_DEFINE} ${Qt5Widgets_COMPILE_DEFINITIONS})

set(QtSimpleChatClient_LIBS ${QtSimpleChatClient_LIBS} ${Qt5Network_LIBRARIES})
set(QtSimpleChatClient_INCLUDE ${QtSimpleChatClient_INCLUDE} ${Qt5Network_INCLUDE_DIRS})
set(QtSimpleChatClient_COMPILE_DEFINE ${QtSimpleChatClient_COMPILE_DEFINE} ${Qt5Network_COMPILE_DEFINITIONS})

set(QtSimpleChatClient_INCLUDE ${QtSimpleChatClient_INCLUDE} ${CMAKE_CURRENT_SOURCE_DIR})
set(QtSimpleChatClient_SRCS
    clientmain.cpp
    chatwindow.cpp
    chatwindow.ui
)
add_executable(QtSimpleChatClient WIN32 ${QtSimpleChatClient_SRCS})
add_dependencies(QtSimpleChatClient QtSimpleChat)
target_include_directories(QtSimpleChatClient PRIVATE ../QtSimpleChat ${QtSimpleChatClient_INCLUDE})
target_link_libraries(QtSimpleChatClient PRIVATE QtSimpleChat ${QtSimpleChatClient_LIBS})
target_compile_definitions(QtSimpleChatClient PRIVATE ${QtSimpleChatClient_DEFINE})
set_target_properties(QtSimpleChatClient PROPERTIES 
    VERSION "1.0"
    SOVERSION 1
    EXPORT_NAME "QtSimpleChatClient"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
