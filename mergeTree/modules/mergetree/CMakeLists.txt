ivw_module(mergeTree)

set(HEADER_FILES
    include/inviwo/mergetree/datastructure/mergetree.h
    include/inviwo/mergetree/mergetreemodule.h
    include/inviwo/mergetree/mergetreemoduledefine.h
    include/inviwo/mergetree/port/mergetreeport.h
    include/inviwo/mergetree/processors/mergetreegenerate.h
    include/inviwo/mergetree/processors/mergetreesimplification.h
    include/inviwo/mergetree/processors/mergetreevis.h
    include/inviwo/mergetree/utils/unionfind.h
)
ivw_group("Header Files" ${HEADER_FILES})

set(SOURCE_FILES
    src/datastructure/mergetree.cpp
    src/mergetreemodule.cpp
    src/processors/mergetreegenerate.cpp
    src/processors/mergetreesimplification.cpp
    src/processors/mergetreevis.cpp
    src/utils/unionfind.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

set(SHADER_FILES
        # Add shaders
        )
ivw_group("Shader Files" ${SHADER_FILES})

set(TEST_FILES
        tests/unittests/mergetree-unittest-main.cpp
        )
ivw_add_unittest(${TEST_FILES})

ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

# Add shader directory to install package
#ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)
