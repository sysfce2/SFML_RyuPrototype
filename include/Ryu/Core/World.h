#pragma once

#include <SFML/Graphics.hpp>
#include <Ryu/Scene/SceneNode.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/CommandQueue.h>
#include <Ryu/Scene/Box.h>
#include <array>


typedef AssetManager<sf::Texture, Textures::SceneID> SceneTextureHolder;

class CharacterIchi;

//namespace ryu {
class World : private sf::NonCopyable
{

    public: 
        explicit World(sf::RenderWindow& window);
        ~World();
        void update(sf::Time dt);
        void draw();
        CommandQueue& getActiveCommands();
        CharacterIchi* getPlayer();
        const sf::Drawable& getPlayerSprite();

    private:
        enum class Layer
        {
            Background,
            Ground1,
            Foreground,
            LayerCount
        };

        void loadTextures();
        void buildScene();

    private:
        sf::RenderWindow& mWindow;
        sf::View mWorldView;
        SceneTextureHolder mSceneTextures;
        SceneNode mSceneGraph;
        std::array<SceneNode*, std::size_t(Layer::LayerCount)> mSceneLayers;

        sf::FloatRect mWorldBounds;
        sf::Vector2f mSpawnPosition;
        Box* mPushBox;
        CharacterIchi* mPlayer;

        CommandQueue mActiveCommands;

};
//} /// namespace ryu