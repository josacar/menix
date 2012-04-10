# Busca qextserialport
FIND_PATH(qextserialport_INCLUDE_DIR qextserialport.h /usr/include /usr/include/qextserialport C:/Qt/qt/include )
FIND_LIBRARY(qextserialport_LIBRARY NAMES qextserialport PATHS /usr/lib /usr/local/lib C:/Qt/qt/lib )

IF (qextserialport_INCLUDE_DIR AND qextserialport_LIBRARY)
   SET(qextserialport_FOUND TRUE)
ENDIF (qextserialport_INCLUDE_DIR AND qextserialport_LIBRARY)

IF (qextserialport_FOUND)
  INCLUDE(FindPackageMessage)
   IF (NOT qextserialport_FIND_QUIETLY)
      MESSAGE(STATUS "Encontrado qextserialport.h: ${qextserialport_INCLUDE_DIR}")
      MESSAGE(STATUS "Encontrado qextserialport: ${qextserialport_LIBRARY}")
   ENDIF (NOT qextserialport_FIND_QUIETLY)
ELSE (qextserialport_FOUND)
   IF (qextserialport_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "qextserialport no encontrado!!!")
   ENDIF (qextserialport_FIND_REQUIRED)
ENDIF (qextserialport_FOUND)
