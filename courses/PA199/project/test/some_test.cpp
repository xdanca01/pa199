// This is a tutorial file. Feel free to remove it.

#include "../some_my_library_dir/some_library_file.hpp"
#include "../PetrMath/Vector.hpp"
#include <gtest/gtest.h>

TEST(some_test_suite, some_test)
{
    EXPECT_EQ(some_lib::some_my_library_function(1), 2) << "2*1==2.";
}

TEST(some_test_suite, another_test)
{
    EXPECT_EQ(some_lib::some_my_library_function(3), 6) << "2*3==6.";
}

TEST(some_test_suite, Vectors)
{
    //Unit
    Petr_Math::Vector Unit(4, 1.f);
    EXPECT_EQ(Unit[1], 1.f) << "y == 1";

    /*//Magnitude
    Petr_Math::Vector VectorToMag(1.f, 4.f, 4.f, 4.f);
    EXPECT_EQ(VectorToMag.magnitude(), 7) << "Magnitude should be 7.";
    
    //Opposite
    EXPECT_EQ(Unit.opposite()[0], -1.f) << "-(1) = -1";

    //Sub
    auto subVec = (Unit - VectorToMag);
    EXPECT_EQ(subVec[0], 0.f) << "1 - 1 = 0";
    EXPECT_EQ(subVec[1], -3.f) << "1 - 4 = -3";
    EXPECT_EQ(subVec[2], -3.f) << "1 - 4 = -3";
    EXPECT_EQ(subVec[3], -3.f) << "1 - 4 = -3";

    //Sub
    auto addVec = (Unit + VectorToMag);
    EXPECT_EQ(addVec[0], 2.f) << "1 + 1 = 2";
    EXPECT_EQ(addVec[1], 5.f) << "1 + 4 = 5";
    EXPECT_EQ(addVec[2], 5.f) << "1 + 4 = 5";
    EXPECT_EQ(addVec[3], 5.f) << "1 + 4 = 5";

    //Dot
    EXPECT_EQ(Unit.dot(VectorToMag), 13.f) << "Dot product should be 13.";

    //Cross
    auto crossProd = Unit.cross(VectorToMag);
    EXPECT_EQ(crossProd[0], 0)  << "x = 0";
    EXPECT_EQ(crossProd[1], -3) << "y = -3";
    EXPECT_EQ(crossProd[2], 3)  << "z = 3";*/
}
