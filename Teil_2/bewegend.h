#ifndef BEWEGEND_H
#define BEWEGEND_H

#include <QRectF>
#include <QPainter>

class Bewegend
{
public:
    Bewegend();

    QRectF rect;
    float dx = 0;
    float dy = 0;
    void paint(QPainter* painter);
    void move();
};

#endif // BEWEGEND_H
