cmake_minimum_required(VERSION 3.6)
project(robust_cpp)

set(CMAKE_CXX_STANDARD 14)

###############################################################################
# set up gurobi and Clp
###############################################################################

include_directories($ENV{EIGEN_HOME}/include/eigen3/ $ENV{GUROBI_HOME}/include/)
# include_directories(usr/local/Cellar/eigen/3.3.3/include/eigen3/ $ENV{GUROBI_HOME}/include/)
link_directories($ENV{GUROBI_HOME}/lib/ $ENV{BOOST_HOME}/lib/ external_code/Clp-1.15.6/lib/)
set(LIBS Clp CoinUtils gurobi70 gurobi_c++ boost_system boost_filesystem)
set(EXTERN_HEADERS $ENV{GUROBI_HOME}/include/gurobi_c++.h $ENV{EIGEN_HOME}/include/eigen3/Eigen/Core)


###############################################################################
# build main program
###############################################################################

file(GLOB_RECURSE HEADER_FILES "src/*.h")
file(GLOB_RECURSE CPP_FILES "src/*.cpp")
file(GLOB_RECURSE FAKE_CPP_FILES "src/.#*.cpp")
list(REMOVE_ITEM CPP_FILES "/Users/christiankroer/Documents/research/optimization/robust_cpp/src/main.cpp")
foreach(fake ${FAKE_CPP_FILES})
  list(REMOVE_ITEM CPP_FILES ${fake})
endforeach()
set(SOURCE_FILES ${HEADER_FILES} ${CPP_FILES} ${EXTERN_HEADERS})

add_executable(robust_cpp ${SOURCE_FILES} src/main.cpp)
TARGET_LINK_LIBRARIES(robust_cpp ${LIBS})


###############################################################################
# googletest setup
###############################################################################

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg
file(GLOB_RECURSE  TEST_FILES "test/*.cpp")
add_executable(run_tests ${SOURCE_FILES} ${TEST_FILES})
target_link_libraries(run_tests gtest_main ${LIBS})

add_custom_target(run_after_build ALL
  run_tests
)
