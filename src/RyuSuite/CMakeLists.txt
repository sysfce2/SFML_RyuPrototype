set (SRC 
        "RAnimator.cpp"
    )

set(HEADER
     "${RyuPrototype_SOURCE_DIR}/include/RyuSuite/RAnimator.h"
)

add_library(ryusuitelib SHARED "${SRC}" "${HEADER}")

target_include_directories(ryusuitelib PUBLIC "${RyuPrototype_SOURCE_DIR}/include")
# target_link_libraries(ryuptlib PUBLIC ryusuitelib)


#duplicate code
find_package(SFML 2.5.1 COMPONENTS audio graphics window system network)
find_package(box2d CONFIG REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(ImGui-SFML REQUIRED)
# find_package(fmt)
find_package(GTest CONFIG REQUIRED)

SET(EXT_LIBS_TO_LINK_TO ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} box2d ImGui-SFML::ImGui-SFML nlohmann_json::nlohmann_json )#fmt::fmt)
target_link_libraries(ryusuitelib PUBLIC ${EXT_LIBS_TO_LINK_TO})
target_link_libraries(ryuptlib PUBLIC ryusuitelib)
