# Automaticly generated by wsjcpp@v0.2.0
cmake_minimum_required(VERSION 3.0)

project(unit-tests C CXX)
add_definitions(-DWSJCPP_APP_VERSION="ut-v0.0.4")
add_definitions(-DWSJCPP_APP_NAME="unit-tests-wsjcpp-jsonrpc20")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${unit-tests_SOURCE_DIR})

set (WSJCPP_LIBRARIES "")
set (WSJCPP_INCLUDE_DIRS "")
set (WSJCPP_SOURCES "")

find_package(Threads REQUIRED)
list (APPEND WSJCPP_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

# wsjcpp-core:v0.2.1
list (APPEND WSJCPP_INCLUDE_DIRS "../src.wsjcpp/wsjcpp_core/")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/wsjcpp_core/wsjcpp_core.cpp")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/wsjcpp_core/wsjcpp_core.h")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/wsjcpp_core/wsjcpp_unit_tests.cpp")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/wsjcpp_core/wsjcpp_unit_tests.h")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/wsjcpp_core/wsjcpp_unit_tests_main.cpp")

# nlohmann/json:v3.9.1
list (APPEND WSJCPP_INCLUDE_DIRS "../src.wsjcpp/nlohmann_json/")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/nlohmann_json/json.hpp")

# wsjcpp-validators:v0.1.2
list (APPEND WSJCPP_INCLUDE_DIRS "../src.wsjcpp/wsjcpp_validators/")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/wsjcpp_validators/wsjcpp_validators.h")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/wsjcpp_validators/wsjcpp_validators.cpp")

# wsjcpp-jsonrpc20:v0.0.4
list (APPEND WSJCPP_INCLUDE_DIRS "../src")
list (APPEND WSJCPP_SOURCES "../src/wsjcpp_jsonrpc20.h")
list (APPEND WSJCPP_SOURCES "../src/wsjcpp_jsonrpc20.cpp")
list (APPEND WSJCPP_SOURCES "../src/wsjcpp_jsonrpc20_export_cli_python.h")
list (APPEND WSJCPP_SOURCES "../src/wsjcpp_jsonrpc20_export_cli_python.cpp")
list (APPEND WSJCPP_SOURCES "../src/wsjcpp_jsonrpc20_export_cli_webjs.h")
list (APPEND WSJCPP_SOURCES "../src/wsjcpp_jsonrpc20_export_cli_webjs.cpp")
list (APPEND WSJCPP_SOURCES "../src/wsjcpp_jsonrpc20_export_cli_base.h")
list (APPEND WSJCPP_SOURCES "../src/wsjcpp_jsonrpc20_export_cli_base.cpp")

# unit-tests
list (APPEND WSJCPP_INCLUDE_DIRS "src")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_json_rpc20_user_session.cpp")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_json_rpc20_param_def.cpp")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_request_server_api.cpp")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_parsing_request_params.cpp")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_request_parse_income_data.cpp")

include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.user-custom.txt)


include_directories(${WSJCPP_INCLUDE_DIRS})

add_executable ("unit-tests" ${WSJCPP_SOURCES})

target_link_libraries("unit-tests" -lpthread ${WSJCPP_LIBRARIES} )

