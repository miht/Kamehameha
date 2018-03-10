#ifndef COLOR_H
#define COLOR_H

#include <QColor>

class Color
{
public:
    int r;
    int g;
    int b;

    Color();
    Color(int r, int g, int b);

    QColor asQColor();
};

#endif // COLOR_H
