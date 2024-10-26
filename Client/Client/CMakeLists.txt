# CMakeList.txt : CMake project for Client, include source and define
# project specific logic here.
#
find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS asio)
find_package(nlohmann_json CONFIG REQUIRED)

    
# Add source to this project's executable.
add_executable (Client "Client.cpp" "Client.h" "ClientClass.h"  "DartMaths.h")

target_link_libraries(Client PRIVATE fmt::fmt)
target_link_libraries(Client PRIVATE Boost::asio)
target_link_libraries(Client PRIVATE nlohmann_json::nlohmann_json)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Client PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
