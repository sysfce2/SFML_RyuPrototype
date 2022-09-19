#include "game.h"
#include <iostream>
#include "assetmanager.h"

int main()
{
	try
	{
		Game game;
		game.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exceptions: " << e.what() << '\n';
	}
	
	return 0;
}
