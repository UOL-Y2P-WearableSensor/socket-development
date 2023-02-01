cmake_minimum_required(VERSION 3.24)
project(WearableSensor_Macos)
set(CMAKE_CXX_STANDARD 11)


include(FetchContent)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.11.0
)
FetchContent_MakeAvailable(spdlog)



add_executable(WearableSensor_Macos main.cpp src/Server.cpp inc/Server.h src/Logging.cpp inc/Logging.h src/Logging.cpp inc/Logging.h src/Socket.cpp inc/Socket.h)


target_link_libraries(WearableSensor_Macos PUBLIC spdlog)