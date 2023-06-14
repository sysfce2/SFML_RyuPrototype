set (SRC 
      "Box.cpp"
      "Crate.cpp"
      "Entity.cpp"
      "SceneNode.cpp"
    )

set(HEADER
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Scene/Box.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Scene/Crate.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Scene/Entity.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Scene/SceneNode.h"
)

add_library(ryuscenelib SHARED "${SRC}" "${HEADER}")

target_include_directories(ryuscenelib PUBLIC 
    "${RyuPrototype_SOURCE_DIR}/include"
)
