
set (SRC
	Core/Game.cpp
	Character/CharacterBase.cpp
	Statemachine/CharacterStateIdle.cpp
	Statemachine/CharacterStateRun.cpp
)

build_chapter(RyuPt SOURCES ${SRC})
