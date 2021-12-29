set(LIBRARY_NAME hermesbdd-all)

add_library(${LIBRARY_NAME} OBJECT
    bdd.hpp
    bdd.cpp
    node.hpp
    node.cpp
    memory_manager.cpp
    tree.cpp
    tree.hpp
    hash.hpp
    cache.cpp
    cache.hpp
    bdd_internal.hpp)

if(COVERAGE)
    target_link_libraries(${LIBRARY_NAME} PUBLIC coverage_config)
endif()
