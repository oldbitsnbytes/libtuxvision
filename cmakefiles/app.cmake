set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("Dir: " ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(app.x
    app/main.cc
)


target_include_directories(app.x PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)


target_link_directories(app.x PUBLIC bin)
add_dependencies(app.x utility lexer est tui) # lexer est tuxml tui


# TABARNAK! j'joubli toujours de faire lier les librairies dans le bon ordre!!!!!
target_link_libraries(app.x
   utility
   lexer
   est
   tui
   sqlite3
)
