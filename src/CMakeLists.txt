
set (SRC
	Animation/SpritesheetAnimation.cpp
	Scene/Box.cpp
	Scene/Entity.cpp
	Scene/SceneNode.cpp
	Control/PlayerController.cpp
	Core/Command.cpp
	Core/CommandQueue.cpp
	Core/Game.cpp
	Core/SpriteNode.cpp
	Core/World.cpp
	Character/CharacterIchi.cpp
	Character/CharacterBase.cpp
	Events/Subject.cpp
	Statemachine/CharacterStateIdle.cpp
	Statemachine/CharacterStateRun.cpp
)

build_chapter(RyuPt SOURCES ${SRC})
