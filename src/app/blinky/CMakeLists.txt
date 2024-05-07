if ("${TARGET}" STREQUAL "arm-cortex-m0plus")
    add_executable(blinky
            main.cpp
            peripherals/nucleo64.h)

    target_link_libraries(blinky
            -Wl,--whole-archive
            stm32_system
            stm32g0b1_hal
            -Wl,--no-whole-archive)
endif ()