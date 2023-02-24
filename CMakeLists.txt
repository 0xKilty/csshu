cmake_minimum_required(VERSION 3.11)
project(csshu)

add_compile_options(-Wall)


include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

find_package(CURL REQUIRED)
add_executable(csshu main.cpp)
#target_link_libraries(csshu PRIVATE CURL::libcurl )
target_link_libraries(csshu PRIVATE cpr::cpr)
#add_custom_target(${PROJECT_NAME}.tar ALL COMMAND
#    tar -cf ${PROJECT_NAME}.tar *.cc CMakeLists.txt)