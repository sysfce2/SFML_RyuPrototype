#include "Ryu/Core/Renderer.h"
#include "Ryu/Core/AssetIdentifiers.h"
#include "Ryu/Events/EventEnums.h"
#include "Ryu/Events/EventBus.h"
#include "Ryu/Core/Utilities.h"
#include "Ryu/Scene/Entity.h"
#include "Ryu/Scene/SceneEnums.h"
#include "Ryu/Core/EventManager.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

Renderer::Renderer(sf::RenderWindow& window, EventManager& eventManager, const std::map<ELevel, std::vector<SceneObjectPhysicsParameters>>& sceneObjects)
     : mPhysicsAssetsManager()
     , mWindow(window)
     , mSceneAssetsManager()
     , mCharacterAssetsManager()
     , mSceneGraph()
     , mWorldBounds(
          {0.f, 0.f},
          {window.getDefaultView().getSize().x, 1200})
     , mSpawnPosition({window.getDefaultView().getSize().x / 2.f,
              (mWorldBounds.size.y - window.getDefaultView().getSize().y)})
     , mPushBox(nullptr)
     , mActiveCommands()
     , mSceneObjects(sceneObjects)
     , mEventManager(eventManager)
 {
     // TODO: add assets to assetmanager like
     // baseTextureManager.load(Textures::PhysicAssetsID::Empty, "assets/scenes/99_dummy/box_empty.png");
     loadTextures();
     buildScene();
     createSceneFromConfiguration(ELevel::Level2);

     // Subscribe to physics events
     EventBus::subscribe(Ryu::EPhysicsEvent::ObjectCreated
                         , [this](std::any data)
                         {
                             auto event = std::any_cast<PhysicsObjectCreatedEvent>(data);
                             this->onPhysicsObjectCreated(event);
                         });

     EventBus::subscribe(Ryu::EPhysicsEvent::ObjectUpdated
                         , [this](std::any data)
                         {
                             auto event = std::any_cast<PhysicsObjectUpdatedEvent>(data);
                             this->onPhysicsObjectUpdated(event);
                         });


     EventBus::subscribe(Ryu::EPhysicsEvent::ObjectDestroyed
         , [this](std::any data)
         {
             auto event = std::any_cast<PhysicsObjectDestroyedEvent>(data);
             this->onPhysicsObjectDestroyed(event);
         });

}

void Renderer::loadTextures()
{
    mPhysicsAssetsManager.load(Textures::PhysicAssetsID::Crate,
                            "assets/scenes/99_dummy/box_wood.png");
    
    mSceneAssetsManager.load(Textures::SceneID::BoxPushable,
                        "assets/scenes/99_dummy/box_wood.png");
    mSceneAssetsManager.load(Textures::SceneID::BGMountain,
                        "assets/backgrounds/99_dummy/722756.png");
    mSceneAssetsManager.load(Textures::SceneID::Grass,
                        "assets/scenes/99_dummy/tile_grass_1.png");
    mSceneAssetsManager.load(Textures::SceneID::Button,
                        "assets/scenes/99_dummy/tile_button_1.png");
    mSceneAssetsManager.load(Textures::SceneID::Teleport,
                        "assets/scenes/99_dummy/tile_teleport_1.png");
    mSceneAssetsManager.load(Textures::SceneID::Grate,
                        "assets/scenes/99_dummy/tile_grate_1.png");

}

void Renderer::buildScene() {
    // set Layer
    for (std::size_t i = 0; i < size_t(Layer::LayerCount); ++i) {
        std::shared_ptr<SceneNode> layer = std::make_shared<SceneNode>();
        mSceneLayers[i] = layer;
        mSceneGraph.attachChild(std::move(layer));
    }
    
    sf::Texture &textureBg =
        mSceneAssetsManager.getResource(Textures::SceneID::BGMountain);
    sf::IntRect textureRect(sf::Vector2i(0, 0), sf::Vector2i(static_cast<int>(mWorldBounds.size.x), static_cast<int>(mWorldBounds.size.y)));

    std::unique_ptr<SpriteNode> backgroundSprite =
        std::make_unique<SpriteNode>(textureBg, textureRect);
    backgroundSprite->setPosition({mWorldBounds.position.x, mWorldBounds.position.y});
    mSceneLayers[static_cast<unsigned>(Layer::Background)]->attachChild(
        std::move(backgroundSprite));

    // pushable Box / moving platform test
    std::unique_ptr<Box> box =
        std::make_unique<Box>(Box::Type::Pushable, mSceneAssetsManager);
    mPushBox = box.get();
    mPushBox->setPosition(sf::Vector2f(760.f,80.f));
    
    mSceneLayers[static_cast<unsigned>(Layer::Foreground)]->attachChild(
        std::move(box));

    auto player = mEventManager.requestPlayer();
}

void Renderer::draw()
{
    mWindow.draw(mSceneGraph);
}

void Renderer::update(sf::Time dt)
{
    while (!mActiveCommands.isEmpty()) {
        mSceneGraph.onCommand(mActiveCommands.pop(), dt);
    }
    mSceneGraph.update(dt);
}

CommandQueue& Renderer::getActiveCommands()
{
    return mActiveCommands;
}


void
Renderer::onPhysicsObjectCreated(const PhysicsObjectCreatedEvent& event)
{
    // Create a SpriteNode for the physics object
    sf::Texture* texture = nullptr;
    
    if (auto* physicsTextureId = std::get_if<Textures::PhysicAssetsID>(&event.textureId))
    {
        if (*physicsTextureId != Textures::PhysicAssetsID::Empty)
        {
            texture = &mPhysicsAssetsManager.getResource(*physicsTextureId);
        }
        else
        {
            texture = &mPhysicsAssetsManager.getResource(Textures::PhysicAssetsID::Empty);
        }
    }
    else if (auto* sceneTextureId = std::get_if<Textures::SceneID>(&event.textureId))
    {
        texture = &mSceneAssetsManager.getResource(*sceneTextureId);
    }
    else if(auto* spriteSheetTextureId = std::get_if<Textures::SpritesheetID>(&event.textureId))
    {
        texture = &mCharacterAssetsManager.getResource(*spriteSheetTextureId);
    }

    if (texture)
    {
        auto spriteNode = std::make_unique<SpriteNode>(*texture);
        // Set origin to center
        sf::Vector2f textureSize(static_cast<float>(texture->getSize().x), 
                                static_cast<float>(texture->getSize().y));
        spriteNode->setOrigin(textureSize / 2.0f);
        
        // Convert position from meters (Box2D) to pixels (SFML)
        spriteNode->setPosition(sf::Vector2f{
            Converter::metersToPixels(event.position.x),
            Converter::metersToPixels(event.position.y)
        });
        
        // Attach to the appropriate layer
        mSceneLayers[static_cast<unsigned>(Layer::Foreground)]->attachChild(std::move(spriteNode));
    }
}

void
Renderer::onPhysicsObjectUpdated(const PhysicsObjectUpdatedEvent& event)
{
    // Note: With the scenegraph approach, we don't need to manually update positions
    // as the physics system should be updating the SceneNodes directly.
    // This is a placeholder for any additional update logic if needed.
}

void
Renderer::onPhysicsObjectDestroyed(const PhysicsObjectDestroyedEvent& event)
{
    // Note: With the scenegraph approach, we don't need to manually remove objects
    // as they should be managed by the scenegraph.
    // This is a placeholder for any cleanup logic if needed.
}

void Renderer::createSceneFromConfiguration(ELevel level)
{
    for (auto& obj : mSceneObjects.at(level))
    {
        auto spriteNode = std::make_unique<SpriteNode>(
            mSceneAssetsManager.getResource(obj.mTextureId));
        
        // Get the texture size to set the origin to the center
        const sf::Texture& texture = mSceneAssetsManager.getResource(obj.mTextureId);
        sf::Vector2f textureSize(static_cast<float>(texture.getSize().x), 
                                static_cast<float>(texture.getSize().y));
        spriteNode->setOrigin(textureSize / 2.0f);
        
        // Convert from meters (Box2D) to pixels (SFML)
        spriteNode->setPosition(sf::Vector2f{
            Converter::metersToPixels(obj.mPosition.x),
            Converter::metersToPixels(obj.mPosition.y)
        });
        
        // Assign to layer based on object type
        if (obj.mType == b2_dynamicBody)
        {
            mSceneLayers[static_cast<size_t>(Layer::Foreground)]->attachChild(std::move(spriteNode));
        }
        else
        {
            mSceneLayers[static_cast<size_t>(Layer::Ground1)]->attachChild(std::move(spriteNode));
        }
    }
}
