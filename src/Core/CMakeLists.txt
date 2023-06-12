set (SRC
    "Command.cpp"
    "CommandQueue.cpp"
    "Game.cpp"
    "SpriteNode.cpp"
    "Utilities.cpp"
    "World.cpp"
)

 # can i add files per foreach from a folder?
# for_each()
  set(HEADER
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Core/AssetIdentifiers.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Core/AssetManager.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Core/AssetManager.inl"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Core/Category.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Core/Command.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Core/CommandQueue.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Core/Game.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Core/SpriteNode.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Core/Utilities.h"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Core/World.h"

  )
# endforeach()

add_library(ryucorelib SHARED "${SRC}" "${HEADER}")

target_include_directories(ryucorelib PUBLIC "${RyuPrototype_SOURCE_DIR}/include")
target_link_libraries(ryuptlib PUBLIC ryucorelib)
