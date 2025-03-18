# This CMake script is written to work with minimum CMake version 3.10.
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

# Forcing CMake to generate a Win32 project since the HMS Transport Provider DLL used
# is 32-bit.
set(CMAKE_GENERATOR_PLATFORM Win32)

# Creating a user host application project.
project(starter_kit_example_project)

# Sanity check that this is ran in a supported environment.
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
else()
  message(WARNING "Host ${CMAKE_SYSTEM_NAME} is not supported to build starter_kit_example.")
endif()

# Creating a user host application executable target.
add_executable(starter_kit_example
  ${PROJECT_SOURCE_DIR}/src/main.c
)

# Changing the default startup project into the executable target.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT starter_kit_example)

# Source (.c) files to add to the executable target. These are 'user unique'. Some are
# related to the CompactCom Driver API.
set(starter_kit_example_SRCS
  ${PROJECT_SOURCE_DIR}/src/main.c
  ${PROJECT_SOURCE_DIR}/src/example_application/abcc_network_data_parameters.c
  ${PROJECT_SOURCE_DIR}/src/example_application/implemented_callback_functions.c
  ${PROJECT_SOURCE_DIR}/src/example_application/cyclical_processing.c
  ${PROJECT_SOURCE_DIR}/src/abcc_adaptation/abcc_software_port.c
  ${PROJECT_SOURCE_DIR}/src/abcc_adaptation/abcc_hardware_abstraction.c
  ${PROJECT_SOURCE_DIR}/src/abcc_adaptation/pctransportprovider/imp_tp.c
)

# Header (.h) files related to the user host application.
set(starter_kit_example_INCS
  ${PROJECT_SOURCE_DIR}/src/example_application/abcc_network_data_parameters.h
  ${PROJECT_SOURCE_DIR}/src/abcc_adaptation/abcc_driver_config.h
  ${PROJECT_SOURCE_DIR}/src/abcc_adaptation/abcc_software_port.h
  ${PROJECT_SOURCE_DIR}/src/abcc_adaptation/abcc_types.h
  ${PROJECT_SOURCE_DIR}/src/abcc_adaptation/pctransportprovider/imp_tp.h
  ${PROJECT_SOURCE_DIR}/src/abcc_adaptation/pctransportprovider/TP.h
)

# Adding the 'user unique' sources to the user host application executable target.
# The header files are added only to keep the file and directory tree structure.
target_sources(starter_kit_example PRIVATE
   ${starter_kit_example_SRCS}
   ${starter_kit_example_INCS}
)

# Directories containing 'user unique' header (.h) files related to the CompactCom
# Driver API.
set(ABCC_API_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/src/abcc_adaptation/
  ${PROJECT_SOURCE_DIR}/src/abcc_adaptation/pctransportprovider/
)

# The directory containing the Anybus CompactCom Driver API repository.
set(ABCC_API_DIR ${PROJECT_SOURCE_DIR}/lib/abcc-driver-api)

# Including the Anybus CompactCom Driver API's CMake module file.
include(${ABCC_API_DIR}/abcc-driver-api.cmake)

# Adding both the Anybus CompactCom Driver API related and unrelated header (.h) files
# to the user host application executable target.
target_include_directories(starter_kit_example PRIVATE
  ${ABCC_API_INCLUDE_DIRS}
)

# Keeping the file and directory tree structure of the host application when 
# generating the IDE project.
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${starter_kit_example_SRCS} ${starter_kit_example_INCS})

# Linking the Anybus CompactCom Driver library to the executable target.
target_link_libraries(starter_kit_example abcc_api)