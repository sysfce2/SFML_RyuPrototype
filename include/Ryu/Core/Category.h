#pragma once
/*
*   Here we define the Receiver Categories for an object (i.e. we can apply a command to it)
*   Each category is initialized with an integer that has one bit set to 1 the rest to 0.
*   with this approach we can combine different categories with the bitwise OR-operator to one integer.
*   To determine if an object is within a combined category we use the bitwise AND-operator.
*   e.g. A = 0100 , B = 0010 -> C(ombined) = A | B = 0110. -> check if a category is inside a combined: B & C = 0010 & 0110 = 0010. 
*   BUT: integer Flags are not typesafe, meaning we can assign values from other enums (which are also integers)
*/
namespace Category
{
    enum class Type
    {
        None                = 0,
        Scene               = 1 << 0,   //0000 0001
        Player              = 1 << 1,   //0000 0010
        Ally                = 1 << 2,   //0000 0100
        Enemy               = 1 << 3,   //0000 1000
        StaticObjects       = 1 << 4,   //0001 0000
        MovableObjects      = 1 << 5,   //0010 0000
    };

} /// namespace ryu::Category
