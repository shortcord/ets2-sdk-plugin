cmake_minimum_required(VERSION 3.16)

project(ets2-telemetry-server LANGUAGES CXX)

add_library(ets2-telemetry-server SHARED
    ets2-telemetry/src/ets2-telemetry.cpp
    ets2-telemetry/src/scs_config_handlers.cpp
    ets2-telemetry/src/sharedmemory.cpp
)

target_include_directories(ets2-telemetry-server PRIVATE
    ets2-telemetry/inc
    scs_sdk_1_14/include
)

target_compile_features(ets2-telemetry-server PRIVATE cxx_std_98)

set_target_properties(ets2-telemetry-server PROPERTIES
    PREFIX ""
)

# Windows-specific configuration
if(WIN32)
    set(BUILD_SHARED_LIBS OFF)
else()
    set(BUILD_SHARED_LIBS ON)
endif()
endif()
