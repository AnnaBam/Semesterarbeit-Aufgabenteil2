#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include "bewegend.h"
#include "gegner.h"
#include <vector>
#include <QPushButton>
#include <QFile>

//struct Bewegend {
//  QRectF rect;
//int dx, dy;
//};

class Widget : public QWidget
{
    Q_OBJECT
    const int FRAMERATE = 60;

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *event);
    Bewegend player;
    Gegner gegner;

    QPushButton* startButton;
    QPushButton* saveButton;
    QPushButton* loadButton;

    std::vector<Gegner*> gegnerListe;

    int points = 0;
    int Leben = 0;
    int xPosition = 300;
    int yPosition = 50;

    QTimer *timer;
    bool running = false;
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void paintLeben(QPainter* painter);
    void erzeugeGegner();
    void gegnerMalen(QPainter* painter);
    void serialize(QFile &file);
    void deserialize(QFile &file);



private slots:
    void speichern();
    void laden();
    void start(void)
    {

        if(Leben > 0){
              QWidget::setFocusPolicy(Qt::StrongFocus);
            pause();
            return;
        }
        startButton->setText("Pause");

        timer->start(1000/FRAMERATE); //Geschwindigkeit der Objekte
        Leben = 3;
        points = 0;

        erzeugeGegner();
        erzeugeGegner();
        erzeugeGegner();
        running = true;

        player = Bewegend();

        QWidget::setFocusPolicy(Qt::StrongFocus);
    }
    void stop(void)
    {
        timer->stop();
        gegnerListe.erase(gegnerListe.begin(), gegnerListe.end());
        startButton->setText("Start");
        running=false;
        points = 0;
    }


    void pause(){
        running = !running;

        if(!running){
            timer->stop();
            startButton->setText("Start");

        }
        else {

            startButton->setText("Pause");

            timer->start(1000/FRAMERATE);
        }
        return;

    }




};

#endif // WIDGET_H
