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
    Magenta
};

class Game;

class Gem
{
    friend class Game;
protected:
    GemColor color;
    sf::RectangleShape shape;

public:
    Gem(GemColor color, sf::Vector2f s);
    virtual ~Gem() = default;

    

    virtual void setPosition(float x, float y);
};