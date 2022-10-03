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
		BGMountain,
		Unknown
	};

	// TODO: sub-spritesheetIDs in character itself ? (e.g. RUN in the states)
    enum class CharacterID {
        IchiIdleRun,
		IchiJump,
		IchiKatanaWalk,
		// ... Spritesheets
    };
}


// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class AssetManager;

typedef AssetManager<sf::Texture, Textures::SceneID> SceneTextureHolder;

//}