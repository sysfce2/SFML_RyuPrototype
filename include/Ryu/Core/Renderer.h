#pragma once

#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Events/PhysicsEvents.h>
#include <SFML/Graphics.hpp>
#include <unordered_map>

using PhysicsAssetsManager = AssetManager<sf::Texture, Textures::PhysicAssetsID>;
using SceneAssetsManager = AssetManager<sf::Texture, Textures::SceneID>;
using CharacterAssetsManager = AssetManager<sf::Texture, Textures::SpritesheetID>;

class Renderer {
public:
    
    Renderer(sf::RenderWindow& window);
    
    void draw();

private:
    struct RenderObject {

        RenderObject(b2BodyId id, std::unique_ptr<sf::Shape> s, auto texId)
            : bodyId(id), shape(std::move(s)), textureId(texId) {}
        
        b2BodyId bodyId;
        std::unique_ptr<sf::Shape> shape;
        std::variant<Textures::PhysicAssetsID, Textures::SceneID, Textures::SpritesheetID> textureId;
    };

    void loadTextures();

    // Physics assets - section
    PhysicsAssetsManager mPhysicsAssetsManager;
    sf::RenderWindow& mWindow;
    std::unordered_map<std::string, RenderObject> mRenderObjects;

    void onPhysicsObjectCreated(const PhysicsObjectCreatedEvent& event);

    void onPhysicsObjectUpdated(const PhysicsObjectUpdatedEvent& event);

    void onPhysicsObjectDestroyed(const PhysicsObjectDestroyedEvent& event);
    // Physics assets - section - end

    // Scene assets
    SceneAssetsManager mSceneAssetsManager;
    // Scene assets - end
    
    // Character assets
    CharacterAssetsManager mCharacterAssetsManager;
    // Character assets - end
};
