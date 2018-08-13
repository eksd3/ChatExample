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
project(QtSimpleChat VERSION "1.0")
set(REQUIRED_QT_VERSION 5.7.0)
find_package(Qt5Core ${REQUIRED_QT_VERSION} REQUIRED)
find_package(Qt5Gui ${REQUIRED_QT_VERSION} REQUIRED)
find_package(Qt5Widgets ${REQUIRED_QT_VERSION} REQUIRED)
find_package(Qt5Network ${REQUIRED_QT_VERSION} REQUIRED)


set(QtSimpleChat_LIBS ${QtSimpleChat_LIBS} ${Qt5Core_LIBRARIES})
set(QtSimpleChat_INCLUDE ${QtSimpleChat_INCLUDE} ${Qt5Core_INCLUDE_DIRS})
set(QtSimpleChat_COMPILE_DEFINE ${QtSimpleChat_COMPILE_DEFINE} ${Qt5Core_COMPILE_DEFINITIONS} )

set(QtSimpleChat_LIBS ${QtSimpleChat_LIBS} ${Qt5Gui_LIBRARIES})
set(QtSimpleChat_INCLUDE ${QtSimpleChat_INCLUDE} ${Qt5Gui_INCLUDE_DIRS})
set(QtSimpleChat_COMPILE_DEFINE ${QtSimpleChat_COMPILE_DEFINE} ${Qt5Gui_COMPILE_DEFINITIONS})

set(QtSimpleChat_LIBS ${QtSimpleChat_LIBS} ${Qt5Widgets_LIBRARIES})
set(QtSimpleChat_INCLUDE ${QtSimpleChat_INCLUDE} ${Qt5Widgets_INCLUDE_DIRS})
set(QtSimpleChat_COMPILE_DEFINE ${QtSimpleChat_COMPILE_DEFINE} ${Qt5Widgets_COMPILE_DEFINITIONS})

set(QtSimpleChat_LIBS ${QtSimpleChat_LIBS} ${Qt5Network_LIBRARIES})
set(QtSimpleChat_INCLUDE ${QtSimpleChat_INCLUDE} ${Qt5Network_INCLUDE_DIRS})
set(QtSimpleChat_COMPILE_DEFINE ${QtSimpleChat_COMPILE_DEFINE} ${Qt5Network_COMPILE_DEFINITIONS})

set(QtSimpleChat_INCLUDE ${QtSimpleChat_INCLUDE} ${CMAKE_CURRENT_SOURCE_DIR})
set(QtSimpleChat_SRCS
    chatsession.cpp
    chatmessage.cpp
    chatserver.cpp
    serverwindow.cpp
    serverwindow.ui
)
set(QtSimpleChat_COMPILE_DEFINE ${QtSimpleChat_COMPILE_DEFINE} QTSIMPLECHAT_LIBRARY)

add_library(QtSimpleChat SHARED ${QtSimpleChat_SRCS})
target_include_directories(QtSimpleChat PUBLIC ${QtSimpleChat_INCLUDE})
target_link_libraries(QtSimpleChat PUBLIC ${QtSimpleChat_LIBS})
target_compile_definitions(QtSimpleChat PRIVATE ${QtSimpleChat_COMPILE_DEFINE})
set_target_properties(QtSimpleChat PROPERTIES 
    VERSION "1.0"
    EXPORT_NAME "QtSimpleChat"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

