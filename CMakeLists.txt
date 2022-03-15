cmake_minimum_required(VERSION 3.16)

project(EFP VERSION 2.2
        DESCRIPTION "Epistemic Forward Planner"
        LANGUAGES C CXX)

set(
        EFP_SOURCES
        src/main.cpp
        include/actions/action.cpp
        include/actions/proposition.cpp
        include/actions/custom_event_models/cem.cpp
        include/actions/custom_event_models/cem_store.cpp
        include/actions/custom_event_models/cevent.cpp
        include/domain/domain.cpp
        include/domain/grounder.cpp
        include/domain/initially.cpp
        include/formulae/belief_formula.cpp
        include/heuristics/heuristics_manager.cpp
        include/heuristics/heuristics_template.ipp
#        include/heuristics/planning_graph.cpp
#        include/heuristics/planning_graph_template.ipp
        include/heuristics/satisfied_goals.cpp
        include/parse/cem_parser.cpp
        include/refactor/asp_maker.cpp
        include/search/planner.ipp
        include/states/kripke/kstate.cpp
        include/states/kripke/kstore.cpp
        include/states/kripke/kworld.cpp
        include/states/possibilities/pstate.cpp
        include/states/possibilities/pstore.cpp
        include/states/possibilities/pworld.cpp
        include/states/state_T.ipp
        include/update/product_update.cpp
        include/utilities/helper.cpp
        include/utilities/helper_t.ipp
        include/utilities/printer.cpp
        include/utilities/reader.cpp
        lib/bisimulation/bisimulation.cpp
#        lib/bisimulation/bisimulation_k.cpp
)

set(
        EFP_HEADERS
        include/actions/action.h
        include/actions/proposition.h
        include/actions/custom_event_models/cem.h
        include/actions/custom_event_models/cem_store.h
        include/actions/custom_event_models/cevent.h
        include/domain/domain.h
        include/domain/grounder.h
        include/domain/initially.h
        include/formulae/belief_formula.h
        include/heuristics/heuristics_manager.h
#        include/heuristics/planning_graph.h
        include/heuristics/satisfied_goals.h
        include/parse/cem_parser.h
        include/refactor/asp_maker.h
        include/search/planner.h
        include/states/kripke/kstate.h
        include/states/kripke/kstore.h
        include/states/kripke/kworld.h
        include/states/possibilities/pstate.h
        include/states/possibilities/pstore.h
        include/states/possibilities/pworld.h
        include/states/state_T.h
        include/update/product_update.h
        include/utilities/define.h
        include/utilities/helper.h
        include/utilities/helper_t.h
        include/utilities/printer.h
        include/utilities/reader.h
        lib/bisimulation/bisimulation.h
#       lib/bisimulation/bisimulation_k.h
)

# FLEX & BISON
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

message(STATUS "Flex version:  ${FLEX_VERSION}")
message(STATUS "Bison version: ${BISON_VERSION}")

flex_target(marL include/parse/mar.lex "${CMAKE_CURRENT_BINARY_DIR}/Parse/mar.lex.cc")
bison_target(marP include/parse/mar.y "${CMAKE_CURRENT_BINARY_DIR}/Parse/mar.tab.cc")
add_flex_bison_dependency(marL marP)

flex_target(cemL include/parse/cem.lex "${CMAKE_CURRENT_BINARY_DIR}/Parse/cem.lex.cc")
bison_target(cemP include/parse/cem.y "${CMAKE_CURRENT_BINARY_DIR}/Parse/cem.tab.cc")
add_flex_bison_dependency(cemL cemP)

# BOOST
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem)
message(STATUS "Boost version: ${Boost_VERSION}")

include_directories(${Boost_INCLUDE_DIR})

# EXECUTABLES
add_executable(efp ${EFP_SOURCES} ${EFP_HEADERS} ${FLEX_marL_OUTPUTS} ${BISON_marP_OUTPUTS} ${FLEX_cemL_OUTPUTS} ${BISON_cemP_OUTPUTS})
target_link_libraries(efp LINK_PUBLIC ${Boost_LIBRARIES})
target_compile_features(efp PRIVATE cxx_std_17)
target_compile_options(efp PRIVATE -Wall -Wextra)
