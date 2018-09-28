#include "widget.h"
#include "QPainter"
#include "QKeyEvent"
#include "iostream"
#include "string"
#include <QRectF>
#include "bewegend.h"
#include <QMessageBox>
#include "gegner.h"
#include <vector>
#include <QPushButton>
#include <QGridLayout>
#include <QTextStream>
#include <QFileDialog>
#include <iostream>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    player = Bewegend();


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));


    startButton = new QPushButton(this);

    startButton->setGeometry(30,420,100,60);
    startButton->setText("Start");

    connect(startButton,SIGNAL(clicked()),this,SLOT(start()));



    saveButton = new QPushButton(this);

    saveButton->setGeometry(150,420,100,60);
    saveButton->setText("Speichern");

    connect(saveButton,SIGNAL(clicked()),this,SLOT(speichern()));


    loadButton = new QPushButton(this);

    loadButton->setGeometry(270,420,100,60);
    loadButton->setText("Laden");

    connect(loadButton,SIGNAL(clicked()),this,SLOT(laden()));


    loadButton->setFocusPolicy(Qt::NoFocus);
    saveButton->setFocusPolicy(Qt::NoFocus);
    startButton->setFocusPolicy(Qt::NoFocus);

}

Widget::~Widget()
{

}

void Widget::speichern()
{
    if(running) start();
    QFileDialog dialog(this);
    QString fileName;
    QFile file;

    dialog.setFileMode(QFileDialog::AnyFile);
    fileName = dialog.getSaveFileName(this, tr("Speichern als"), ".", tr("Positionen (*.myz)"));

    if (fileName.isNull()==false)
    {
        file.setFileName(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Dateifehler"),
                                 tr("Folgende Datei kann nicht verwendet werden: ") + fileName,QMessageBox::Ok);
        }
        this->serialize(file);
        file.close();
       if (!running) start();

        return;
    }
}


void Widget::laden(){

      if(running) start();
    QFileDialog dialog(this);
    QString fileName;
    QFile file;

    dialog.setFileMode(QFileDialog::AnyFile);
    fileName = dialog.getOpenFileName(this,
                                      tr("Speichern als"), ".", tr("Position (*.myz)"));

    if (fileName.isNull()==false)
    {
        file.setFileName(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Dateifehler"),
                                 tr("Folgende Datei kann nicht geöffnet werden: ") + fileName,QMessageBox::Ok);
        }

        this->deserialize(file);
        file.close();



        return;
    }
}

void Widget::serialize(QFile &file){
    QTextStream out(&file);

    out << player.rect.x() << "-" << Leben << "-" << points << "\n";

    while(gegnerListe.size() < 3){
        erzeugeGegner();
    }

    Gegner* gegner1 = gegnerListe.at(0);
    Gegner* gegner2 = gegnerListe.at(1);
    Gegner* gegner3 = gegnerListe.at(2);


    out << gegner1->rect.x() << "-" << gegner1->rect.y() << "-" << gegner1->dy << "-" << gegner1->rect.width() << "-" <<  gegner1->rect.height() << "\n";
    out << gegner2->rect.x() << "-" << gegner2->rect.y()<< "-" << gegner2->dy << "-" << gegner2->rect.width() << "-" <<  gegner2->rect.height() << "\n";
    out << gegner3->rect.x() << "-" << gegner3->rect.y() << "-" << gegner3->dy << "-" << gegner3->rect.width() << "-" <<  gegner3->rect.height() << "\n";
}

void Widget::deserialize(QFile &file){
     gegnerListe.erase(gegnerListe.begin(), gegnerListe.end());

    QTextStream in(&file);


        QString line = in.readLine();
        QStringList PLP = line.split("-");
        QString x = PLP.at(0);
        player = Bewegend();

        player.rect.setX(x.toFloat());
        QString l = PLP.at(1);
        Leben = l.toInt();
        QString p = PLP.at(2);
        points = p.toInt();


        for(int i =0; i < 3; i++){
            line = in.readLine();
            QStringList gegnerString = line.split("-");
            Gegner* g = new Gegner();

            QString x = gegnerString.at(0);
            g->rect.setX(x.toFloat());
            QString y = gegnerString.at(1);
            g->rect.setY(y.toFloat());
            QString dy = gegnerString.at(2);
            g->dy = dy.toInt();
            QString w = gegnerString.at(3);
            g->rect.setWidth(w.toFloat());
            QString h = gegnerString.at(4);
            g->rect.setHeight(h.toFloat());

            gegnerListe.push_back(g);
        }



}



void Widget::paintEvent(QPaintEvent *event){

    QPainter* painter = new QPainter(this);
    painter->begin(this);

//Spieler
    player.paint(painter);
    player.move();

//Punktestand
    QString counter("Points: ");
    counter.append(QString::number(points));
    painter->drawText(30,30, counter);
    points +=1;

//Leben
    paintLeben(painter);

//Gegner
    gegnerMalen(painter);





//Game Over
    if (Leben < 0 && running){


        QString text = "GAME OVER! Points: ";
        text.append(QString::number(points));
        stop();
        QMessageBox gameOver;
        gameOver.setText(text);
        gameOver.exec();

        stop();

    }

    painter->end();
}

void Widget::gegnerMalen(QPainter* painter){


    for(int i =0; i < gegnerListe.size();i++){
        Gegner* gegner = gegnerListe.at(i);
        gegner->paint(painter);
        gegner->move();

        if(player.rect.intersects(gegner->rect)){
            gegnerListe.erase(gegnerListe.begin()+i);
            erzeugeGegner();
            Leben--;

       };

        if(gegner->rect.y() > 400){
            gegnerListe.erase(gegnerListe.begin()+i);
            erzeugeGegner();
        }

    }

    //Kollision


}

void Widget::paintLeben(QPainter* painter){

    int xPosition = 330;
    int yPosition = 20;
    for (int i = 0;i < Leben; i++){
        painter->setBrush(Qt::red);
        painter->drawEllipse(xPosition, yPosition, 10, 10);

        xPosition += 20;
    }
}

void Widget::erzeugeGegner(){
    Gegner* g = new Gegner();
    gegnerListe.push_back(g);
}

void Widget::keyPressEvent(QKeyEvent* event){

    switch(event->key()){
        case Qt::Key_Right:
            player.dx = 10;
        break;
        case Qt::Key_Left:
            player.dx = -10;
        break;
break;
    }
}

void Widget::keyReleaseEvent(QKeyEvent* event){
    switch(event->key()){
        case Qt::Key_Right:
            player.dx = 0;
        break;
        case Qt::Key_Left:
            player.dx = 0;
        break;
    }
}


