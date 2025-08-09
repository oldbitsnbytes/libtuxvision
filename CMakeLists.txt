#OPTION(IO           "Build the IO sublib............................[liblux++.io.a]"  		ON)

cmake_minimum_required(VERSION 3.30 FATAL_ERROR)
set (BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR})

project(tuxvision
     VERSION 0.1.0
     DESCRIPTION "A non-standard c++ framework suite of my personal hobby projects"
     LANGUAGES CXX)

OPTION(UTILITY      "Build the bits and the bytes support...........[libtuxvision.utility.a]"   ON)
OPTION(LEXER        "Build the lexer sublib.........................[libtuxvision.lexer.a]"     ON)
OPTION(EST          "Build the est (Expression Syntax Tree) sublib..[libtuxvision.est.a]" 	    ON)
OPTION(TUXML        "Build the small text formatter sublib..........[libtuxvision.tuxml.a]"	    OFF)
OPTION(TUI          "Build the console ANSI UI sublib(tux::vision)..[libtuxvision.tui.a]" 	    ON)
OPTION(APP          "Build the Application tests....................[app.x]"    	            ON)


set(FRAME tuxvision)

if(UTILITY)
    message(STATUS "Building the bits and the bytes utilities:")
    include(cmakefiles/utility.cmake)
endif()

# if(IO)
#     message(STATUS "Building input-output section.")
#     include(cmakefiles/io.cmake)
# endif()

if(LEXER)
    message(STATUS "Building the lexer.")
    include(cmakefiles/lexer.cmake)
endif()

if(EST)
    message(STATUS "Building the 'Expression Syntax Tree'")
    include(cmakefiles/est.cmake)
endif()

IF(TUXML)
    message(STATUS "Building the TUX markup lang.")
    include(cmakefiles/tuxml.cmake)
ENDIF()        


if(TUI)
    message(STATUS "Building the tuxvision TUI")
    include(cmakefiles/tui.cmake)
endif()

if(APP)
    message(STATUS "Building Application tests:")
    include(cmakefiles/app.cmake)
endif()

include(GNUInstallDirs)
