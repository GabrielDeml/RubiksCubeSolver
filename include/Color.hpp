#ifndef COLOR_HPP
#define COLOR_HPP

enum class Color {
    W = 0,
    Y = 1,
    R = 2,
    O = 3,
    G = 4,
    B = 5
};

class ColorObject {
    Color value;
    public:
        explicit ColorObject(Color color);
        ~ColorObject();
        Color getColor() const;
};

#endif


