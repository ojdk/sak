cmake_minimum_required(VERSION 3.10)

# Project name and version
project(sak VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Options for standalone builds
option(SAK_BUILD_TESTS "Build sak tests" ON)

# Set source and header files
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCES
  ${SOURCE_DIR}/src/TaskScheduler.cpp
  
  ${SOURCE_DIR}/src/json/json.cpp
  ${SOURCE_DIR}/src/json/json_impl.cpp
  ${SOURCE_DIR}/src/json/LohmannJsonValidater.cpp
  
  ${SOURCE_DIR}/src/telemetric/TelemetricExecutionTimeLimit.cpp
  
  ${SOURCE_DIR}/vcpkg.json
  )

set(HEADERS
  ${SOURCE_DIR}/include/sak/details.hpp
  ${SOURCE_DIR}/include/sak/expected.hpp
  ${SOURCE_DIR}/include/sak/sak_api.hpp
  ${SOURCE_DIR}/include/sak/TaskScheduler.hpp

  ${SOURCE_DIR}/include/sak/signal/signal.hpp

  ${SOURCE_DIR}/include/sak/json/json.hpp
  ${SOURCE_DIR}/include/sak/json/IJsonValidater.hpp
  
  ${SOURCE_DIR}/include/sak/telemetric/ITelemetricDevice.hpp
  ${SOURCE_DIR}/include/sak/telemetric/TelemetricExecutionTimeLimit.hpp
  
  ${SOURCE_DIR}/include/sak/logging/ILogSink.hpp
  ${SOURCE_DIR}/include/sak/logging/LogContextData.hpp
)

set(PRIVATE_HEADER_FILES
    ${SOURCE_DIR}/src/json/json_impl.hpp
    ${SOURCE_DIR}/src/json/LohmannJsonValidater.hpp
)

# Add library targets
add_library(sak_static STATIC ${SOURCES} ${HEADERS} ${PRIVATE_HEADER_FILES})
add_library(sak_shared SHARED ${SOURCES} ${HEADERS} ${PRIVATE_HEADER_FILES})

# Set target properties
set_target_properties(sak_static PROPERTIES OUTPUT_NAME "saklib")
set_target_properties(sak_shared PROPERTIES OUTPUT_NAME "sak")

# Specify include directories
target_include_directories(sak_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(sak_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link dependencies
find_package(nlohmann_json CONFIG REQUIRED)
find_package(nlohmann_json_schema_validator CONFIG REQUIRED)

target_link_libraries(sak_static
  nlohmann_json::nlohmann_json
  nlohmann_json_schema_validator::validator
)

target_link_libraries(sak_shared
  nlohmann_json::nlohmann_json
  nlohmann_json_schema_validator::validator
)

# Install targets
install(TARGETS sak_static sak_shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Add compile definitions for shared library
target_compile_definitions(sak_shared PUBLIC SAK_DYNAMIC)
target_compile_definitions(sak_shared PRIVATE SAK_EXPORT)

# Install headers
install(DIRECTORY include/ DESTINATION include)

# Enable testing if requested
if(SAK_BUILD_TESTS)
  enable_testing()

  find_package(GTest REQUIRED)

  add_executable(sak_tests
      ${SOURCE_DIR}/src/test_StrongType.cpp
      ${SOURCE_DIR}/src/test_expected.cpp
      ${SOURCE_DIR}/src/test_index.cpp
      ${SOURCE_DIR}/src/test_number.cpp
      ${SOURCE_DIR}/src/test_TaskScheduler.cpp

      ${SOURCE_DIR}/src/signal/test_signal.cpp

      ${SOURCE_DIR}/src/json/test_LohmannJsonValidater.cpp
      ${SOURCE_DIR}/src/json/test_json.cpp
      
      ${SOURCE_DIR}/src/telemetric/test_TelemetricExecutionTimeLimit.cpp
      ${SOURCE_DIR}/src/telemetric/test_mock_TelemetricDevice.hpp
  )

  target_include_directories(sak_tests PRIVATE ${SOURCE_DIR}/include)

  target_link_libraries(sak_tests
    GTest::GTest
    GTest::Main
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator::validator
    sak_static
  )

  include(GoogleTest)
  gtest_discover_tests(sak_tests)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           

  
# Add a custom target to run tests after a successful build
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS sak_tests
)

# Make the test target depend on the build target
add_dependencies(run_tests sak_tests)
endif()
