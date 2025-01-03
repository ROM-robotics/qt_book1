cmake_minimum_required(VERSION 3.8)
project(rom_qt)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

find_package(rviz_common REQUIRED)
find_package(rviz_rendering REQUIRED)
find_package(rviz_default_plugins REQUIRED)

# file(GLOB PRO_INCLUDE_DIR RELATIVE ${CMAKE_SOURCE_DIR} FOLLOW_SYMLINKS src/*.h *.hpp)

include_directories(
       include
       ${CMAKE_CURRENT_BINARY_DIR}
       ${CMAKE_CURRENT_SOURCE_DIR}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(rom_qt src/main.cpp src/main_window.cpp src/main_window.ui)
target_include_directories(rom_qt PUBLIC include)
target_link_libraries(rom_qt Qt5::Widgets)
ament_target_dependencies(rom_qt rclcpp std_msgs geometry_msgs nav_msgs std_srvs rviz_common rviz_rendering rviz_default_plugins)

install(TARGETS rom_qt DESTINATION lib/${PROJECT_NAME})

ament_package()
