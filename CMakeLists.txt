cmake_minimum_required(VERSION 3.5)

project(game_text C)

enable_testing()
include(CTest)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall --coverage")

## find SDL2
include(sdl2.cmake)
message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

## compilation rules
include_directories(${SDL2_ALL_INC})
add_executable(game_sdl main.c game_sdl.c)

## -- Pour Arda et Naisy (executer sur macOS)
##include_directories("/usr/local/Cellar/sdl2/2.30.1/include/SDL2")
##include_directories("/usr/local/Cellar/sdl2_image/2.8.2_1/include/SDL2")
##include_directories("/usr/local/Cellar/sdl2_ttf/2.22.0/include/SDL2")

## copy useful ressources in the build directory
file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

link_directories(${CMAKE_SOURCE_DIR})
add_executable(game_text game_text.c)
add_executable(game_solve game_solve.c)
add_executable(game_test_albarut game_test_albarut.c)
add_executable(game_test_herakotondra game_test_herakotondra.c)
add_executable(game_test_pbui game_test_pbui.c)

# target_link_libraries(game_text libgame.a)
# target_link_libraries(game_test_albarut libgame.a)
# target_link_libraries(game_test_herakotondra libgame.a)
# target_link_libraries(game_test_pbui libgame.a)

add_library(game game.c game_aux.c game_ext.c game_private.c queue.c game_tools.c game_random.c)
target_link_libraries(game_sdl ${SDL2_ALL_LIBS} game)
target_link_libraries(game_text game)
target_link_libraries(game_solve game)
target_link_libraries(game_test_albarut game)
target_link_libraries(game_test_herakotondra game)
target_link_libraries(game_test_pbui game)

add_test(test_albarut_dummy ./game_test_albarut dummy)
add_test(test_herakotondra_dummy ./game_test_herakotondra dummy)
add_test(test_pbui_dummy ./game_test_pbui dummy)

add_test(test_albarut_game_restart ./game_test_albarut game_restart)
add_test(test_albarut_game_get_constraint ./game_test_albarut game_get_constraint)
add_test(test_albarut_game_default_solution ./game_test_albarut game_default_solution)
add_test(test_albarut_game_print ./game_test_albarut game_print)
add_test(test_albarut_game_equal ./game_test_albarut game_equal)
add_test(test_albarut_game_new_empty ./game_test_albarut game_new_empty)
add_test(test_albarut_game_new_empty_ext ./game_test_albarut game_new_empty_ext)
add_test(test_albarut_game_get_neighbourhood ./game_test_albarut game_get_neighbourhood)
add_test(test_albarut_game_undo ./game_test_albarut game_undo)
add_test(test_albarut_game_load ./game_test_albarut game_load)
add_test(test_albarut_game_save ./game_test_albarut game_save)
add_test(test_albarut_game_solve ./game_test_albarut game_solve)

add_test(test_herakotondra_game_set_color ./game_test_herakotondra game_set_color)
add_test(test_herakotondra_game_delete ./game_test_herakotondra game_delete)
add_test(test_herakotondra_game_get_color ./game_test_herakotondra game_get_color)
add_test(test_herakotondra_game_get_next_square ./game_test_herakotondra game_get_next_square)
add_test(test_herakotondra_game_play_move ./game_test_herakotondra game_play_move)
add_test(test_herakotondra_game_default ./game_test_herakotondra game_default)
add_test(test_herakotondra_game_redo ./game_test_herakotondra game_redo)
add_test(test_herakotondra_game_is_wrapping ./game_test_herakotondra game_is_wrapping)
add_test(test_herakotondra_game_new_ext ./game_test_herakotondra game_new_ext)

add_test(test_pbui_game_won ./game_test_pbui game_won)
add_test(test_pbui_game_nb_neighbors ./game_test_pbui game_nb_neighbors)
add_test(test_pbui_game_get_status ./game_test_pbui game_get_status)
add_test(test_pbui_game_set_constraint ./game_test_pbui game_set_constraint)
add_test(test_pbui_game_copy ./game_test_pbui game_copy)
add_test(test_pbui_game_new ./game_test_pbui game_new)
add_test(test_pbui_game_nb_rows ./game_test_pbui game_nb_rows)
add_test(test_pbui_game_nb_cols ./game_test_pbui game_nb_cols)
add_test(test_pbui_game_nb_solutions ./game_test_pbui game_nb_solutions)
#set(CMAKE_VERBOSE_MAKEFILE on)

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})