#pragma once

#include <string>

class b2BodyType;
class EntityType;


struct LevelObject
{
    LevelObject(int x, int y, int size_x, int size_y, std::string name,
                b2BodyType type
                , Textures::SceneID texture = Textures::SceneID::Unknown
                , EntityType entityType = EntityType::None)
    :  posX(x)
        , posY(y)
        , sizeX(size_x)
        , sizeY(size_y)
        , name(name)
        , type(type)
        , texture(texture)
        , entityType(entityType)
    {}

    int posX;
    int posY;
    int sizeX;
    int sizeY;
    std::string name;
    b2BodyType type;
    Textures::SceneID texture;
    EntityType entityType;
};
