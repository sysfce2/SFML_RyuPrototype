#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Command;

//namespace ryu {

class SceneNode : public sf::Drawable, 
                public sf::Transformable, 
                private sf::NonCopyable
{
    public:
        typedef std::unique_ptr<SceneNode> Ptr;

    public:
        SceneNode();
        virtual ~SceneNode();

        void attachChild(Ptr child);
        Ptr detachChild(const SceneNode& node);
        void update(sf::Time dt);
        virtual unsigned int getCategory() const;
        sf::Transform getWorldTransform() const;
        sf::Vector2f getWorldPosition() const;
        void onCommand(const Command& command, sf::Time dt);
    
    protected:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const /*TODO: for a test : remove comment  !final*/;
        void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
        
        virtual void updateCurrent(sf::Time dt);
        void updateChildren(sf::Time dt);

    private:
        std::vector<Ptr> mChildren;
        SceneNode* mParent;
};

//} /// namespace ryu