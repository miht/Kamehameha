#include "color.h"

Color::Color()
{
    r = g = b = 0.0;
}

Color::Color(int r, int g, int b) :
    r(r), g(g), b(b)
{

}

QColor Color::asQColor() {
    return QColor(r, g, b);
}
