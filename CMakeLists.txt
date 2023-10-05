cmake_minimum_required(VERSION 3.15)

# Sets the minimum macOS version
if (APPLE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum version of the target platform" FORCE) 
	if(CMAKE_OSX_DEPLOYMENT_TARGET)
		message("The minimum macOS version is set to " $CACHE{CMAKE_OSX_DEPLOYMENT_TARGET}.)
	endif()
endif ()

cmake_minimum_required(VERSION 3.15)

set (PROJECT_NAME nn-inference-template)

project (${PROJECT_NAME} VERSION 0.0.1)

option(BUILD_ARCHITECTURE "Architecture to build for")

# Sets the cpp language minimum
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Export only public symbols
# We set this option for all our libraries since the google benchmark lib are setup with hidden visibility
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)


# ==============================================================================

# Plug-In / Standalone setup

# ==============================================================================

# Compilation options
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

if(APPLE)
	set (FORMATS_TO_BUILD AU VST3 Standalone)
else()
	set (FORMATS_TO_BUILD VST3 Standalone)
endif()

# Juce is included via a submodule
add_subdirectory(modules/JUCE)

# Install inference engines

# ONNXRuntime
include(cmake/SetupOnnxRuntime.cmake)
# LibTorch
include(cmake/SetupLibTorch.cmake)
# TensorflowLite
include(cmake/SetupTensorflowLite.cmake)

# Add minimal examples
add_subdirectory(minimal-inference)

# Include the assets and common directories
add_subdirectory(assets)

set (TARGET_NAME NN_Inference_Template)

juce_add_plugin(${TARGET_NAME}
		# VERSION ...                               # Set this if the plugin version is different to the project version
		# ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
		# ICON_SMALL ...
		COMPANY_NAME "AckvaSchulz"
		# IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
		# NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
		# NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
		# IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
		# EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
		# COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
		PLUGIN_MANUFACTURER_CODE AcSc              # A four-character manufacturer id with at least one upper-case character
		PLUGIN_CODE Nnit                            # A unique four-character plugin id with exactly one upper-case character
		# GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case

		if(APPLE)
            HARDENED_RUNTIME_ENABLED TRUE
            HARDENED_RUNTIME_OPTIONS "com.apple.security.device.audio-input"
            MICROPHONE_PERMISSION_ENABLED TRUE
            MICROPHONE_PERMISSION_TEXT "Need access to your audio interface"
		endif()

		FORMATS ${FORMATS_TO_BUILD}                     # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
		PRODUCT_NAME "NN Inference Template"			            # The name of the final executable, which can differ from the target name
		)

juce_generate_juce_header(${TARGET_NAME})

# Add all source files to file list
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h)

# For now remove unused dsp files
file(GLOB_RECURSE SOURCES_DSP CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/dsp/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/dsp/*.h)
foreach(SOURCE ${SOURCES_DSP})
    list(REMOVE_ITEM SOURCES ${SOURCE})
endforeach()


# Add all sources to target
target_sources(${TARGET_NAME} PRIVATE ${SOURCES})

# Add include directories for all folders in the source
file(GLOB_RECURSE SOURCE_DIRS LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/source/*)
list(APPEND SOURCE_DIRS ${CMAKE_CURRENT_LIST_DIR}/source)

foreach (DIR ${SOURCE_DIRS})
    if (IS_DIRECTORY ${DIR})
        target_include_directories(${TARGET_NAME} PRIVATE ${DIR})
    endif ()
endforeach ()

# Make the folder structure visible in IDEs like Xcode
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX "source" FILES ${SOURCES})

target_compile_definitions(${TARGET_NAME}
    PRIVATE
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    DONT_SET_USING_JUCE_NAMESPACE=1)

target_link_libraries(${TARGET_NAME}
    PRIVATE
		onnxruntime
		torch
        tensorflowlite_c
		# BinaryData
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_core
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)


# ==============================================================================

# Test setup

# ==============================================================================

# enable ctest
enable_testing()

# TODO start and stop executable test
cmake_policy(SET CMP0135 NEW)
# This module enables populating content at configure time via any method supported by the ExternalProject module. Whereas ExternalProject_Add() downloads at build time, the FetchContent module makes content available immediately, allowing the configure step to use the content in commands like add_subdirectory(), include() or file() operations.
include(FetchContent)

# Externally provided libraries
# Using zip files instead is faster
FetchContent_Declare(googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)

FetchContent_Declare(benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.3)

# This command ensures that each of the named dependencies are made available to the project by the time it returns. If the dependency has already been populated the command does nothing. Otherwise, the command populates the dependency and then calls add_subdirectory() on the result.
FetchContent_MakeAvailable(googletest)

# For benchmark we want to set the BENCMARK_ENABLE_TESTING to OFF therefore we cannot use FetchContent_MakeAvailable()
# Check if population has already been performed
FetchContent_GetProperties(benchmark)
if(NOT benchmark_POPULATED)
  # Fetch the content using previously declared details
  FetchContent_Populate(benchmark)

  # Set custom variables, policies, etc.
  set(BENCHMARK_ENABLE_TESTING OFF)

  if (APPLE AND (BUILD_ARCHITECTURE STREQUAL "arm64" OR CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64"))
    set(HAVE_STD_REGEX ON)
    set(RUN_HAVE_STD_REGEX 1)
  endif()
  
  # Bring the populated content into the build
  add_subdirectory(${benchmark_SOURCE_DIR} ${benchmark_BINARY_DIR})
endif()

# get all test cpp and header files
file(GLOB_RECURSE TEST_SOURCES_${TARGET_NAME} CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/*.h)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/test PREFIX "test_${TARGET_NAME}" FILES ${TEST_SOURCES_${TARGET_NAME}})

# Setup the test executable
add_executable(Test_${TARGET_NAME} ${TEST_SOURCES_${TARGET_NAME}})
set_property(TARGET Test_${TARGET_NAME} PROPERTY CXX_STANDARD 20)

# Add include directories for all folders in test
file(GLOB_RECURSE TEST_DIRS_${TARGET_NAME} LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/test)

foreach (DIR ${TEST_DIRS}_${TARGET_NAME})
    if (IS_DIRECTORY ${DIR})
        target_include_directories(Test_${TARGET_NAME} PRIVATE ${DIR})
    endif ()
endforeach ()

# Link the test executable against gtest_main and google benchmark
target_link_libraries(Test_${TARGET_NAME} PRIVATE gtest_main benchmark::benchmark)

# We can't link again to the shared juce target without ODL violations (https://github.com/sudara/pamplejuce/issues/31, https://forum.juce.com/t/windows-linker-issue-on-develop/55524/2)
# Therefore we steal the compile definitions and include directories from the main target and pass them to our test target
# Since we linked the shared juce targets in PRIVATE mode, they are not linked to the test target again
target_link_libraries(Test_${TARGET_NAME} PRIVATE ${TARGET_NAME})
target_compile_definitions(Test_${TARGET_NAME} PRIVATE $<TARGET_PROPERTY:${TARGET_NAME},COMPILE_DEFINITIONS>)
target_include_directories(Test_${TARGET_NAME} PRIVATE $<TARGET_PROPERTY:${TARGET_NAME},INCLUDE_DIRECTORIES>)

# include Loads and runs CMake code from the file given. Loads and runs CMake code from the file given.
include(GoogleTest)

if (BUILD_ARCHITECTURE STREQUAL CMAKE_HOST_SYSTEM_PROCESSOR OR BUILD_ARCHITECTURE STREQUAL OFF)
    # gtest_discover_tests will register a CTest test for each gtest and run them all in parallel with the rest of the Test.
    gtest_discover_tests(Test_${TARGET_NAME})
endif ()