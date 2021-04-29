cmake_minimum_required(VERSION 3.16)
project(Amati VERSION 0.0.1)


set (BaseTargetName Amati)


# The following few lines are copied from https://github.com/eyalamirmusic/JUCECmakeRepoPrototype
# Being a Linux user, I don't really know to what extent they are necessary, or whether they are even correct.
# If you are building on another OS, please tell me aout your experience!

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version" FORCE)

option(UniversalBinary "Build universal binary for mac" OFF)

if (UniversalBinary)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
endif()

# Static linking in Windows
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

#Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

#-------------------------------------------------------------------------------

option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)


add_subdirectory(JUCE)

include_directories(
	/usr/local/include
)


juce_add_plugin("${BaseTargetName}"
    COMPANY_NAME "Glocq"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE L0cq # Not sure I understand what those
    PLUGIN_CODE Mati              # are about, I hope I'm doing it right
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "Amati"
)

juce_generate_juce_header(${BaseTargetName})

target_sources(${BaseTargetName} PRIVATE
    Source/DebugComponent.cpp
    Source/EditorComponent.cpp
    Source/FaustCodeTokenizer.cpp
    Source/FaustProgram.cpp
    Source/ParamEditor.cpp
    Source/PluginEditor.cpp
    Source/PluginProcessor.cpp
)

target_compile_definitions(${BaseTargetName}
    PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCER_ENABLE_GPL_MODE=1
    # Change the following if you want more accessible VST parameters!
    PARAM_COUNT=16
)

target_link_libraries(${BaseTargetName} PRIVATE
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
    -L/usr/local/lib faust
)
