cmake_minimum_required(VERSION 3.6)
project(robust_cpp)

set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

SET(PEDANTIC_FLAGS "-g -Wall -Wextra -pedantic -Wno-sign-compare -DDEBUG")

SET(CMAKE_CXX_FLAGS "" )
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${PEDANTIC_FLAGS} -fsanitize=address" )
SET(CMAKE_CXX_FLAGS_DEBUG_UBSAN "${CMAKE_CXX_FLAGS} ${PEDANTIC_FLAGS} -fsanitize=memory -fno-omit-frame-pointer -fsanitize-memory-track-origins" )
SET(CMAKE_CXX_FLAGS_DEBUG_NO_ASAN "${CMAKE_CXX_FLAGS} ${PEDANTIC_FLAGS}" )
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Ofast" )

###############################################################################
# set up gurobi and Clp
###############################################################################

include_directories($ENV{EIGEN_HOME}/include/eigen3/ $ENV{GUROBI_HOME}/include/)
# include_directories(usr/local/Cellar/eigen/3.3.3/include/eigen3/ $ENV{GUROBI_HOME}/include/)
link_directories($ENV{GUROBI_HOME}/lib/ $ENV{BOOST_HOME}/lib/)
set(LIBS gurobi_c++ gurobi70 boost_system boost_filesystem pthread debug asan)
set(EXTERN_HEADERS $ENV{GUROBI_HOME}/include/gurobi_c++.h $ENV{EIGEN_HOME}/include/eigen3/Eigen/Core)


###############################################################################
# build main program
###############################################################################

file(GLOB_RECURSE HEADER_FILES "src/*.h")
file(GLOB_RECURSE CPP_FILES "src/*.cpp")
file(GLOB_RECURSE FAKE_CPP_FILES "src/.#*.cpp")
foreach(fake ${FAKE_CPP_FILES})
  list(REMOVE_ITEM CPP_FILES ${fake})
endforeach()
list(FILTER CPP_FILES EXCLUDE REGEX "main.cpp")
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


# Now simply link against gtest or gtest_main as needed. Eg
file(GLOB_RECURSE  TEST_FILES "test/*.cpp")
add_executable(run_tests ${SOURCE_FILES} ${TEST_FILES})
target_link_libraries(run_tests gtest_main ${LIBS})

add_custom_target(run_after_build ALL
  run_tests
)
