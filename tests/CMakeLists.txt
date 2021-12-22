set(UNIT_TESTS
    test_hermesbdd
    test_nqueens
)

foreach(TEST ${UNIT_TESTS})
    add_executable(${TEST} ${TEST}.cpp)
    if(COVERAGE)
        target_compile_options(${TEST} PUBLIC ${COVERAGE_COMPILER_FLAGS})
    endif()
    add_test(${TEST} ${TEST})
    target_link_libraries(${TEST} hermesbdd)
endforeach()