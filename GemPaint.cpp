#include "GemPaint.hpp"
#include <algorithm>
#include <random>
#include <set>

GemPaint::GemPaint(GemColor color, sf::Vector2f s) : Gem(color, s)
{
    shape.setOutlineThickness(-18.f);
    shape.setOutlineColor(sf::Color::White);
}

void GemPaint::onMatched(std::vector<std::vector<std::shared_ptr<Gem>>> &grid, int row, int col)
{
    Gem::onMatched(grid, row, col);

    int rows = int(grid.size());
    int cols = int(grid[0].size());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distRow(0, rows - 1);
    std::uniform_int_distribution<> distCol(0, cols - 1);

    int counter = 100;
    int painted = 0;
    while (painted < 2 && counter)
    {
        counter--;
        int r = distRow(gen);
        int c = distCol(gen);

        if (grid[r][c] && !Gem::areNeighbors({r,c}, {row, col}) && (r!=row || c!=col))
        {
            grid[r][c] = std::make_shared<Gem>(color, shape.getSize());
            painted++;
        }
    }
}