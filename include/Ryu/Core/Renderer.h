#pragma once

#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/CommandQueue.h>
#include <Ryu/Events/PhysicsEvents.h>
#include <Ryu/Scene/SceneNode.h>
#include <Ryu/Scene/Box.h>
#include <Ryu/Core/SpriteNode.h>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <array>

using PhysicsAssetsManager = AssetManager<sf::Texture, Textures::PhysicAssetsID>;
using SceneAssetsManager = AssetManager<sf::Texture, Textures::SceneID>;
using CharacterAssetsManager = AssetManager<sf::Texture, Textures::SpritesheetID>;

class EventManager;

class Renderer {
public:
    
    Renderer(sf::RenderWindow& window, EventManager& eventManager);
    
    void draw();
    void buildScene();
    void update(sf::Time dt);
    CommandQueue& getActiveCommands();

    // Layer management
    enum class Layer { Background, Ground1, Foreground, LayerCount };

private:
    void loadTextures();

    // Physics assets - section
    PhysicsAssetsManager mPhysicsAssetsManager;
    sf::RenderWindow& mWindow;
    
    // Scene graph and layers
    SceneNode mSceneGraph;
    std::array<std::shared_ptr<SceneNode>, static_cast<size_t>(Layer::LayerCount)> mSceneLayers;
    SceneAssetsManager mSceneAssetsManager;
    sf::FloatRect mWorldBounds;
    sf::Vector2f mSpawnPosition;
    Box* mPushBox;
    CommandQueue mActiveCommands;
    EventManager& mEventManager;

    void onPhysicsObjectCreated(const PhysicsObjectCreatedEvent& event);
    void onPhysicsObjectUpdated(const PhysicsObjectUpdatedEvent& event);
    void onPhysicsObjectDestroyed(const PhysicsObjectDestroyedEvent& event);
    // Physics assets - section - end

    // Character assets
    CharacterAssetsManager mCharacterAssetsManager;
    // Character assets - end
};
