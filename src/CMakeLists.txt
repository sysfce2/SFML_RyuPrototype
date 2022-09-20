
set (SRC
	Core/Game.cpp
	Core/SpriteNode.cpp
	Core/World.cpp
	Character/CharacterBase.cpp
	Scene/Box.cpp
	Scene/Entity.cpp
	Scene/SceneNode.cpp
	Statemachine/CharacterStateIdle.cpp
	Statemachine/CharacterStateRun.cpp
)

build_chapter(RyuPt SOURCES ${SRC})
