#pragma once

#include <SFML/Graphics.hpp>
#include <Ryu/Scene/SceneNode.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/CommandQueue.h>
#include <Ryu/Debug/b2DrawSFML.hpp>
#include <Ryu/Scene/Box.h>
#include <Ryu/Scene/Crate.h>

#include <array>
#include <vector>

typedef AssetManager<sf::Texture, Textures::SceneID> SceneTextureHolder;

class CharacterIchi;
class b2World;
class b2Body;


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
        // TODO: make this static ? -> access from everywhere
        std::unique_ptr<b2World>& getPhysicsWorld(){return phWorld;};
        sf::Shape* getShapeFromPhysicsBody(b2Body* physicsBody);

        void setDebugDrawer(sf::RenderTarget& target);

    private:
        enum class Layer
        {
            Background,
            Ground1,
            Foreground,
            LayerCount
        };

        void createText(const sf::String text, sf::Text& textToShow);
        void loadTextures();
        void buildScene();
        void setPhysics();
        b2Body* createPhysicalBox(int pos_x, int pos_y, int size_x, int size_y, b2BodyType type, Textures::SceneID texture);
        
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

        // box2d physics
        std::unique_ptr<b2World> phWorld;
        std::vector<b2Body*> phGroundBodies;
        std::vector<sf::Text> texts;
        b2Body* pBoxTest;
        bool phDebugPhysics;
        float phTimeStep;
        b2DrawSFML debugDrawer;
        
        std::vector<Crate*> mCrates;

        // Clock for calculating delta time (for physics simulation)
        sf::Clock clock;

};
//} /// namespace ryu