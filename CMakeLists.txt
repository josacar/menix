project(menix)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")
set(CMAKE_BUILD_TYPE "Relese")
find_package(Qt4 REQUIRED)
find_package(qextserialport REQUIRED)

# Win32 y MacOSX Hacks
IF(WIN32)
	SET(GUI_TYPE WIN32)
ELSEIF(APPLE)
	SET(GUI_TYPE MACOSX_BUNDLE)
ENDIF(WIN32)

# Para el libqextserialport
IF(UNIX)
  add_definitions(-D_TTY_POSIX_)
ENDIF(UNIX)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} )

# Archivos fuente c/cpp
set(${PROJECT_NAME}_SRCS 
      menixdebug.cpp
      menixprefs.cpp
      menix.cpp
      opcionesglobales.cpp
      server.cpp
      smsat.cpp
      main.cpp
)

# MinGW hacks
IF( MINGW )
      ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ico.o
                        COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.rc 
                             -o ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ico.o )
      SET(${PROJECT_NAME}_RCS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ico.o)
ELSE( MINGW )
      SET(${PROJECT_NAME}_RCS ${PROJECT_NAME}.rc)
ENDIF( MINGW )

# Archivos UI
set(${PROJECT_NAME}_UIS
      menix.ui
      menixprefs.ui
      menixdebug.ui
)

# Archivos QRC
set( ${PROJECT_NAME}_RESOURCES
     menix.qrc
)

# # Icono de la app para Hase
# set( ${PROJECT_NAME}_RCS
#      menix.rc
# )

# Ficheros con la macro Q_OBJECT
set( ${PROJECT_NAME}_MOC_HDRS
      menix.h
      menixdebug.h
      menixprefs.h
      server.h
#     opcionesglobales.h
      server.h
      smsat.h
)

# Compilamos recursos
qt4_add_resources( ${PROJECT_NAME}_RC_SRCS 
	${${PROJECT_NAME}_RESOURCES}
)

# Compilamos los ui con uic
qt4_wrap_ui( ${PROJECT_NAME}_UIS_HDRS
	${${PROJECT_NAME}_UIS}
)

# Compilamos los .h con moc
qt4_wrap_cpp( ${PROJECT_NAME}_MOC_SRCS
	${${PROJECT_NAME}_MOC_HDRS}
)

add_executable( ${PROJECT_NAME}
	${GUI_TYPE}
	${${PROJECT_NAME}_SRCS}
	${${PROJECT_NAME}_MOC_SRCS}
	${${PROJECT_NAME}_RC_SRCS}
	${${PROJECT_NAME}_UIS_HDRS}
	${${PROJECT_NAME}_RCS}
)

target_link_libraries( menix
	${QT_QTCORE_LIBRARY}
	${QT_QTGUI_LIBRARY}
	${QT_QTNETWORK_LIBRARY}
	${qextserialport_LIBRARY}
)
