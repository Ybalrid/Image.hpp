CMAKE_MINIMUM_REQUIRED(VERSION 3.6)

set(CMAKE_CXX_STANDARD 17)

project(ImageTests)

configure_file(./Image_unit_test/green.bmp ./green.bmp COPYONLY)
configure_file(./Image_unit_test/Lenna.png ./Lenna.png COPYONLY)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

find_package(FreeImage REQUIRED)

include_directories(./Image ./Image_unit_test ${FREEIMAGE_INCLUDE_DIRS})

add_executable(ImageTests ./Image_unit_test/ImageTest.cpp ./Image_unit_test/main.cpp)

target_link_libraries(ImageTests ${FREEIMAGE_LIBRARIES})

enable_testing()

include(ParseAndAddCatchTests)
ParseAndAddCatchTests(${PROJECT_NAME})
