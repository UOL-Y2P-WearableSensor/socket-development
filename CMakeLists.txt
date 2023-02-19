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

include_directories(src)
aux_source_directory(src SRC_LIST)
aux_source_directory(. SRC_LIST)

add_executable(WearableSensor_Macos ${SRC_LIST} src/IMU_json.cpp src/IMU_json.h src/IMU_json.h)


target_link_libraries(WearableSensor_Macos PUBLIC spdlog)



