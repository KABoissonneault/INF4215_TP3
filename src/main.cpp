#include "Game.h"

#include <iostream>
#include <exception>

#include <vector>

using namespace INF4215_TP3;

int main(int argc, char* argv[])
{
    std::vector<std::string> aCmdArgs;
    for(int i = 0; i < argc; ++i)
    {
        aCmdArgs.push_back(argv[i]);
    }

    try
    {
        Game& game = Game::Instance();

        game.Initialize(aCmdArgs);
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
