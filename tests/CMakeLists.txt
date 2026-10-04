add_executable(
    ENG2D_test
    main.cpp
)

target_link_libraries(
    ENG2D_test
    PRIVATE
        ENG2D::ENG2D
)

target_compile_features(
    ENG2D_test
    PRIVATE
        cxx_std_20
)

# copy runtime DLLs next to the executable on Windows

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/assets DESTINATION ${CMAKE_BINARY_DIR})

if(WIN32)

    add_custom_command(
        TARGET ENG2D_test
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_FILE_DIR:ENG2D_test>"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3_image::SDL3_image>"
            "$<TARGET_FILE_DIR:ENG2D_test>"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
            "$<TARGET_FILE_DIR:ENG2D_test>"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3_mixer::SDL3_mixer>"
            "$<TARGET_FILE_DIR:ENG2D_test>"
    )

endif()