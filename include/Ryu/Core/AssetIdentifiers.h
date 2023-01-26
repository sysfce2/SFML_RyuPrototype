#pragma once

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

	enum class LevelID {
		Unknown,
		Level1,
		Level2,
	};

	// TODO: sub-spritesheetIDs in character itself ? (e.g. RUN in the states)
    enum class CharacterID {
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
				IchiWalk,
		// ... Spritesheets
    };
}


// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class AssetManager;

typedef AssetManager<sf::Texture, Textures::SceneID> SceneTextureHolder;

//}