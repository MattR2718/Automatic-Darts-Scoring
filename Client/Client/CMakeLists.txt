# CMakeList.txt : CMake project for Client, include source and define
# project specific logic here.
#
find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS asio)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(hello-imgui CONFIG REQUIRED)
    
# Add source to this project's executable.
# add_executable (Client "Client.cpp" "Client.h" "ClientClass.h"  "DartMaths.h")
hello_imgui_add_app(Client "Client.cpp" "Client.h" "ClientClass.h"  "DartMaths.h" "player.cpp" "player.h" "mt_client.cpp")

target_link_libraries(Client PRIVATE fmt::fmt Boost::asio nlohmann_json::nlohmann_json)

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