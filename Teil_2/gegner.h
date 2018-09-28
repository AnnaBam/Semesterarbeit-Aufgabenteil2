#ifndef GEGNER_H
#define GEGNER_H

#include <QRectF>
#include <QColor>
#include <QPainter>


class Gegner
{
public:
    Gegner();
    QRectF rect;
    int dx, dy;
    QColor color;

    void move();
    void paint(QPainter* painter);

};

#endif // GEGNER_H
