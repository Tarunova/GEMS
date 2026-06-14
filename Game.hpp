#pragma once

#include "Gem.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Game
{
private:
    static int constexpr delayMS = 128;
    static int constexpr boardDimension = 10;
    static int constexpr cellSize = 64;
    static float constexpr specialGemProbability = 0.05f;

    std::vector<std::vector<std::shared_ptr<Gem>>> gameBoard;
    sf::RenderWindow gameWindow;

    sf::Vector2i selectedCell = sf::Vector2i(-1, -1);
    bool lock = false;

    void initializeBoard();
    void renderBoard();
    void processEvents();
    void gameUpdate();

    void performSwap(sf::Vector2i first, sf::Vector2i second);
    bool findAndRemoveMatches();
    void dropGems();
    void replenishBoard();
    GemColor generateRandomColor() const;

public:
    Game();
    void startGameLoop();
};