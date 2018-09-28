#include "gegner.h"
#include <random>


Gegner::Gegner()
{
    this->rect = QRectF(rand()% 401, 31, rand()%15+10, rand()%15+10);
    this->dy = rand()%4 +2;
    this->dx = 0;

    this->color = QColor(0,0, 0,255);
}
//☻

void Gegner::move(){
    rect.translate(dx, dy);
}

void Gegner::paint(QPainter* painter){
    painter->fillRect(rect, color);
}
