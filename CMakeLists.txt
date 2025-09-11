cmake_minimum_required(VERSION 3.19)
project(car LANGUAGES CXX)

# 启用自动处理moc、uic和rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

# 设置包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

qt_add_executable(${PROJECT_NAME}
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/components/dashboard.cpp
    src/components/dashboard.h
    src/components/menu.cpp
    src/components/menu.h
    src/components/setting.cpp
    src/components/setting.h
    src/resources/resources.qrc
)

qt_add_translations(
    TARGETS car
    TS_FILES car_zh_CN.ts
)

target_link_libraries(car
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS car
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET car
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
