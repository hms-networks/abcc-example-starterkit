#-----------------------------------------------------------------------------
# Requires CMake 3.11 or later (due to add_executable() )
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.11)

#-----------------------------------------------------------------------------
# Sanity check that this is run in a supported environment.
#-----------------------------------------------------------------------------
if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
   message(FATAL_ERROR
      "Building starter_kit_example on host '${CMAKE_HOST_SYSTEM_NAME}' "
      "is not supported.")
endif()

#-----------------------------------------------------------------------------
# The HMS Transport Provider DLL is 32-bit, so the example must be built as a
# 32-bit application. With a Visual Studio generator, default to Win32 unless
# a platform was given explicitly with -A. The result is verified after
# project() below.
#-----------------------------------------------------------------------------
if(CMAKE_GENERATOR MATCHES "^Visual Studio" AND NOT CMAKE_GENERATOR_PLATFORM)
   set(CMAKE_GENERATOR_PLATFORM Win32)
endif()

#-----------------------------------------------------------------------------
# Restricting the available build configurations to Debug and Release.
#-----------------------------------------------------------------------------
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

#-----------------------------------------------------------------------------
# Early sanity check (Visual Studio generators only): verify that a
# Visual Studio installation with the "Desktop development with C++"
# workload is available, so that we can give an actionable error message
# instead of a cryptic "No CMAKE_C_COMPILER could be found" later on.
#
# Known limitation: vswhere is queried without version scoping. With
# multiple Visual Studio instances installed, the check may pass on an
# instance other than the one the generator selects, so the fallback to
# the cryptic error can still occur.
#-----------------------------------------------------------------------------
if(CMAKE_GENERATOR MATCHES "^Visual Studio")
   find_program(VSWHERE_EXE NAMES vswhere.exe
      PATHS "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/Installer")
   if(NOT VSWHERE_EXE)
      message(FATAL_ERROR
         "vswhere.exe was not found in the Visual Studio Installer directory. "
         "This usually means no Visual Studio installation is present. "
         "Install Visual Studio with the 'Desktop development with C++' "
         "workload, then re-run CMake.")
   endif()
   execute_process(
      COMMAND "${VSWHERE_EXE}" -products * -prerelease
              -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64
              -property installationPath
      OUTPUT_VARIABLE VS_PATH
      ERROR_VARIABLE  VSWHERE_ERROR
      RESULT_VARIABLE VSWHERE_RESULT
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_STRIP_TRAILING_WHITESPACE
   )
   if(NOT VSWHERE_RESULT EQUAL 0)
      message(FATAL_ERROR
         "Running '${VSWHERE_EXE}' failed (${VSWHERE_RESULT}): ${VSWHERE_ERROR}\n"
         "The Visual Studio installation could not be checked. Repair Visual "
         "Studio with the Visual Studio Installer, then re-run CMake.")
   endif()
   if(NOT VS_PATH)
      message(FATAL_ERROR
         "Visual Studio was found, but the 'Desktop development with C++' "
         "workload is not installed. Open the Visual Studio Installer, "
         "select 'Modify' and enable the workload, then re-run CMake.")
   endif()
endif()

#-----------------------------------------------------------------------------
# Creating a user host application project.
#-----------------------------------------------------------------------------
project(starter_kit_example_project)

#-----------------------------------------------------------------------------
# Verify that a 32-bit target was selected (see CMAKE_GENERATOR_PLATFORM
# above). CMAKE_SIZEOF_VOID_P is the pointer size in bytes of the selected
# compiler, determined by project(): 4 for a 32-bit target, 8 for a 64-bit
# target. This works regardless of how the target was chosen (-A Win32 or a
# 32-bit toolchain).
#-----------------------------------------------------------------------------
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
   message(FATAL_ERROR
      "starter_kit_example must be built as a 32-bit application, because the "
      "HMS Transport Provider DLL is 32-bit. Delete the build directory and "
      "re-run CMake with '-A Win32' (Visual Studio) or a 32-bit toolchain.")
endif()

#-----------------------------------------------------------------------------
# Creating a user host application executable target.
#-----------------------------------------------------------------------------
add_executable(starter_kit_example)

#-----------------------------------------------------------------------------
# Changing the default startup project into the executable target.
#-----------------------------------------------------------------------------
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT starter_kit_example)

#-----------------------------------------------------------------------------
# Source (.c) files to add to the executable target. These are 'user unique'.
# Some are related to the Anybus CompactCom Driver API.
#-----------------------------------------------------------------------------
set(starter_kit_example_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/example_application/abcc_network_data_parameters.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/example_application/implemented_callback_functions.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/example_application/network_settings_sequence.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/abcc_adaptation/abcc_software_port.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/abcc_adaptation/abcc_hardware_abstraction.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/abcc_adaptation/pctransportprovider/imp_tp.c
)

#-----------------------------------------------------------------------------
# Header (.h) files related to the user host application.
#-----------------------------------------------------------------------------
set(starter_kit_example_INCS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/abcc_adaptation/abcc_driver_config.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/abcc_adaptation/abcc_software_port.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/abcc_adaptation/abcc_types.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/abcc_adaptation/pctransportprovider/imp_tp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/abcc_adaptation/pctransportprovider/TP.h
)

#-----------------------------------------------------------------------------
# Adding the 'user unique' sources to the user host application executable
# target. The header files are added only to keep the file and directory tree
# structure.
#-----------------------------------------------------------------------------
target_sources(starter_kit_example PRIVATE
   ${starter_kit_example_SRCS}
   ${starter_kit_example_INCS}
)

#-----------------------------------------------------------------------------
# Directories containing 'user unique' header (.h) files related to the
# Anybus CompactCom Driver API.
#-----------------------------------------------------------------------------

set(ABCC_API_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/abcc_adaptation/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/abcc_adaptation/pctransportprovider/
)

#-----------------------------------------------------------------------------
# The directory containing the Anybus CompactCom Driver API repository.
#-----------------------------------------------------------------------------
set(ABCC_API_DIR
   ${CMAKE_CURRENT_SOURCE_DIR}/lib/abcc-driver-api
)

#-----------------------------------------------------------------------------
# Including the Anybus CompactCom Driver API's CMake module file.
#-----------------------------------------------------------------------------
include(${ABCC_API_DIR}/abcc-driver-api.cmake)

#-----------------------------------------------------------------------------
# Adding both the Anybus CompactCom Driver API related and unrelated
# header (.h) files to the user host application executable target.
#-----------------------------------------------------------------------------
target_include_directories(starter_kit_example PRIVATE
  ${ABCC_API_INCLUDE_DIRS}
)

#-----------------------------------------------------------------------------
# Keeping the file and directory tree structure of the host application when
# generating the IDE project.
#-----------------------------------------------------------------------------
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${starter_kit_example_SRCS} ${starter_kit_example_INCS})

#-----------------------------------------------------------------------------
# Linking the Anybus CompactCom Driver library to the executable target.
#-----------------------------------------------------------------------------
target_link_libraries(starter_kit_example abcc_api)
