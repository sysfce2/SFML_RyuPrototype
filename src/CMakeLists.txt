
set (SRC
	Animation/SpritesheetAnimation.cpp
	Debug/b2DrawSFML.cpp
	Scene/Box.cpp
	Scene/Entity.cpp
	Scene/SceneNode.cpp
	Scene/Crate.cpp
	Control/PlayerController.cpp
	Core/Command.cpp
	Core/CommandQueue.cpp
	Core/Game.cpp
	Core/SpriteNode.cpp
  Core/Utilities.cpp
	Core/World.cpp
	Character/CharacterIchi.cpp
	Character/CharacterBase.cpp
	Events/Subject.cpp
	Physics/DebugDraw.cpp
	Statemachine/CharacterStateClimb.cpp
	Statemachine/CharacterStateIdle.cpp
	Statemachine/CharacterStateDuckEnter.cpp
	Statemachine/CharacterStateDuckIdle.cpp
	Statemachine/CharacterStateDuckWalk.cpp
	Statemachine/CharacterStateRun.cpp
	Statemachine/CharacterStateFalling.cpp
	Statemachine/CharacterStateFallingEnd.cpp
)

build_chapter(RyuPt SOURCES ${SRC})
