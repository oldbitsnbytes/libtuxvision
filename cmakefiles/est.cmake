
set(WORK   "est")


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_DIR ${FRAME}/${WORK})
set(TARGETNAME ${WORK})

message(STATUS "TARGET_DIR: ${TARGET_DIR}")

set(HEADERS
    ${TARGET_DIR}/alu.h
    ${TARGET_DIR}/ynod.h
)

add_library(${WORK} STATIC

    ${HEADERS}
    ${TARGET_DIR}/alu.cc
    ${TARGET_DIR}/ynod.cc


)



target_include_directories(${WORK} PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           $<INSTALL_INTERFACE:include/${TARGET_DIR}>
)


set_target_properties(${WORK} PROPERTIES OUTPUT_NAME "${FRAME}++.${WORK}")

target_compile_definitions(${WORK} PUBLIC "${WORK}_DEBUG=$<CONFIG:Debug>")
include(GenerateExportHeader)
generate_export_header(${WORK} EXPORT_FILE_NAME "${WORK}_export")

IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    )
ENDIF ()


target_link_libraries(${WORK} ${CMAKE_DL_LIBS} utility lexer sqlite3)


install(TARGETS ${WORK}
        EXPORT  ${WORK}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)

INSTALL(FILES ${HEADERS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${TARGET_DIR}")


add_custom_target("uninstall_${WORK}" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall_${WORK}"
    POST_BUILD
    COMMENT "Uninstall ${WORK} files under include and lib"
    COMMAND rm -vrf ${CMAKE_INSTALL_PREFIX}/include/${TARGET_DIR} ${CMAKE_INSTALL_PREFIX}/lib/${WORK}* || echo Nothing in install_manifest.txt to be uninstalled!
)


add_dependencies(${WORK} utility lexer)
