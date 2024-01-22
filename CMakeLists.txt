cmake_minimum_required(VERSION 3.1)

# define a project name
project(CRUD_Book)

# define empty list of libraries to link
set(PROJECT_LIBS "")

# set optimization level
set(CMAKE_BUILD_TYPE Debug)

# set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# load DOCOPT
find_package(docopt REQUIRED)
include_directories(${DOCOPT_INCLUDE_DIRS})
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp)
# create executable
add_executable(${PROJECT_NAME} Main.cpp)

# link libraries
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS} docopt
  SQLiteCpp
  sqlite3
  pthread
  dl
  )
