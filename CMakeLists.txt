cmake_minimum_required(VERSION 3.15)

# ==============================================================================
# Define the options for the nn-inference-template
# ==============================================================================

option(NNITEMPLATE_WITH_INSTALL "Add install targets" ON)

# ==============================================================================
# Setup the project
# ==============================================================================

set (PROJECT_NAME nn-inference-template)

project (${PROJECT_NAME} VERSION 0.0.2)

# Sets the minimum macOS version
if (APPLE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum version of the target platform" FORCE) 
	if(CMAKE_OSX_DEPLOYMENT_TARGET)
		message("The minimum macOS version is set to " $CACHE{CMAKE_OSX_DEPLOYMENT_TARGET}.)
	endif()
endif ()

# Sets the cpp language minimum
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NNITEMPLATE_WITH_INSTALL)
    set(ANIRA_WITH_INSTALL ON)
endif()

# ==============================================================================
# Add the anira library
# ==============================================================================

# Either add the anira library as a git submodule and use add_subdirectory
add_subdirectory(modules/anira)

# Or download the latest release from github and the folder to the cmake prefix path and use find_package
# list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/modules/anira-x.x.x")
# find_package(anira REQUIRED)

# ==============================================================================
# Add the JUCE library
# ==============================================================================

# EXCLUDE_FROM_ALL so that juce does not get installed, because we build it as static lib
add_subdirectory(modules/JUCE EXCLUDE_FROM_ALL)

# ==============================================================================
# Plug-In / Standalone setup
# ==============================================================================

if(APPLE)
set(FORMATS_TO_BUILD AU VST3 Standalone)
else()
set(FORMATS_TO_BUILD VST3 Standalone)
endif()

# Include the assets and common directories
add_subdirectory(assets)

set(TARGET_NAME NN_Inference_Template)
set(PRODUCT_NAME "NN Inference Template")

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

VST3_AUTO_MANIFEST FALSE

if(APPLE)
HARDENED_RUNTIME_ENABLED TRUE
HARDENED_RUNTIME_OPTIONS "com.apple.security.device.audio-input"
MICROPHONE_PERMISSION_ENABLED TRUE
MICROPHONE_PERMISSION_TEXT "Need access to your audio interface"
endif()

FORMATS ${FORMATS_TO_BUILD}                     # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
PRODUCT_NAME ${PRODUCT_NAME}			            # The name of the final executable, which can differ from the target name
)

juce_generate_juce_header(${TARGET_NAME})

# Set the model path as compile definition for the build interface
set(GUITARLSTM_MODELS_PATH_TENSORFLOW "${CMAKE_CURRENT_LIST_DIR}/modules/GuitarLSTM/tensorflow-version/models/")
set(GUITARLSTM_MODELS_PATH_PYTORCH "${CMAKE_CURRENT_LIST_DIR}/modules/GuitarLSTM/pytorch-version/models/")

target_compile_definitions(${TARGET_NAME}
    PUBLIC
    GUITARLSTM_MODELS_PATH_TENSORFLOW="${GUITARLSTM_MODELS_PATH_TENSORFLOW}"
    GUITARLSTM_MODELS_PATH_PYTORCH="${GUITARLSTM_MODELS_PATH_PYTORCH}"
)

# Add all source files to file list
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h)

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
        anira::anira
		BinaryData
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_core
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# ==============================================================================
# Add install target for the plugin
# ==============================================================================

if(NNITEMPLATE_WITH_INSTALL)
    include(cmake/install.cmake)
endif()  
