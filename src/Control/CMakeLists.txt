set (SRC 
      "PlayerController.cpp"
    )

set(HEADER
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Control/CharacterEnums.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Control/PlayerController.h"
)

add_library(ryucontrollib SHARED "${SRC}" "${HEADER}")

target_include_directories(ryucontrollib PUBLIC "${RyuPrototype_SOURCE_DIR}/include")
