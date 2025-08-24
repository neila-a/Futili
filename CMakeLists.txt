cmake_minimum_required(VERSION 3.16)

project(futili
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "Git-based file backup tool in the context menu."
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core LinguistTools Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core LinguistTools Widgets)
set(TS_FILES ${PROJECT_NAME}_zh_CN.ts)

add_executable(${PROJECT_NAME}
  main.cpp
  #${TS_FILES}
)
target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    NAME="${PROJECT_NAME}"
    VERSION="${PROJECT_VERSION}"
    DESCRIPTION="${PROJECT_DESCRIPTION}"
)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT "neilaspace@outlook.com")
set(CPACK_PACKAGE_CONTACT Neila)
set(CPACK_DEBIAN_PACKAGE_SECTION utils)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(FILES futili.desktop DESTINATION /usr/share/kio/servicemenus)
endif()
