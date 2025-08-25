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
find_package(KF6I18n)

add_executable(${PROJECT_NAME}
  src/main.cpp
  public/${PROJECT_NAME}.qrc
)
target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    KF${QT_VERSION_MAJOR}::I18n
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    NAME="${PROJECT_NAME}"
    VERSION="${PROJECT_VERSION}"
    DESCRIPTION="${PROJECT_DESCRIPTION}"
)

include(GNUInstallDirs)
set(LOCALE_INSTALL_DIR /usr/share/locale/)
add_subdirectory(po)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT "neilaspace@outlook.com")
set(CPACK_PACKAGE_CONTACT Neila)
set(CPACK_DEBIAN_PACKAGE_SECTION utils)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_SUGGESTS dolphin)
include(CPack)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(FILES public/${PROJECT_NAME}.desktop DESTINATION /usr/share/kio/servicemenus)
endif()
