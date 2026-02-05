#pragma once
#include "splashkit.h"

//Abstract base class (polymorphism)
class Cell
{
protected:
    double x, y;
    double size;
    int index;

public:
    Cell(double x, double y, double size, int index);

    virtual ~Cell() = default;

    virtual void draw() const = 0;
    virtual void draw_highlight(color c) const = 0;

    virtual int score_value() const = 0;
    virtual bool contains (double mx, double my) const;

    int get_index() const;
};

//squarecell square shape
class SquareCell : public Cell
{
public:
    SquareCell(double x, double y, double size, int index);

    void draw() const override;
    void draw_highlight(color c) const override;
    int score_value() const override;

};

//circle cell
class CircleCell : public Cell
{
public:
    CircleCell(double x, double y, double size, int index);

    void draw() const override;
    void draw_highlight(color c) const override;
    int score_value() const override;

    //hit circle
    bool contains(double mx, double my) const override;

};
