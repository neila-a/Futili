cmake_minimum_required(VERSION 3.16)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(futili
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "Git-based file backup tool in the context menu."
)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

find_package(KF6KIO)
find_package(KF6CoreAddons)
find_package(KF6Config)
find_package(KF6I18n)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(KDEInstallDirs6)
include(KDECompilerSettings)
include(KDECMakeSettings)

kcoreaddons_add_plugin(${PROJECT_NAME} SOURCES
    src/${PROJECT_NAME}.cpp src/${PROJECT_NAME}.h
    src/dirfile.h src/dirfile.cpp

    src/resources/${PROJECT_NAME}.qrc
    INSTALL_NAMESPACE kf6/kfileitemaction
)
target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    KF6::CoreAddons
    KF6::KIOWidgets
    KF6::I18n
    KF6::ConfigGui
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    NAME="${PROJECT_NAME}"
    VERSION="${PROJECT_VERSION}"
    DESCRIPTION="${PROJECT_DESCRIPTION}"
)

add_subdirectory(po)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT "neilaspace@outlook.com")
set(CPACK_PACKAGE_CONTACT Neila)
set(CPACK_DEBIAN_PACKAGE_SECTION utils)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_DEPENDS dolphin)
include(CPack)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${KDE_INSTALL_FULL_LIBDIR}/${CMAKE_SYSTEM_PROCESSOR}-linux-gnu/qt6/plugins/kf6/kfileitemaction/
    RUNTIME DESTINATION ${KDE_INSTALL_FULL_BINDIR}
)
