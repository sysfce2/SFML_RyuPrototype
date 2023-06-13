set (SRC 
      "b2DrawSFML.cpp"
    )

set(HEADER
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Debug/b2DrawSFML.hpp"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Debug/imGuiDebug.h"
)

add_library(ryudebuglib SHARED "${SRC}" "${HEADER}")

target_include_directories(ryudebuglib PUBLIC "${RyuPrototype_SOURCE_DIR}/include")
