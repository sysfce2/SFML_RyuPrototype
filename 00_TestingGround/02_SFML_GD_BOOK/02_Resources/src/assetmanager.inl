
template <typename Resource, typename Identifier>
Resource&
ResourceHolder<Resource, Identifier>::getResource(Identifier id)
{
    auto resourceFound = mResourceMap.find(id);
    assert(resourceFound != mResourceMap.end());
    return *(resourceFound->second);
}

template <typename Resource, typename Identifier>
const Resource& 
ResourceHolder<Resource, Identifier>::getResource(Identifier id) const
{
    auto resourceFound = mResourceMap.find(id);
    assert(resourceFound != mResourceMap.end());
    return *(resourceFound->second); 
}

template <typename Resource, typename Identifier>
void 
ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
    auto resource = std::make_unique<Resource>();
    if(!resource->loadFromFile(filename))
    {
        throw std::runtime_error("ResourceHolder:load - Failed to load " + filename);
    }

    // give ownership to TextureMap
    auto inserted = mResourceMap.insert(std::make_pair(id,std::move(resource)));
    assert(inserted.second);
}