set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "")
set(CMAKE_C_FLAGS_MINSIZEREL "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "")
set(CMAKE_CXX_FLAGS_MINSIZEREL "")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(project_settings INTERFACE)
target_compile_options(project_settings INTERFACE
        $<$<CONFIG:Debug>:-Os>
        $<$<AND:$<CONFIG:MinSizeRel>,$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>>:-Os>
        $<$<AND:$<CONFIG:MinSizeRel>,$<OR:$<C_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:Clang>>>:-Oz>
        $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-O3>
        $<$<OR:$<CONFIG:RelWithDebInfo>,$<CONFIG:Debug>>:-g3>
)

# Perform compiler flag tests
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

function(set_flags_conditionally language flag)
    if (${language} STREQUAL C)
        check_c_compiler_flag(${flag} flag_exists)
        if (${flag_exists})
            target_compile_options(project_settings INTERFACE $<$<COMPILE_LANGUAGE:C>:${flag}>)
        endif ()
    elseif (${language} STREQUAL CXX)
        check_cxx_compiler_flag(${flag} flag_exists)
        if (${flag_exists})
            target_compile_options(project_settings INTERFACE $<$<COMPILE_LANGUAGE:CXX>:${flag}>)
        endif ()
    else ()
        message(FATAL_ERROR "set_flags_conditionally called with language ${language} failed")
    endif ()
endfunction()

set_flags_conditionally(C -ffunction-sections)
set_flags_conditionally(CXX -ffunction-sections)
set_flags_conditionally(C -fdata-sections)
set_flags_conditionally(CXX -fdata-sections)
set_flags_conditionally(C -fmessage-length=0)
set_flags_conditionally(CXX -fmessage-length=0)
set_flags_conditionally(CXX -fno-exceptions)
set_flags_conditionally(CXX -fno-rtti)
set_flags_conditionally(CXX -fno-use-cxa-atexit)
set_flags_conditionally(CXX -Wno-deprecated-declarations)
