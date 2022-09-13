#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <cassert>

namespace ryu
{

namespace Textures
{
    enum class ID {
        IchiRun,
    };
}

// generalized approach to hold all kinds of assets
template <typename Resource, typename Identifier>
class AssetManager
{
    public:
        void load(Identifier id, const std::string& filename);
        template <typename Parameter>
        void load(Identifier id, const std::string& filename, const Parameter& secondParam);
        Resource& getResource(Identifier id);
        const Resource& getResource(Identifier id) const;

    private:
        std::map<Identifier, std::unique_ptr<Resource> > mResourceMap;
};
} /// namespoace ryu
#include "AssetManager.inl"