
add_subdirectory("Animation")
add_subdirectory("Character")
add_subdirectory("Control")
add_subdirectory("Core")
add_subdirectory("Debug")
add_subdirectory("Events")
add_subdirectory("Physics")
add_subdirectory("RyuSuite")
add_subdirectory("Scene")
add_subdirectory("Statemachine")

#[[
set (SRC
x	Animation/SpritesheetAnimation.cpp
x	Animation/JsonParser.cpp
x	Debug/b2DrawSFML.cpp
x	Scene/Box.cpp
x	Scene/Entity.cpp
x	Scene/SceneNode.cpp
x	Scene/Crate.cpp
x	Control/PlayerController.cpp
x	Core/Command.cpp
x	Core/CommandQueue.cpp
x	Core/Game.cpp
x	Core/SpriteNode.cpp
x Core/Utilities.cpp
x	Core/World.cpp
x	Character/CharacterIchi.cpp
x	Character/CharacterBase.cpp
x	Events/Subject.cpp
	Physics/DebugDraw.cpp
x	Statemachine/CharacterStateClimb.cpp
x	Statemachine/CharacterStateIdle.cpp
x	Statemachine/CharacterStateDuckEnter.cpp
x	Statemachine/CharacterStateDuckIdle.cpp
x	Statemachine/CharacterStateDuckWalk.cpp
x	Statemachine/CharacterStateRun.cpp
x	Statemachine/CharacterStateJumpUp.cpp
x	Statemachine/CharacterStateFalling.cpp
x	Statemachine/CharacterStateFallingEnd.cpp
	RyuSuite/RAnimator.cpp
)

build_chapter(RyuPt SOURCES ${SRC})
#]]
