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

    std::cout << "Bienvenue a notre TP3 de INF4215!" << std::endl;
    std::cout << "Cette console affichera des informations importantes sur le deroulement de la partie" << std::endl;

    try
    {
        Game& game = Game::Instance();

        game.Initialize(aCmdArgs);
        game.Run();
    }
    catch(Game::InitializeException& e)
    {
        std::cout << "Erreur dans l'initialization du jeu:" << std::endl;
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
