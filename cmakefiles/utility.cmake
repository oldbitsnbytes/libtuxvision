cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(utility CXX)

set(WORK "utility")
set(TARGET_DIR ${FRAME}/${WORK})

set(HEADERS
    ${TARGET_DIR}/defs.h
    ${TARGET_DIR}/cadres.h
    ${TARGET_DIR}/cmdargs.h
    ${TARGET_DIR}/colors.h
    ${TARGET_DIR}/notify.h
    ${TARGET_DIR}/expect.h
    ${TARGET_DIR}/geometry.h
    ${TARGET_DIR}/glyphes.h
    #${TARGET_DIR}/data/databits.h
    #${TARGET_DIR}/data/field.h
    #${TARGET_DIR}/data/table.h
    ${TARGET_DIR}/sys/sys.h
    ${TARGET_DIR}/sys/sys.inc
    ${TARGET_DIR}/object.h
    ${TARGET_DIR}/rem.h
    ${TARGET_DIR}/string.h
    ${TARGET_DIR}/app_base.h
    ${TARGET_DIR}/vchar.h
    ${TARGET_DIR}/strscan.h
    ${TARGET_DIR}/strscan.numeric_data.value.inc.h
    ${TARGET_DIR}/io/lfd.h
    #${TARGET_DIR}/io/listener.h
    ${TARGET_DIR}/io/tcp_socket.h
    ${TARGET_DIR}/io/kbhit.h
    ${TARGET_DIR}/io/mouse.h
    ${TARGET_DIR}/io/console.h

)


add_library(${WORK} STATIC

    ${HEADERS}
    ${TARGET_DIR}/colors.cc
    ${TARGET_DIR}/geometry.cc
    #${TARGET_DIR}/data/databits.cc
    #${TARGET_DIR}/data/field.cc
    #${TARGET_DIR}/data/table.cc
    ${TARGET_DIR}/sys/sys.cc
    ${TARGET_DIR}/cadres.cc
    ${TARGET_DIR}/glyphes.cc
    ${TARGET_DIR}/object.cc
    ${TARGET_DIR}/rem.cc
    ${TARGET_DIR}/cmdargs.cc
    ${TARGET_DIR}/string.cc
    ${TARGET_DIR}/app_base.cc
    ${TARGET_DIR}/vchar.cc
    ${TARGET_DIR}/vchar.pad.cc
    ${TARGET_DIR}/strscan.cc
    ${TARGET_DIR}/io/lfd.cc
    #${TARGET_DIR}/io/listener.cc
    ${TARGET_DIR}/io/tcp_socket.cc
    ${TARGET_DIR}/io/kbhit.cc
    ${TARGET_DIR}/io/mouse.cc
    ${TARGET_DIR}/io/console.cc

)
target_include_directories(${WORK} PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:include/${TARGET_DIR}>
)
message("build and include dir:" ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_DIR})
set_target_properties(${WORK} PROPERTIES OUTPUT_NAME ${FRAME}.${WORK})



target_compile_definitions("${WORK}" PUBLIC "${WORK}_DEBUG=$<CONFIG:Debug>")
include(GenerateExportHeader)
generate_export_header(${WORK} EXPORT_FILE_NAME "${PROJECT_NAME}_export")

IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    )
ENDIF ()


target_link_libraries(${WORK} ${CMAKE_DL_LIBS} sqlite3) # and other system dependencies...





install(TARGETS ${WORK}
        EXPORT ${WORK}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        )

INSTALL(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${TARGET_DIR})


add_custom_target("uninstall_${PROJECT_NAME}" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall_${PROJECT_NAME}"
    POST_BUILD
    COMMENT "Uninstall ${PROJECT_NAME} files under include and lib"
    COMMAND rm -vrf ${CMAKE_INSTALL_PREFIX}/include/${TARGET_DIR} ${CMAKE_INSTALL_PREFIX}/lib/lib${PROJECT_NAME}* || echo Nothing in install_manifest.txt to be uninstalled!
)


#add_subdirectory(test.app)
