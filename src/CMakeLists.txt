include_directories(.)

file(GLOB_RECURSE src
     "*.hpp"
     "*.cpp"
)

add_executable(ravi ${src})