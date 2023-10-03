# This is a tutorial file. Feel free to remove it.

add_library(PetrMath
    "PetrMath.hpp"
    "PetrMath.cpp"
     "Vector.cpp" "Vector.hpp" "Matrix.cpp" "Matrix.hpp" "Quaternion.cpp" "Quaternion.hpp" "AxisAngle.cpp" "AxisAngle.hpp")

# (optional) Search and ling with 3rd party libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
target_link_libraries(PetrMath PUBLIC lodepng glad::glad glfw)
