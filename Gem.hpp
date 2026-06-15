#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

enum class GemColor
{
    Red,
    Yellow,
    Green,
    Cyan,
    Blue,
    Magenta,
    Orange,
    Violet
};

class Game;

class Gem
{
    friend class Game;

private:
    static float constexpr bonusGemProbability = 0.05f;

protected:
    GemColor color;
    sf::RectangleShape shape;

public:
    Gem(GemColor color, sf::Vector2f s);
    virtual ~Gem() = default;

    
    static bool areNeighbors(sf::Vector2i first, sf::Vector2i second);
    virtual void setPosition(float x, float y);
    virtual void onMatched(std::vector<std::vector<std::shared_ptr<Gem>>>& grid, int row, int col);
};