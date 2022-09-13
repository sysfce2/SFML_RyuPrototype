
using namespace ryu;

template <typename Resource, typename Identifier>
Resource&
AssetManager<Resource, Identifier>::getResource(Identifier id)
{
    auto resourceFound = mResourceMap.find(id);
    assert(resourceFound != mResourceMap.end());
    return *(resourceFound->second);
}

template <typename Resource, typename Identifier>
const Resource& 
AssetManager<Resource, Identifier>::getResource(Identifier id) const
{
    auto resourceFound = mResourceMap.find(id);
    assert(resourceFound != mResourceMap.end());
    return *(resourceFound->second); 
}

template <typename Resource, typename Identifier>
void 
AssetManager<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
    auto resource = std::make_unique<Resource>();
    if(!resource->loadFromFile(filename))
    {
        throw std::runtime_error("AssetManager:load - Failed to load " + filename);
    }

    // give ownership to ResourceMap
    auto inserted = mResourceMap.insert(std::make_pair(id,std::move(resource)));
    assert(inserted.second);
}

// esp. for the loading of shaders this overloaded method is used
template <typename Resource,typename Identifier>
template <typename Parameter>
void AssetManager<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
    auto resource = std::make_unique<Resource>();
    if(!resource->loadFromFile(filename, secondParam))
    {
        throw std::runtime_error("ResourceHolder:load - Failed to load " + filename);
    }

    // give ownership to TextureMap
    auto inserted = mResourceMap.insert(std::make_pair(id,std::move(resource)));
    assert(inserted.second);
}