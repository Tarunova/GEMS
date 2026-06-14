#include "Game.hpp"
#include <random>
#include <algorithm>
#include <set>
#include <iostream>
#include <chrono>
#include <thread>
#include "GemBomb.hpp"
#include "GemPaint.hpp"

Game::Game() : gameWindow(sf::VideoMode(boardDimension* cellSize, boardDimension* cellSize), "Gems")
{
    initializeBoard();
}

void Game::initializeBoard()
{
    gameBoard.resize(boardDimension, std::vector<std::shared_ptr<Gem>>(boardDimension));
    for (int row = 0; row < boardDimension; ++row)
    {
        for (int col = 0; col < boardDimension; ++col)
        {
            GemColor color = generateRandomColor();
            gameBoard[row][col] = std::make_shared<Gem>(color, sf::Vector2f(float(cellSize), float(cellSize)));
            gameBoard[row][col]->setPosition(float(col * cellSize), float(row * cellSize));
        }
    }
}

void Game::renderBoard()
{
    for (int row = 0; row < boardDimension; ++row)
    {
        for (int col = 0; col < boardDimension; ++col)
        {
            if (gameBoard[row][col])
            {
                gameBoard[row][col]->setPosition(float(col * cellSize), float(row * cellSize));
                gameWindow.draw(gameBoard[row][col]->shape);
            }
        }
    }
}


void Game::processEvents()
{
    sf::Event event;
    while (gameWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            gameWindow.close();

        if (!lock && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            int clickX = event.mouseButton.x / cellSize;
            int clickY = event.mouseButton.y / cellSize;

            if (clickX >= 0 && clickX < boardDimension && clickY >= 0 && clickY < boardDimension)
            {
                if (selectedCell.x == -1)
                {
                    selectedCell.x = clickX;
                    selectedCell.y = clickY;
                }
                else
                {
                    sf::Vector2i secondCell( clickX, clickY );
                    if (Gem::areNeighbors(selectedCell, secondCell))
                    {
                        performSwap(selectedCell, secondCell);
                        if (findAndRemoveMatches())
                        {
                            gameUpdate();
                        }
                        else
                        {
                            performSwap(selectedCell, secondCell);
                        }
                    }
                    selectedCell.x = -1;
                    selectedCell.y = -1;
                }
            }
        }
    }
}

void Game::gameUpdate()
{
    int counter = 100;
    bool matchesFound = false;
    dropGems();
    replenishBoard();
    matchesFound = findAndRemoveMatches();
    if (matchesFound)
    {
        dropGems();
        replenishBoard();
    }
    lock = matchesFound;
}

void Game::startGameLoop()
{
    while (gameWindow.isOpen())
    {
        processEvents();
        gameUpdate();

        gameWindow.clear();
        renderBoard();
        gameWindow.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMS));
    }
}

void Game::performSwap(sf::Vector2i first, sf::Vector2i second)
{
    std::swap(gameBoard[first.y][first.x], gameBoard[second.y][second.x]);
}

bool Game::findAndRemoveMatches()
{
    bool foundMatch = false;
    std::vector<std::vector<bool>> visitedCells(boardDimension, std::vector<bool>(boardDimension, false));
    std::set<std::pair<int, int>> gemsToRemove;

    auto depthFirstSearch = [&](int row, int col, GemColor targetColor, std::vector<std::pair<int, int>>& connectedGroup, auto& dfsFunc)
        {
            if (row < 0 || row >= boardDimension || col < 0 || col >= boardDimension ||
                visitedCells[row][col] || !gameBoard[row][col] ||
                gameBoard[row][col]->color != targetColor)
                return;

            visitedCells[row][col] = true;
            connectedGroup.push_back({ row, col });

            dfsFunc(row - 1, col, targetColor, connectedGroup, dfsFunc);
            dfsFunc(row + 1, col, targetColor, connectedGroup, dfsFunc);
            dfsFunc(row, col - 1, targetColor, connectedGroup, dfsFunc);
            dfsFunc(row, col + 1, targetColor, connectedGroup, dfsFunc);
        };

    for (int row = 0; row < boardDimension; ++row)
    {
        for (int col = 0; col < boardDimension; ++col)
        {
            if (!visitedCells[row][col] && gameBoard[row][col])
            {
                std::vector<std::pair<int, int>> connectedGroup;
                GemColor currentColor = gameBoard[row][col]->color;
                depthFirstSearch(row, col, currentColor, connectedGroup, depthFirstSearch);

                if (connectedGroup.size() >= 3)
                {
                    foundMatch = true;
                    for (const auto& position : connectedGroup)
                    {
                        gemsToRemove.insert(position);
                    }
                }
            }
        }
    }

    for (auto [row, col] : gemsToRemove)
    {
        if (gameBoard[row][col])
        {
            gameBoard[row][col]->onMatched(gameBoard, row, col);
            gameBoard[row][col] = nullptr;
        }
    }

    return foundMatch;
}

void Game::dropGems()
{
    for (int col = 0; col < boardDimension; ++col)
    {
        for (int row = boardDimension - 1; row >= 0; --row)
        {
            if (!gameBoard[row][col])
            {
                for (int aboveRow = row - 1; aboveRow >= 0; --aboveRow)
                {
                    if (gameBoard[aboveRow][col])
                    {
                        gameBoard[row][col] = gameBoard[aboveRow][col];
                        gameBoard[aboveRow][col] = nullptr;
                        break;
                    }
                }
            }
        }
    }
}

void Game::replenishBoard()
{
    for (int row = 0; row < boardDimension; ++row)
    {
        for (int col = 0; col < boardDimension; ++col)
        {
            if (!gameBoard[row][col])
            {
                GemColor color = generateRandomColor();
                gameBoard[row][col] = std::make_shared<Gem>(color, sf::Vector2f(float(cellSize), float(cellSize)));
                gameBoard[row][col]->setPosition(float(col * cellSize), float(row * cellSize));
            }
        }
    }
}

GemColor Game::generateRandomColor() const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, 8);
    return static_cast<GemColor>(dist(gen));
}
