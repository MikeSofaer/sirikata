# Searches for an OpenCOLLADA installation
#
# Defines:
#
#   OpenCOLLADA_FOUND           True if OpenCOLLADA was found, else false
#   OpenCOLLADA_LIBRARIES       Libraries to link (without full path)
#   OpenCOLLADA_LIBRARY_DIRS    Directories containing the libraries (-L option)
#   OpenCOLLADA_LDFLAGS         All required linker flags
#   OpenCOLLADA_INCLUDE_DIRS    Directories containing the header files (-I option)
#   OpenCOLLADA_CFLAGS          All required compiler flags
#
# To specify an additional directory to search, set OpenCOLLADA_ROOT.
#
# Author: Mark C. Barnes, 2009
#

SET(OpenCOLLADA_FOUND FALSE)

# Look for the headers, first in the additional location and then in default system locations
FIND_PATH(OpenCOLLADA_INCLUDE_DIRS NAMES COLLADAFWPrerequisites.h PATHS ${OpenCOLLADA_ROOT} ${OpenCOLLADA_ROOT}/include/ DOC "Location of OpenCOLLADA header files" NO_DEFAULT_PATH)
IF(NOT OpenCOLLADA_INCLUDE_DIRS)
    FIND_PATH(OpenCOLLADA_INCLUDE_DIRS NAMES COLLADAFWPrerequisites.h DOC "Location of OpenCOLLADA header files")
ENDIF()

IF(OpenCOLLADA_INCLUDE_DIRS)

    # toplevel directory
    SET(OpenCOLLADA_ROOT_DIRS ${OpenCOLLADA_INCLUDE_DIRS})
    IF("${OpenCOLLADA_ROOT_DIRS}" MATCHES "/include$")
        # Destroy trailing "/include" in the path.
        GET_FILENAME_COMPONENT(OpenCOLLADA_ROOT_DIRS ${OpenCOLLADA_ROOT_DIRS} PATH)
    ENDIF()

    # library path
    SET(OpenCOLLADA_LIBRARY_DIRS ${OpenCOLLADA_ROOT_DIRS})
    IF(EXISTS "${OpenCOLLADA_LIBRARY_DIRS}/lib")
        SET(OpenCOLLADA_LIBRARY_DIRS ${OpenCOLLADA_LIBRARY_DIRS}/lib)
    ENDIF(EXISTS "${OpenCOLLADA_LIBRARY_DIRS}/lib")

    IF(WIN32) # must distinguish debug and release builds

        FIND_LIBRARY(OpenCOLLADA_DEBUG_LIBRARY NAMES COLLADASaxFrameworkLoader COLLADASaxFrameworkLoader_d libCOLLADASaxFrameworkLoaderd libCOLLADASaxFrameworkLoader_d
                                 PATH_SUFFIXES "" Debug PATHS ${OpenCOLLADA_LIBRARY_DIRS} NO_DEFAULT_PATH)
        FIND_LIBRARY(OpenCOLLADA_RELEASE_LIBRARY NAMES COLLADASaxFrameworkLoader libCOLLADASaxFrameworkLoader
                                 PATH_SUFFIXES "" Release PATHS ${OpenCOLLADA_LIBRARY_DIRS} NO_DEFAULT_PATH)
        SET(OpenCOLLADA_LIBRARIES)
        IF(OpenCOLLADA_DEBUG_LIBRARY AND OpenCOLLADA_RELEASE_LIBRARY)
            SET(OpenCOLLADA_LIBRARIES debug ${OpenCOLLADA_DEBUG_LIBRARY} optimized ${OpenCOLLADA_RELEASE_LIBRARY})
        ELSEIF(OpenCOLLADA_DEBUG_LIBRARY)
            SET(OpenCOLLADA_LIBRARIES ${OpenCOLLADA_DEBUG_LIBRARY})
        ELSEIF(OpenCOLLADA_RELEASE_LIBRARY)
            SET(OpenCOLLADA_LIBRARIES ${OpenCOLLADA_RELEASE_LIBRARY})
        ENDIF()
    ELSE()
        FIND_LIBRARY(OpenCOLLADA_LIBRARIES NAMES COLLADASaxFrameworkLoader PATHS ${OpenCOLLADA_LIBRARY_DIRS} NO_DEFAULT_PATH)
    ENDIF()

    IF(OpenCOLLADA_LIBRARIES)
        SET(OpenCOLLADA_FOUND TRUE)
    ENDIF()
ENDIF()

IF(OpenCOLLADA_FOUND)
    IF(NOT OpenCOLLADA_FIND_QUIETLY)
        MESSAGE(STATUS "Found OpenCOLLADA: includes at ${OpenCOLLADA_INCLUDE_DIRS}, libraries at ${OpenCOLLADA_LIBRARIES}")
    ENDIF()
ELSE()
    IF(OpenCOLLADA_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "OpenCOLLADA not found")
    ENDIF()
ENDIF()
