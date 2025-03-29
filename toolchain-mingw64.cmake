# Specify the cross-compiler
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Where to find Windows libraries (MinGW root)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Adjust search behavior
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Hint SDL2 location for MinGW
set(SDL2_PATH /usr/x86_64-w64-mingw32)
