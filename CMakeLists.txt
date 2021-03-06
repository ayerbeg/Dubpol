#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Dubpol)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
              	    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Get examples sources from common and shared
#
#list(APPEND sources 
  # uncomment the line below to include HBOOK analysis
#  ${PROJECT_SOURCE_DIR}/src/DetectorConstruction.cc
#  ${PROJECT_SOURCE_DIR}/src/DetectorMessenger.cc
#  ${PROJECT_SOURCE_DIR}/src/EventAction.cc
#  ${PROJECT_SOURCE_DIR}/src/PhysicsList.cc
#  ${PROJECT_SOURCE_DIR}/src/PrimaryGeneratorAction.cc
#  ${PROJECT_SOURCE_DIR}/src/RunAction.cc
#  ${PROJECT_SOURCE_DIR}/src/SteppingAction.cc)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(Dubpol Dubpol.cc ${sources} ${headers})
target_link_libraries(Dubpol ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} )

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build Dubpol. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(Dubpol_SCRIPTS
    vis.mac gui.mac var.ini Dubpol_AnalysisMacro.C
  )

foreach(_script ${Dubpol_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS Dubpol DESTINATION bin)
