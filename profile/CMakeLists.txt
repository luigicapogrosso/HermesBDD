set(PROFILE_FILES
    profile_replaceme
    profile_ITE
)

foreach(PROFILE ${PROFILE_FILES})
    add_executable(${PROFILE} ${PROFILE}.cpp)
    target_link_libraries(${PROFILE} hermesbdd)
endforeach()

add_custom_target(profiles)