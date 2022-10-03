#pragma once

enum class EInput 
{
    NONE,
    PRESSUP,
    PRESSDOWN,
    PRESSRIGHT,
    PRESSLEFT,
    RELEASEUP,
    RELEASEDOWN,
    RELEASERIGHT,
    RELEASELEFT,
};

enum class ECharacterState{
    None,
    Idle,
    Walk,
    Run,
    //...
};

enum class EMoveDirecton
{
    None,
    Up,
    Down,
    Left,
    Right,
    UpLeft,
    UpRight,
    DownLeft,
    DownRight
};
