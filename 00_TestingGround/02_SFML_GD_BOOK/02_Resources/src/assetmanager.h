#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <cassert>

namespace Textures
{
    enum class ID {
        Landscape,
        Airplane,
        Missile
    };
}

// generalized approach to hold all kinds of assets
template <typename Resource, typename Identifier>
class ResourceHolder
{
    public:
        void load(Identifier id, const std::string& filename);
        Resource& getResource(Identifier id);
        const Resource& getResource(Identifier id) const;

    private:
        std::map<Identifier, std::unique_ptr<Resource> > mResourceMap;
};

#include "assetmanager.inl"