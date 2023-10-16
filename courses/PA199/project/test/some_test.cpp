// This is a tutorial file. Feel free to remove it.

#include "../some_my_library_dir/some_library_file.hpp"
#include "../PetrMath/Vector.cpp"
#include "../PetrMath/Matrix.cpp"
#include "../PetrMath/AxisAngle.cpp"
#include "../PetrMath/Quaternion.cpp"
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

    //Magnitude
    Petr_Math::Vector VectorToMag(1.f, 2.f, 2.f, 0.f);
    EXPECT_EQ(VectorToMag.magnitude(), 3) << "Magnitude should be 3.";
    
    //Opposite
    EXPECT_EQ(Unit.opposite()[0], -1.f) << "-(1) = -1";

    //Sub
    auto subVec = (Unit - VectorToMag);
    EXPECT_EQ(subVec[0], 0.f) << "1 - 1 = 0";
    EXPECT_EQ(subVec[1], -1.f) << "1 - 2 = -1";
    EXPECT_EQ(subVec[2], -1.f) << "1 - 2 = -1";
    EXPECT_EQ(subVec[3], 1.f) << "1 - 0 = 1";

    //Sub
    auto addVec = (Unit + VectorToMag);
    EXPECT_EQ(addVec[0], 2.f) << "1 + 1 = 2";
    EXPECT_EQ(addVec[1], 3.f) << "1 + 2 = 3";
    EXPECT_EQ(addVec[2], 3.f) << "1 + 2 = 3";
    EXPECT_EQ(addVec[3], 1.f) << "1 + 0 = 1";

    //Dot
    EXPECT_EQ(Unit.dot(VectorToMag), 5.f) << "Dot product should be 5.";

    //Cross
    auto crossProd = Unit.cross(VectorToMag);
    EXPECT_EQ(crossProd[0], 0)  << "x = 0";
    EXPECT_EQ(crossProd[1], -1) << "y = -1";
    EXPECT_EQ(crossProd[2], 1)  << "z = 1";
}

TEST(some_test_suite, Matrix)
{
    Petr_Math::Matrix Ones(4, 4, 1.0f);
    Petr_Math::Matrix Twos(4, 4, 2.0f);
    //Multiplication
    Petr_Math::Matrix Result = Ones * Twos;
    EXPECT_EQ(Result.at(0,0), 8.f) << "1*2 + 1*2 + 1*2 + 1*2 == 8";

    //Transpose
    Petr_Math::Matrix Result2 = Result.transpose();
    //Multiplication by a vector.
}

TEST(some_test_suite, AxisAngle)
{
    Petr_Math::AxisAngle Angle(1.0f, Petr_Math::Vector(3, 1.0f));
    Petr_Math::Quaternion Quat(Angle);
    //Quat = Quat.Normalize();
    /*EXPECT_EQ(Quat.s, 0.877582550) << "scalar == 0.877582550";
    EXPECT_EQ(Quat.u.data[0], 0.2767965) << "scalar == 1";*/
}
