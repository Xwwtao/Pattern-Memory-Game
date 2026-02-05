#include "Model_Cell.h"

//Cell
Cell::Cell(double x, double y, double size, int index)
    : x(x), y(y), size(size), index(index) {}

bool Cell::contains(double mx, double my) const
{
    return mx >= x && mx <= x + size &&
           my >= y && my <= y + size;
}

int Cell::get_index() const
{
    return index;
}

//saquare cell
SquareCell::SquareCell(double x, double y, double size, int index)
    : Cell(x, y, size, index) {}

void SquareCell::draw() const
{
    draw_rectangle(COLOR_BLACK, x, y, size, size);
}

void SquareCell::draw_highlight(color c) const
{
    fill_rectangle(c, x + 4, y + 4, size - 8, size - 8);
}

//square cell has 10 scores
int SquareCell::score_value() const
{
    return 10;
}

//circle cell
CircleCell::CircleCell(double x, double y, double size, int index)
    : Cell(x, y, size, index) {}

void CircleCell::draw() const
{
    draw_rectangle(COLOR_BLACK, x, y, size, size);
}

void CircleCell::draw_highlight(color c) const
{
    double radius = size / 2;
    double centerX = x + radius;
    double centerY = y + radius;
    fill_circle(c, centerX, centerY, radius - 3);
}


//circle cell has 20 scores
int CircleCell::score_value() const
{
    return 20;
}

//detect mouse in circle or not
bool CircleCell::contains(double mx, double my) const
{
    double r = size / 2.0;
    double cx = x + r;
    double cy = y + r;
    double dx = mx - cx;
    double dy = my - cy;
    return dx * dx + dy * dy <= r * r;
}
