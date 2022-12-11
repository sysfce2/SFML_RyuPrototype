#pragma once

enum class EInput 
{
    None,
    PressUp,
    PressDown,
    PressRight,
    PressLeft,
    ReleaseUp,
    ReleaseDown,
    ReleaseRight,
    ReleaseLeft,
};

enum class ECharacterState{
    None,
    Idle,
    Walk,
    Run,
    //...
};

enum class EMoveDirection
{
    None=0,
    Up,
    Down,
    Left,
    Right,
    UpLeft,
    UpRight,
    DownLeft,
    DownRight
};

enum class EClimbHeight{
    None=0,
    Low,
    Mid,
    High,  
};