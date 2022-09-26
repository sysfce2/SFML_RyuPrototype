#pragma once

#include <SFML/Graphics.hpp>
#include "scenenode.h"
#include "assetmanager.h"
#include "aircarft.h"
#include "commandqueue.h"
#include <array>

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;

class World : private sf::NonCopyable
{

    public: 
        explicit World(sf::RenderWindow& window);
        void update(sf::Time dt);
        void draw();
        CommandQueue& getActiveCommands();

    private:
        enum class Layer
        {
            Background,
            Air,
            LayerCount
        };

        void loadTextures();
        void buildScene();

    private:
        sf::RenderWindow& mWindow;
        sf::View mWorldView;
        TextureHolder mTextures;
        SceneNode mSceneGraph;
        std::array<SceneNode*, std::size_t(Layer::LayerCount)> mSceneLayers;

        sf::FloatRect mWorldBounds;
        sf::Vector2f mSpawnPosition;
        float mScrollSpeed;
        Aircraft* mPlayerAircraft;

        CommandQueue mActiveCommands;

};