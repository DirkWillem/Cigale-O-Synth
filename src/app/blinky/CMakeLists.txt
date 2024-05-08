function(add_target MCU TARGET)
    add_executable(blinky_${TARGET}
            "target/${TARGET}/initialize.cpp"
            "target/${TARGET}/interrupts_callbacks.h"
            "target/${TARGET}/peripherals.h"

            "initialize.h"
            "main.cpp")

    target_link_libraries(blinky_${TARGET} PRIVATE
            -Wl,--whole-archive
            stm32_system
            ${MCU}_hal
            -Wl,--no-whole-archive)

    target_include_directories(blinky_${TARGET} PRIVATE
            "${CMAKE_CURRENT_LIST_DIR}"
            "${CMAKE_CURRENT_LIST_DIR}/target/${TARGET}")
endfunction()


if ("${TARGET}" STREQUAL "arm-cortex-m0plus")
#    add_executable(blinky
#            main.cpp
#            peripherals/peripherals.h)
#
    add_target(stm32g0b1 nucleo64_g0b1)
endif ()

if ("${TARGET}" STREQUAL "arm-cortex-m4")
    add_target(stm32g474 nucleo64_g474)
endif ()