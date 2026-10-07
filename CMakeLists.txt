#-----------------------------------------------------------------------------
# Requires CMake 3.10 or later (matching the Anybus CompactCom Driver API
# .cmake file, which uses include_guard)
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)

#-----------------------------------------------------------------------------
# Sanity check that this is run in a supported environment.
# Note: this must run before set(CMAKE_GENERATOR_PLATFORM Win32) below —
# on a non-Windows host, the platform setting would abort the default
# generator during project() before this diagnostic could be reached.
#-----------------------------------------------------------------------------
if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
   message(FATAL_ERROR
      "Building starter_kit_example on host '${CMAKE_HOST_SYSTEM_NAME}' "
      "is not supported.")
endif()

#-----------------------------------------------------------------------------
# Generate a Win32 (32-bit) project, since the
# HMS Transport Provider DLL is 32-bit.
#-----------------------------------------------------------------------------
set(CMAKE_GENERATOR_PLATFORM Win32)

#-----------------------------------------------------------------------------
# Restricting the available build configurations to Debug and Release.
#-----------------------------------------------------------------------------
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

#-----------------------------------------------------------------------------
# Early sanity check: verify that a Visual Studio installation with the
# "Desktop development with C++" workload is available, so that we can
# give an actionable error message instead of a cryptic
# "No CMAKE_C_COMPILER could be found" later on.
#-----------------------------------------------------------------------------
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
   find_program(VSWHERE_EXE NAMES vswhere.exe
      PATHS "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/Installer")
   if(VSWHERE_EXE)
      execute_process(
         COMMAND "${VSWHERE_EXE}" -products *
	         -requires Microsoft.VisualStudio.Workload.NativeDesktop
                 -property installationPath
         OUTPUT_VARIABLE VS_PATH
	 OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      if(NOT VS_PATH)
         message(FATAL_ERROR
            "Visual Studio was found, but the 'Desktop development with C++' "
	    "workload is not installed. Open the Visual Studio Installer, "
	    "select 'Modify', enable the workload, then re-run CMake.")
      endif()
   endif()
endif()

#-----------------------------------------------------------------------------
# Creating a user host application project.
#-----------------------------------------------------------------------------
project(starter_kit_example_project)

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
set(ABCC_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/abcc-driver-api)

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
