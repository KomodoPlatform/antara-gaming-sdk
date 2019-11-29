add_library(antara_animation2d STATIC)
target_sources(antara_animation2d PRIVATE
        antara/gaming/animation2d/animation.2d.cpp
        antara/gaming/animation2d/component.animation.2d.cpp)
target_include_directories(antara_animation2d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_animation2d PUBLIC antara::ecs antara::graphics)
add_library(antara::animation2d ALIAS antara_animation2d)