#include "command.h"
#include "category.h"

Command::Command()
: category(static_cast<unsigned>(Category::Type::Player))
{}