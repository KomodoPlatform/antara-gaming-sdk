##! Dependancies
include(FetchContent)

FetchContent_Declare(
        doctest
        URL https://github.com/onqtam/doctest/archive/2.3.3.tar.gz
)

FetchContent_MakeAvailable(doctest)