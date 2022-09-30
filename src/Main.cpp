#include <Ryu/Core/Game.h>
#include <iostream>

int main()
{
	try
	{
		Game game;
		game.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << "\nEXCEPTION: " << e.what() << '\n';
	}
	
	
	return 0;
}