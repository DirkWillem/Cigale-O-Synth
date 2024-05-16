function(add_thermocouples_app MCU TARGET)
    add_application("thermocouples"
            MCU "${MCU}"
            TARGET "${TARGET}"
            SRCS
            "target/${TARGET}/initialize.cpp"
            "target/${TARGET}/interrupts_callbacks.h"
            "target/${TARGET}/peripherals.h"

            "initialize.h"
            "main.cpp"
            LIBS fp)
endfunction()

if ("${TARGET}" STREQUAL "arm-cortex-m4")
    add_thermocouples_app(stm32g474 nucleo64_g474)
endif ()