cmake_minimum_required(VERSION 2.8)
project(plantilla)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
ADD_DEFINITIONS( -Wall -DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB )
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCES main.cpp mainwindow.cpp osgview.cpp)


#
# Qt4
#
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt4 COMPONENTS REQUIRED QtGui QtOpenGL)
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTOPENGL TRUE )
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR}/.. ${QT_QTGUI_INCLUDE_DIR} )


#
# OpenCV 2
#
find_package(OpenCV 3 REQUIRED)
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIR} )
SET(LIBS ${LIBS} ${OpenCV_LIBS})
MESSAGE(STATUS ${OpenCV_LIBS})


#
# OpenSceneGraph
#
INCLUDE_DIRECTORIES( /usr/local/include/osg /usr/include/osgViewer )
SET(LIBS ${OSG_LIBS} -L/usr/local/lib64/ -losg -losgViewer -losgGA -losgText -losgDB -losgUtil )


#
# Link
#
add_executable(plantilla_tema_4 ${SOURCES} )
target_link_libraries(plantilla_tema_4 ${LIBS} Qt4::QtGui Qt4::QtOpenGL libapriltags.a -lopencv_highgui ${OpenCV_LIBS} )
