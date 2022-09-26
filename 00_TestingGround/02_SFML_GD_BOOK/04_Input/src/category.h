#pragma once
/*
*   Here we define the Category for an object, so we can apply a command to it,
*   Each category is initialized with an integer that has one bit set to 1 the rest to 0.
*   with this approach we can combine different categories with the bitwise OR-operator to one integer.
*   BUT: integer Flags are not typesafe, meaning we can assign values from other enums (which are also integers)
*/
namespace Category
{
    enum class Type
    {
        None        = 0,
        Scene       = 1 << 0,   //0000 0001
        Player      = 1 << 1,   //0000 0010
        Ally        = 1 << 2,   //0000 0100
        Enemy       = 1 << 3,   //0000 1000
        Objects     = 1 << 4,   //0001 0000
    };

} /// namespace Category
