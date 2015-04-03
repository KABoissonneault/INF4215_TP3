#include "Game.h"

#include <iostream>
#include <exception>

using namespace INF4215_TP3;

int main()
{
    try
    {
        Game& game = Game::Instance();

        game.Run();
    }
    catch(Game::InitializeException& e)
    {
        std::cout << "Error in Game initialization:" << std::endl;
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
