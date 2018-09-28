#include "bewegend.h"

#include <QPainter>
#include <QColor>
#include <QBrush>

Bewegend::Bewegend()
{
    rect = QRectF(175,380,50,10);

}


void Bewegend::paint(QPainter *painter){
    painter->drawRect(rect);
    QBrush pattern(Qt::BDiagPattern);
    painter->fillRect(rect, pattern);
}

void Bewegend::move(){
    rect.translate(dx, dy);
}
