set (SRC
     "AnimationManager.cpp"
     "SpritesheetAnimation.cpp"
     "JsonParser.cpp"
    )

set(HEADER
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Animation/AnimationManager.h"
     "${RyuPrototype_SOURCE_DIR}/include/RyuSuite/RAnimator.h"
)

add_library(ryuanimationlib SHARED "${SRC}" "${HEADER}")

target_include_directories(ryuanimationlib PUBLIC "${RyuPrototype_SOURCE_DIR}/include")
