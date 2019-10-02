##! Uncomment those lines if you use the gaming sdk as an external project
#if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
#    set(LINUX TRUE)
#endif ()

#include(FetchContent)

#FetchContent_Declare(
#        antara-gaming-sdk
#        URL https://github.com/KomodoPlatform/antara-gaming-sdk/archive/master.zip
#)

#FetchContent_MakeAvailable(antara-gaming-sdk)
#init_apple_env()

add_executable(quick_and_dirty quick_and_dirty.cpp)
target_link_libraries(quick_and_dirty PUBLIC antara::world)