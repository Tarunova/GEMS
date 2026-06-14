#include "Gem.hpp"

Gem::Gem(GemColor color, sf::Vector2f s) : color(color)
{
    shape.setSize(s);

    switch (color)
    {
    case GemColor::Red:
        shape.setFillColor(sf::Color(207, 19, 19));
        break;
    case GemColor::Yellow:
        shape.setFillColor(sf::Color(207, 166, 19));
        break;
    case GemColor::Green:
        shape.setFillColor(sf::Color(19, 158, 3));
        break;
    case GemColor::Cyan:
        shape.setFillColor(sf::Color(19, 207, 191));
        break;
    case GemColor::Blue:
        shape.setFillColor(sf::Color(19, 50, 207));
        break;
    case GemColor::Magenta:
        shape.setFillColor(sf::Color(201, 19, 207));
        break;
    }
}

void Gem::setPosition(float x, float y)
{
    shape.setPosition(x, y);
}

bool Gem::areNeighbors(sf::Vector2i first, sf::Vector2i second)
{
    return (std::abs(first.x - second.x) + std::abs(first.y - second.y)) == 1;
}






