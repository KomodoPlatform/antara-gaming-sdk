##! Dependancies
include(FetchContent)

set(DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF CACHE BOOL "Override option" FORCE)
FetchContent_Declare(
        doctest
        URL https://github.com/onqtam/doctest/archive/2.3.3.tar.gz
)

FetchContent_Declare(
        entt
        URL https://github.com/skypjack/entt/archive/master.zip
)

FetchContent_MakeAvailable(doctest entt)