#include "Gem.hpp"
#include "GemBomb.hpp"
#include "GemPaint.hpp"

#include <random>

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
    case GemColor::Orange:
        shape.setFillColor(sf::Color(255, 90, 20));
        break;
    case GemColor::Violet:
        shape.setFillColor(sf::Color(42, 0, 97));
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

void Gem::onMatched(std::vector<std::vector<std::shared_ptr<Gem>>>& grid, int row, int col)
{

    int rows = int(grid.size());
    int cols = int(grid[0].size());

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distRow(0, rows - 1);
    std::uniform_int_distribution<> distCol(0, cols - 1);

    int counter = 100;
    int made = 0;
    while (made < 1 && counter) {
        counter--;
        int r = distRow(gen);
        int c = distCol(gen);
        if (grid[r][c] && (r != row || c != col))
        {
            std::uniform_real_distribution<float> chance(0.f, 1.f);
            float randomValue = chance(gen);
            if (randomValue < bonusGemProbability)
            {
                grid[r][c] = std::make_shared<GemBomb>(grid[r][c]->color, shape.getSize());
            }
            else if (randomValue < bonusGemProbability*2.0f)
            {
                grid[r][c] = std::make_shared<GemPaint>(grid[r][c]->color, shape.getSize());
            }
            made++;
        }
    }
}





