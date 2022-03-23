cmake_minimum_required(VERSION 3.16)

project(EFP VERSION 2.2
        DESCRIPTION "Epistemic Forward Planner"
        LANGUAGES C CXX)

# Source files to include in compilation -- To include all use: file(GLOB_RECURSE EFP_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
set(
        EFP_SOURCES
        src/main.cpp
        src/actions/action.cpp
        src/actions/proposition.cpp
        src/actions/custom_event_models/cem.cpp
        src/actions/custom_event_models/cem_store.cpp
        src/actions/custom_event_models/cevent.cpp
        src/domain/domain.cpp
        src/domain/domain_config.cpp
        src/domain/grounder.cpp
        src/domain/initially.cpp
        src/formulae/belief_formula.cpp
        src/heuristics/heuristics_manager.cpp
        src/heuristics/heuristics_template.ipp
        src/heuristics/satisfied_goals.cpp
        src/parse/cem_parser.cpp
        src/refactor/asp_maker.cpp
        src/search/planner.ipp
        src/states/kripke/kstate.cpp
        src/states/kripke/kstore.cpp
        src/states/kripke/kworld.cpp
        src/states/possibilities/pstate.cpp
        src/states/possibilities/pstore.cpp
        src/states/possibilities/pworld.cpp
        src/states/state_T.ipp
        src/update/product_update.cpp
        src/utilities/helper.cpp
        src/utilities/helper_t.ipp
        src/utilities/printer.cpp
        src/utilities/reader.cpp
        external/bisimulation/bisimulation.cpp
)

# Headers files to include in compilation -- To include all use: file(GLOB_RECURSE EFP_HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)
set(
        EFP_HEADERS
        src/actions/action.h
        src/actions/proposition.h
        src/actions/custom_event_models/cem.h
        src/actions/custom_event_models/cem_store.h
        src/actions/custom_event_models/cevent.h
        src/domain/domain.h
        src/domain/domain_config.h
        src/domain/grounder.h
        src/domain/initially.h
        src/formulae/belief_formula.h
        src/heuristics/heuristics_manager.h
        src/heuristics/satisfied_goals.h
        src/parse/cem_parser.h
        src/refactor/asp_maker.h
        src/search/planner.h
        src/states/kripke/kstate.h
        src/states/kripke/kstore.h
        src/states/kripke/kworld.h
        src/states/possibilities/pstate.h
        src/states/possibilities/pstore.h
        src/states/possibilities/pworld.h
        src/states/state_T.h
        src/update/product_update.h
        src/utilities/define.h
        src/utilities/helper.h
        src/utilities/helper_t.h
        src/utilities/printer.h
        src/utilities/reader.h
        external/bisimulation/bisimulation.h
)

# FLEX & BISON
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

message(STATUS "Flex version:  ${FLEX_VERSION}")
message(STATUS "Bison version: ${BISON_VERSION}")

flex_target(marL src/parse/mar.lex "${CMAKE_CURRENT_BINARY_DIR}/Parse/mar.lex.cc")
bison_target(marP src/parse/mar.y "${CMAKE_CURRENT_BINARY_DIR}/Parse/mar.tab.cc")
add_flex_bison_dependency(marL marP)

flex_target(cemL src/parse/cem.lex "${CMAKE_CURRENT_BINARY_DIR}/Parse/cem.lex.cc")
bison_target(cemP src/parse/cem.y "${CMAKE_CURRENT_BINARY_DIR}/Parse/cem.tab.cc")
add_flex_bison_dependency(cemL cemP)

# BOOST
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem)
message(STATUS "Boost version: ${Boost_VERSION}")

include_directories(${Boost_INCLUDE_DIR})

# EXECUTABLES
set(PROJECT_EXE efp.out)
add_executable(${PROJECT_EXE} ${EFP_SOURCES} ${EFP_HEADERS} ${FLEX_marL_OUTPUTS} ${BISON_marP_OUTPUTS} ${FLEX_cemL_OUTPUTS} ${BISON_cemP_OUTPUTS})

# Compilation Features
set(CMAKE_CXX_STANDARD 14)
target_link_libraries(${PROJECT_EXE} LINK_PUBLIC ${Boost_LIBRARIES})
target_compile_features(${PROJECT_EXE} PRIVATE cxx_std_${CMAKE_CXX_STANDARD})
target_compile_options(${PROJECT_EXE} PRIVATE -Wall) #To show one error at the time

# Auto-ignore build directory
if (NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
    file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif ()
