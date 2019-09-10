

##! antara ecs tests
add_executable(antara_ecs_tests)
target_sources(antara_ecs_tests PUBLIC antara.ecs.tests.cpp antara.ecs.system.tests.cpp)
target_link_libraries(antara_ecs_tests PRIVATE doctest)