CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(Polycode)


IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
    MESSAGE("No CMAKE_BUILD_TYPE specified, defaulting to ${CMAKE_BUILD_TYPE}")
ENDIF(NOT CMAKE_BUILD_TYPE)

#IF(APPLE OR WIN32)
#    SET(build_player OFF)
#ELSE()
#    SET(build_player OFF)
#ENDIF()

# Options for what components to build
#OPTION(POLYCODE_BUILD_SHARED "Build Polycode shared libraries" OFF)
#OPTION(POLYCODE_BUILD_STATIC "Build Polycode static libraries" ON)
OPTION(POLYCODE_BUILD_BINDINGS "Build Polycode Lua bindings" OFF)
OPTION(POLYCODE_BUILD_MODULES "Build Polycode modules" ON)
OPTION(POLYCODE_BUILD_PLAYER "Build Polycode standalone player" OFF)
OPTION(POLYCODE_BUILD_TOOLS "Build Polycode tools" ON)
OPTION(POLYCODE_BUILD_DOCS "Build Polycode documentation" ON)

OPTION(POLYCODE_INSTALL_FRAMEWORK "Install Polycode Core, Modules and Tools" ON)
OPTION(POLYCODE_INSTALL_PLAYER "Install Polycode Player" ${POLYCODE_BUILD_PLAYER})
OPTION(POLYCODE_INSTALL_TEMPLATE "Install Template project" ON)
OPTION(POLYCODE_INSTALL_DOCS ${POLYCODE_BUILD_DOCS})

# Some non-standard CMake modules
SET(CMAKE_MODULE_PATH ${Polycode_SOURCE_DIR}/CMake)

IF(NOT POLYCODE_RELEASE_DIR)
    SET(POLYCODE_RELEASE_DIR ${Polycode_SOURCE_DIR}/Release/${CMAKE_SYSTEM_NAME})
    MESSAGE("No POLYCODE_RELEASE_DIR specified, to ${POLYCODE_RELEASE_DIR}") 
ENDIF(NOT POLYCODE_RELEASE_DIR)

SET(CMAKE_INSTALL_PREFIX ${POLYCODE_RELEASE_DIR}/Framework/)

# set default externals install location
SET(CMAKE_PREFIX_PATH 
    ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies
    ${POLYCODE_RELEASE_DIR}/Framework/Modules/Dependencies
    ${POLYCODE_RELEASE_DIR}/Framework/Tools/Dependencies)

MESSAGE(DEBUG "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")

# If the following large ugly mess is not present, failures can occur with -G "Unix Makefiles".
# FIXME: cmake should be finding ALL of the following things by way of CMAKE_PREFIX_PATH
SET(ZLIB_LIBRARY ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(ZLIB_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
SET(OGG_LIBRARY ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(OGG_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
SET(PNG_LIBRARY ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(PNG_PNG_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
SET(PHYSFS_LIBRARY ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(PHYSFS_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
SET(OPENAL_LIBRARY ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(OPENAL_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include/AL)
SET(FREETYPE_LIBRARY ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(FREETYPE_INCLUDE_DIRS ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
SET(VORBIS_LIBRARY ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(VORBIS_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
SET(VORBISFILE_LIBRARY ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(VORBISFILE_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
SET(LUA_LIBRARY ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(LUA_LIBRARIES ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(LUA_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
SET(BOX2D_RELEASE_LIBRARY ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(BOX2D_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
SET(ASSIMP_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
SET(BULLET_INCLUDE_DIR ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/include)
SET(LIBBULLETCOLLISION ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(LIBBULLETDYNAMICS ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(LIBBULLETMATH ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(LIBBULLETMULTITHREADED ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)
SET(LIBBULLETSOFTBODY ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies/lib)

# Process subdirectories
ADD_SUBDIRECTORY(Core/Contents)
ADD_SUBDIRECTORY("Assets/Default asset pack")
ADD_SUBDIRECTORY("Assets/Templates")
ADD_SUBDIRECTORY("Documentation")

IF(POLYCODE_BUILD_BINDINGS)
    ADD_SUBDIRECTORY(Bindings)
ENDIF(POLYCODE_BUILD_BINDINGS)

IF(POLYCODE_BUILD_MODULES)
    ADD_SUBDIRECTORY(Modules/Contents)
    IF(POLYCODE_BUILD_BINDINGS)
        ADD_SUBDIRECTORY(Modules/Bindings)
    ENDIF(POLYCODE_BUILD_BINDINGS)
ENDIF(POLYCODE_BUILD_MODULES)

IF(POLYCODE_BUILD_PLAYER)
    ADD_SUBDIRECTORY(Player/Contents)
ENDIF(POLYCODE_BUILD_PLAYER)

IF(POLYCODE_BUILD_TOOLS)
    ADD_SUBDIRECTORY(Tools/Contents)
ENDIF(POLYCODE_BUILD_TOOLS)

INSTALL(FILES LICENSE.txt
        DESTINATION ./)

# No Windows Build yet
IF (NOT WIN32)
    INSTALL(DIRECTORY ${Polycode_SOURCE_DIR}/Examples/C++/Build/${CMAKE_SYSTEM_NAME} DESTINATION Examples)
    INSTALL(DIRECTORY ${Polycode_SOURCE_DIR}/Examples/C++/Contents DESTINATION Examples/${CMAKE_SYSTEM_NAME})
    INSTALL(DIRECTORY ${Polycode_SOURCE_DIR}/Examples/C++/Resources DESTINATION Examples/${CMAKE_SYSTEM_NAME})
ENDIF(NOT WIN32)

INSTALL(DIRECTORY ${POLYCODE_RELEASE_DIR}/Framework/Core/Dependencies DESTINATION Core)
INSTALL(DIRECTORY ${POLYCODE_RELEASE_DIR}/Framework/Modules/Dependencies DESTINATION Modules)

SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "8")
SET(CPACK_PACKAGE_VERSION_PATCH "2")

IF(WIN32)
	SET(CPACK_GENERATOR "ZIP")
ELSE(WIN32)
	SET(CPACK_GENERATOR "TGZ")
ENDIF(WIN32)

INCLUDE(CPack)

# Install assets and dependencies
