cmake_minimum_required(VERSION 3.16)

project(ets2-telemetry-server LANGUAGES CXX)

add_library(ets2-telemetry-server SHARED
    ets2-telemetry/src/ets2-telemetry.cpp
    ets2-telemetry/src/scs_config_handlers.cpp
    ets2-telemetry/src/sharedmemory.cpp
)

target_include_directories(ets2-telemetry-server PRIVATE
    ets2-telemetry/inc
    scs_sdk_1_5/include
)

target_compile_features(ets2-telemetry-server PRIVATE cxx_std_98)

set_target_properties(ets2-telemetry-server PROPERTIES
    PREFIX ""
)

if(WIN32)
    target_sources(ets2-telemetry-server PRIVATE ets2-telemetry/ets2-telemetry.def)
    target_compile_definitions(ets2-telemetry-server PRIVATE WIN32 _WINDOWS _USRDLL TELEMETRY_EXPORTS)
    target_compile_definitions(ets2-telemetry-server PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<NOT:$<CONFIG:Debug>>:NDEBUG>
        $<$<NOT:$<CONFIG:Debug>>:_CRT_SECURE_NO_WARNINGS>
    )
else()
    include(CheckLibraryExists)
    check_library_exists(rt shm_open "" HAVE_SHM_OPEN_IN_RT)
    if(HAVE_SHM_OPEN_IN_RT)
        target_link_libraries(ets2-telemetry-server PRIVATE rt)
    endif()
endif()
