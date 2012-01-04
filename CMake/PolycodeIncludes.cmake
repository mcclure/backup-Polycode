# If the following short ugly mess is not present, failures can occur with -G "Unix Makefiles".
# FIXME: cmake should be finding ALL of the following things by way of CMAKE_PREFIX_PATH
IF (MINGW)
SET(OGG_LIBRARY libogg)
SET(OGG_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
SET(PNG_LIBRARY png)
SET(PNG_PNG_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
ENDIF()

# platform specific defines and include directories for win and sdl
IF(MSVC OR MINGW)
    ADD_DEFINITIONS(/D_UNICODE /DUNICODE)
    INCLUDE_DIRECTORIES("${Polycode_SOURCE_DIR}/Core/Contents/PolycodeView/MSVC/Polycore")
ELSEIF(APPLE)
    ADD_DEFINITIONS(-DGL_GLEXT_PROTOTYPES)
    INCLUDE_DIRECTORIES("${Polycode_SOURCE_DIR}/Core/Contents/PolycodeView/Mac OS X")
ELSE(MSVC)
    ADD_DEFINITIONS(-DGL_GLEXT_PROTOTYPES)
    INCLUDE_DIRECTORIES("${Polycode_SOURCE_DIR}/Core/Contents/PolycodeView/Linux")
ENDIF(MSVC OR MINGW)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(ZLIB REQUIRED)
FIND_PACKAGE(PNG REQUIRED)
FIND_PACKAGE(PhysFS REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED)
FIND_PACKAGE(Freetype REQUIRED)
FIND_PACKAGE(Ogg REQUIRED)
FIND_PACKAGE(Vorbis REQUIRED)
FIND_PACKAGE(VorbisFile REQUIRED)
FIND_PACKAGE(Lua REQUIRED)

# Use SDL on non-Apple unixes
IF(UNIX AND NOT APPLE)
    FIND_PACKAGE(SDL REQUIRED)
ENDIF(UNIX AND NOT APPLE)

#
# Set up common include directories
INCLUDE_DIRECTORIES(
    ${Polycode_SOURCE_DIR}/Core/Contents/Include
    ${OPENGL_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${PHYSFS_INCLUDE_DIR}
    ${OGG_INCLUDE_DIR}
    ${VORBISFILE_INCLUDE_DIR}
    ${PNG_INCLUDE_DIR}
    ${OPENGLEXT_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
)
