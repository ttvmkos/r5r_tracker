# -----------------------------------------------------------------------------
# Start the source file list
# -----------------------------------------------------------------------------
macro( start_sources )
    add_sources( SOURCE_GROUP ""
        # Add the CMakeLists file to the project filter root
        "CMakeLists.txt"
)
endmacro()

# -----------------------------------------------------------------------------
# Add source files to target within a project filter
# -----------------------------------------------------------------------------
macro( add_sources )
    set( options )
    set( oneValueArgs SOURCE_GROUP )
    set( multiValueArgs )

    cmake_parse_arguments( ADD_SOURCES
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN}
    )

    if( NOT ADD_SOURCES_SOURCE_GROUP )
        message( FATAL_ERROR "SOURCE_GROUP must be provided" )
    endif()

    source_group( "${ADD_SOURCES_SOURCE_GROUP}" FILES ${ADD_SOURCES_UNPARSED_ARGUMENTS} )
    target_sources( ${PROJECT_NAME} PRIVATE ${ADD_SOURCES_UNPARSED_ARGUMENTS} )
endmacro()

# -----------------------------------------------------------------------------
# End the source file list ( optional parameter sets the runtime output dir )
# -----------------------------------------------------------------------------
macro( end_sources )
    if( NOT "${ARGN}" STREQUAL "" ) # Check if an output directory is passed
        set_target_output_dirs( ${PROJECT_NAME} ${ARGN} )
    else()
        set_target_output_dirs( ${PROJECT_NAME} "${BUILD_OUTPUT_DIR}/" )
    endif()
endmacro()

# -----------------------------------------------------------------------------
# Add modules to the project
# -----------------------------------------------------------------------------
macro( add_module MODULE_TYPE MODULE_NAME REUSE_PCH FOLDER_NAME WARNINGS_AS_ERRORS APPLY_COMPILE_OPTIONS )
    project( ${MODULE_NAME} )

    if( ${MODULE_TYPE} STREQUAL "lib" )
        add_library( ${PROJECT_NAME} )
    elseif( ${MODULE_TYPE} STREQUAL "shared_lib" )
        add_library( ${PROJECT_NAME} SHARED )
        target_link_options( ${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Release>:/LTCG>"
    )
    elseif( ${MODULE_TYPE} STREQUAL "exe" )
        add_executable( ${PROJECT_NAME} )
        target_link_options( ${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Release>:/LTCG>"
    )
    else()
        message( FATAL_ERROR "Invalid module type: ${MODULE_TYPE}; expected 'lib', 'shared_lib', or 'exe'." )
    endif()

    if ( NOT "${REUSE_PCH}" STREQUAL "" )
        target_precompile_headers( ${PROJECT_NAME} REUSE_FROM ${REUSE_PCH} )
    endif()

    set_target_properties( ${MODULE_NAME} PROPERTIES FOLDER ${FOLDER_NAME} )

    if( ${OPTION_WARNINGS_AS_ERRORS} )
        warnings_as_errors( ${PROJECT_NAME} ${WARNINGS_AS_ERRORS} )
    endif()

    if ( NOT "${APPLY_COMPILE_OPTIONS}" STREQUAL "FALSE" )
        target_compile_options( ${PROJECT_NAME} PRIVATE
            $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Ob2>
            $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Oi>
            $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Ot>
            $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/GS->
            $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Gy>
            $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/fp:fast>
    )
    endif()
endmacro()

# -----------------------------------------------------------------------------
# Initialize global compiler defines
# -----------------------------------------------------------------------------
macro( define_compiler_variables )
    if( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
        add_definitions( -DCOMPILER_MSVC )
    elseif( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
        add_definitions( -DCOMPILER_CLANG )
    elseif( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
        add_definitions( -DCOMPILER_GCC )
    else()
        message( FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}" )
    endif()
endmacro()

# -----------------------------------------------------------------------------
# Apply whole program optimization for this target in release ( !slow! )
# -----------------------------------------------------------------------------
macro( whole_program_optimization )
    target_compile_options( ${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/GL>
    )
endmacro()

# -----------------------------------------------------------------------------
# Toggles wether or not to treat warnings as errors
# -----------------------------------------------------------------------------
macro( warnings_as_errors TARGET FLAG )
    if( ${FLAG} )
        if( MSVC )
            target_compile_options( ${TARGET} PRIVATE /WX )
        else()
            target_compile_options( ${TARGET} PRIVATE -Werror )
        endif()
    else()
        if( MSVC )
            target_compile_options( ${TARGET} PRIVATE "/wd4996" )
        else()
            target_compile_options( ${TARGET} PRIVATE "-Wno-error" )
        endif()
    endif()
endmacro()

# -----------------------------------------------------------------------------
# Disables verbose warnings caused within thirdparty code ( !only use on thirdparty projects! )
# -----------------------------------------------------------------------------
macro( thirdparty_suppress_warnings )
    if( MSVC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
        target_compile_options( ${PROJECT_NAME} PRIVATE
            /wd4057 # 'function': 'int *' differs in indirection to slightly different base types from 'unsigned int [4]'
            /wd4100 # Unreferenced formal parameter.
            /wd4131 # Using old-style declarations
            /wd4152 # Function/data pointer conversion in expression.
            /wd4200 # Zero-sized array in union; SDL2 uses this for compiler compatibility.
            /wd4201 # Nameless struct/union.
            /wd4204 # nonstandard extension used: non-constant aggregate initializer.
            /wd4221 # nonstandard extension used: 'value': cannot be initialized using address of automatic variable 'symbol'
            /wd4244 # Type conversion truncation; protobuf has many, but this appears intentional.
            /wd4245 # 'return': conversion signed/unsigned mismatch
            /wd4267 # Type conversion truncation; protobuf has many, but this appears intentional.
            /wd4307 # Integral constant overflow.
            /wd4389 # Signed/unsigned mismatch.
            /wd4456 # Declaration hides previous local declaration.
            /wd4457 # Declaration hides function parameter.
            /wd4505 # Unreferenced local function has been removed.
            /wd4701 # potentially uninitialized local variable.
            /wd4702 # Unreachable code.
        )
    endif()
    warnings_as_errors( ${PROJECT_NAME} FALSE )
endmacro()
