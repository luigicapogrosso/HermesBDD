set(LIBRARY_NAME hermesbdd-all)

add_library(${LIBRARY_NAME} OBJECT
    bdd.cpp
    node.cpp
    memory_manager.cpp
    tree.cpp
    cache.cpp)

if(COVERAGE)
    target_link_libraries(${LIBRARY_NAME} PUBLIC coverage_config)
endif()
