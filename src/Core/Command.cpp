#include <Ryu/Core/Command.h>
#include <Ryu/Core/Category.h>

//namespace ryu{
    
Command::Command()
: category(static_cast<unsigned>(Category::Type::Player))
{}

//} /// namespace ryu