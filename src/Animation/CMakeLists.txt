set (SRC 
     "SpritesheetAnimation.cpp"
     "JsonParser.cpp"
    )

set(HEADER
     "${RyuPrototype_SOURCE_DIR}/include/RyuSuite/RAnimator.h"
)

add_library(ryuanimationlib SHARED "${SRC}" "${HEADER}")

target_include_directories(ryuanimationlib PUBLIC "${RyuPrototype_SOURCE_DIR}/include")
