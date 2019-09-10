//
// Created by roman on 10/09/2019.
//

#include <cstring>
#include <doctest/doctest.h>
#include "antara/gaming/core/version.hpp"

TEST_CASE("test version")
{
	CHECK(std::strlen(antara::gaming::version()) > 0);
}