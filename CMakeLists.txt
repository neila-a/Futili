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


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core LinguistTools Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core LinguistTools Widgets)

find_package(KF6KIO)
find_package(KF6CoreAddons)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)

kcoreaddons_add_plugin(${PROJECT_NAME} SOURCES
    src/${PROJECT_NAME}.cpp src/${PROJECT_NAME}.h
    src/main.cpp
    src/resources/${PROJECT_NAME}.qrc
    src/locales/${PROJECT_NAME}_zh_CN.ts
    INSTALL_NAMESPACE kf6/kfileitemaction
)
qt_add_translations(${PROJECT_NAME} TS_FILES src/locales/${PROJECT_NAME}_zh_CN.ts)
target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    KF6::CoreAddons
    KF6::KIOWidgets
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    NAME="${PROJECT_NAME}"
    VERSION="${PROJECT_VERSION}"
    DESCRIPTION="${PROJECT_DESCRIPTION}"
)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT "neilaspace@outlook.com")
set(CPACK_PACKAGE_CONTACT Neila)
set(CPACK_DEBIAN_PACKAGE_SECTION utils)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_SUGGESTS dolphin)
include(CPack)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION /usr/lib/x86_64-linux-gnu/qt6/plugins/kf6/kfileitemaction/
    RUNTIME DESTINATION ${KDE_INSTALL_FULL_BINDIR}
)
