cmake_minimum_required(VERSION 3.8)
project(test1)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(std_srvs REQUIRED)

find_package(rviz_common REQUIRED)
find_package(rviz_rendering REQUIRED)
find_package(rviz_default_plugins REQUIRED)


find_package(Qt5 REQUIRED COMPONENTS Widgets)
#find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
#qt_standard_project_setup()

#---------------- အင်ကလု ----------------#
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gui/ui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gui/header)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/header)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/source)

set(CMAKE_AUTOUIC_SEARCH_PATHS gui/ui)
set(UI_FILES gui/ui/mainwindow.ui)
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

add_executable(MainWindow
    gui/source/main.cpp
    gui/header/mainwindow.h
    gui/source/mainwindow.cpp
    src/header/publisher.h
    src/source/publisher.cpp
    src/header/subscriber.h
    src/source/subscriber.cpp
    ${UI_HEADERS}
)

ament_target_dependencies(MainWindow rclcpp std_msgs)
target_link_libraries(MainWindow ${Qt5Core_LIBRARIES})
target_link_libraries(MainWindow ${Qt5Widgets_LIBRARIES})

#---------------- ဖျက်ရန် -----------------#
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

ament_package()
