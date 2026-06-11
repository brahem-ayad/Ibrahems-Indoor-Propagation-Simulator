include(FetchContent)

FetchContent_Declare(
    cdt
    GIT_REPOSITORY https://github.com/artem-ogre/CDT.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(cdt)

# Create a clean, normal interface target for our project to use
if(NOT TARGET CDT)
    add_library(CDT INTERFACE)
    target_include_directories(CDT INTERFACE ${cdt_SOURCE_DIR}/include)
    
    # Now we can safely create the namespaced alias that CMake expects
    add_library(CDT::CDT ALIAS CDT)
endif()
