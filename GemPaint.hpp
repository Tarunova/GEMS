#pragma once
#include "Gem.hpp"

class GemPaint : public Gem
{
private:
    static int constexpr Ncand = 16;
    static int constexpr candidates[16][2] = {
    {-2, 0}, {-2, 1}, {-1, 2}, {0, 2}, {1, 2}, {2, 1}, {2, 0}, {2, -1},
    {1, -2}, {0, -2}, {-1, -2}, {-2, -1}, {-2, -2}, {-1, -1}, {1, 1}, {1, -1}
    };
    
public:
    GemPaint(GemColor color, sf::Vector2f s);
    void onMatched(std::vector<std::vector<std::shared_ptr<Gem>>>& grid, int row, int col) override;
};