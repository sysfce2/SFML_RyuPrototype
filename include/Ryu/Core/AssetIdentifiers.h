#pragma once
#include <Thirdparty/betterEnum/enum.h>

// Forward declaration of SFML classes
namespace sf
{
	class Texture;
}

//namespace ryu{

namespace Textures
{
	enum class SceneID
	{
		BoxPushable,
		BoxHeavy,
		Ladder,
		Ground,
		Grass,
		BGMountain,
		Unknown
	};

	enum class PhysicAssetsID
	{
		Empty,
		Crate,
	};

	BETTER_ENUM(LevelID, char,
		Unknown=0,
		Level1,
		Level2
	);

	// TODO: sub-spritesheetIDs in character itself ? (e.g. RUN in the states)
  // Rename ? to CharacterAnimationId
    BETTER_ENUM(CharacterID, char ,
        None=0,
				IchiIdleRun,
				IchiJump,
				IchiKatanaWalk,
				IchiFallingLow,
				IchiEndFallingLow,
				IchiEndFallingHigh,
				IchiDuckEnter,
				IchiDuckIdle,
				IchiDuckWalk,
				IchiLadderIdle,
				IchiLadderClimb,
				IchiJumpUp,
				IchiJumpForward,
				IchiSword1Idle,
				IchiSword1Walk,
				IchiWalk
		// ... Spritesheets
    );

	enum class GuiID {
			 None,
			 ForwardFrame,
			 BackwardFrame,
			 Play,
			 Stop,
			 EndFrame,
			 StartFrame,
			 Frame,
			 FrameBorder,	
	};
}


// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class AssetManager;

typedef AssetManager<sf::Texture, Textures::SceneID> SceneTextureHolder;

//}
