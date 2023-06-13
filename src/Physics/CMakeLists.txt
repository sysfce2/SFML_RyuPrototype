set (SRC 
     "DebugDraw.cpp"
    )

set(HEADER
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Physics/DebugDraw.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Physics/Raycast.h"
)

add_library(ryuphysicslib SHARED "${SRC}" "${HEADER}")

target_include_directories(ryuphysicslib PUBLIC "${RyuPrototype_SOURCE_DIR}/include")
