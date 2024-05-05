if ("${TARGET}" STREQUAL "arm-cortex-m0plus")
    add_executable(blinky
            main.cpp
            interrupts.cpp)

    target_link_libraries(blinky
            stm32g0b1_hal
            -Wl,--whole-archive
            stm32_system
            -Wl,--no-whole-archive)
endif ()