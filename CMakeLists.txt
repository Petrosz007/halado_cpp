cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(
  BiMap
  VERSION 0.0.1
  DESCRIPTION "BiMap"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -pedantic -Werror)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

# add_compile_options(-fsanitize=thread)
# add_link_options(-fsanitize=thread)

# add_compile_options(-fsanitize=memory)
# add_link_options(-fsanitize=memory)

# add_compile_options(-fsanitize=undefined)
# add_link_options(-fsanitize=undefined)


add_subdirectory(src)
add_subdirectory(extern/catch)
add_subdirectory(test)
