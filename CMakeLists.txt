project(qt_ogl_es3)
cmake_minimum_required(VERSION 2.8)

include_directories(
  inc
)

link_directories(
  /usr/lib
)

link_libraries(
)

#----------------------------------------------------------------
set(OUTPUTNAME "eglsample")

set(SRC_FILES
  src/main.cpp
)

set(HDR_FILES
)

add_executable(
  ${OUTPUTNAME}
  ${SRC_FILES}
)

target_link_libraries(
    ${OUTPUTNAME}
	X11
	EGL
	GLESv2
)

#----------------------------------------------------------------
install(
    FILES
        ${PROJECT_BINARY_DIR}/${OUTPUTNAME}
    DESTINATION
        ${CMAKE_SOURCE_DIR}/bin
    PERMISSIONS
        OWNER_READ
        OWNER_EXECUTE
        GROUP_READ
        GROUP_EXECUTE
        WORLD_READ
        WORLD_EXECUTE
)

install(
  DIRECTORY
        ${CMAKE_SOURCE_DIR}/qml
    DESTINATION
        ${CMAKE_SOURCE_DIR}/bin
)
