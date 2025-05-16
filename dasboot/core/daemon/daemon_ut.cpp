#include <gtest/gtest.h>

#include <iostream>

#include <dasboot/core/daemon/daemon.hpp>

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	auto result = RUN_ALL_TESTS();
	return result;
}