#ifndef SIDE_HPP
#define SIDE_HPP

#include <vector>
#include "Color.hpp"

class Side {
    std::vector<Color> colors; // 9 stickers per side
    public:
        explicit Side(Color color);
        ~Side();
        std::vector<Color> getColors() const;
        Color getColor(int index) const;
};

#endif


