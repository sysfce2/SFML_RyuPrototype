set (SRC 
        "CharacterBase.cpp"
        "CharacterIchi.cpp"
    )

set(HEADER
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Character/CharacterBase.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Character/CharacterIchi.h"
)

add_library(ryucharacterlib SHARED "${SRC}" "${HEADER}")

target_include_directories(ryucharacterlib PUBLIC "${RyuPrototype_SOURCE_DIR}/include")
target_link_libraries(ryuptlib PUBLIC ryucharacterlib)
