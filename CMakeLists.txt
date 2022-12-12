cmake_minimum_required(VERSION 3.16)
project(Amati VERSION 0.0.1)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

include(FetchContent)
set(FETCHCONTENT_QUIET false)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE
    GIT_TAG 7.0.1
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(juce juce)
find_package(FAUST)
set(BaseTargetName Amati)

# The following few lines are copied from https://github.com/eyalamirmusic/JUCECmakeRepoPrototype
# Being a Linux user, I don't really know to what extent they are necessary, or whether they are even correct.
# If you are building on another OS, please tell me about your experience!
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version" FORCE)

option(UniversalBinary "Build universal binary for mac" OFF)

if(UniversalBinary)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
endif()

# Static linking in Windows
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

# By default we don't want Xcode schemes to be made for modules, etc
set(CMAKE_XCODE_GENERATE_SCHEME OFF)

# -------------------------------------------------------------------------------
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

# Adds all the module sources so they appear correctly in the IDE
# Must be set before JUCE is added as a sub-dir (or any targets are made)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

# This is a failed attempt to bury ALL_BUILD in Targets/
# This should be called before any target is made
# Bug in Xcode? https://gitlab.kitware.com/cmake/cmake/-/issues/21383
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")

# Create a /Modules directory in the IDE with the JUCE Module code
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)

set(Formats VST3 AU Standalone)

juce_add_plugin(${BaseTargetName}
    COMPANY_NAME "Glocq"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE L0cq # Not sure I understand what those
    PLUGIN_CODE Mati              # are about, I hope I'm doing it right
    FORMATS ${Formats}
    PRODUCT_NAME ${BaseTargetName}
)

# C++ std
target_compile_features(${BaseTargetName} PRIVATE cxx_std_17)

juce_generate_juce_header(${BaseTargetName})

set(SourceFiles
        Source/ConsoleComponent.h
        Source/EditorComponent.h
        Source/FaustCodeTokenizer.h
        Source/FaustProgram.h
        Source/ParamEditor.h
        Source/PluginEditor.h
        Source/PluginProcessor.h
        Source/SettingsComponent.h

        Source/ConsoleComponent.cpp
        Source/EditorComponent.cpp
        Source/FaustCodeTokenizer.cpp
        Source/FaustProgram.cpp
        Source/ParamEditor.cpp
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
        Source/SettingsComponent.cpp
)

target_sources(${BaseTargetName} PRIVATE ${SourceFiles})

# No, we don't want our source buried in extra nested folders
set_target_properties("${PROJECT_NAME}" PROPERTIES FOLDER "")

# The source tree should uhhh, still look like the source tree, yo
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "" FILES ${SourceFiles})

# This cleans up the folder organization, especially on Xcode.
# It tucks the Plugin varieties into a "Targets" folder and generate an Xcode Scheme manually
# Xcode scheme generation is turned off globally to limit noise from other targets
# The non-hacky way of doing this is via the global PREDEFINED_TARGETS_FOLDER propety
# However that doesn't seem to be working in Xcode
# Not all plugin types (au, vst) available on each build type (win, macos, linux)
foreach(target ${Formats} "All")
    if(TARGET ${BaseTargetName}_${target})
        set_target_properties(${BaseTargetName}_${target} PROPERTIES

            # Tuck the actual plugin targets into a folder where they won't bother us
            FOLDER "Targets"

            # MacOS only: Sets the default executable that Xcode will open on build
            # For this exact path to to work, manually build the AudioPluginHost.xcodeproj in the JUCE subdir
            XCODE_SCHEME_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/extras/AudioPluginHost/Builds/MacOSX/build/Debug/AudioPluginHost.app"

            # Let us build the target in Xcode
            XCODE_GENERATE_SCHEME ON)
    endif()
endforeach()

target_compile_definitions(${BaseTargetName}
    PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0

    # Change the following if you want more accessible VST parameters!
    PARAM_COUNT=16
)

target_link_libraries(${BaseTargetName} PRIVATE
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
    faust
)
