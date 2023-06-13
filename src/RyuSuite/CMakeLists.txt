set (SRC 
     "RAnimator.cpp"
    )

set(HEADER
     "${RyuPrototype_SOURCE_DIR}/include/RyuSuite/RAnimator.h"
)

add_library(ryusuitelib SHARED "${SRC}" "${HEADER}")

target_include_directories(ryusuitelib PUBLIC "${RyuPrototype_SOURCE_DIR}/include")
