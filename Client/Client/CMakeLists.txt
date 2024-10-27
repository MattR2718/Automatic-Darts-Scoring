# CMakeList.txt : CMake project for Client, include source and define
# project specific logic here.
#

# Check if the target system is Windows
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Set the linker flag to use the Windows subsystem
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

find_package(Boost REQUIRED COMPONENTS asio)
find_package(hello-imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
    
# Check if the target system is Windows
#if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
#    hello_imgui_add_app(Client WIN32 "Client.cpp" "Client.h" "ClientClass.h"  "DartMaths.h" "player.cpp" "player.h" "mt_client.cpp")
#else()
    hello_imgui_add_app(Client "Client.cpp" "Client.h"  "DartMaths.h" "player.cpp" "player.h" "mt_client.cpp")
#endif()
target_link_libraries(Client PRIVATE Boost::asio)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Client PROPERTY CXX_STANDARD 20)
endif()

set(OUTPUT_DIRECTORY_DATA ${CMAKE_CURRENT_BINARY_DIR}/assets)

# Create a custom command to copy the directory
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Client/assets ${OUTPUT_DIRECTORY_DATA}
    COMMENT "Copying data directory from ${CMAKE_SOURCE_DIR}/Client/assets to ${OUTPUT_DIRECTORY_DATA}"
)