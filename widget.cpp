#include "widget.h"
#include "scene.h"
#include <QKeyEvent>

Widget::Widget(QWidget *parent)
    : QGraphicsView(parent)
{
    setMinimumSize(400, 400);
    setMouseTracking(true);

    Scene *scene = new Scene(this);
    scene->setSceneRect(-100000, -100000, 200000, 200000);
    setScene(scene);
}

Widget::~Widget()
{

}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        event->accept();
        scale(1.1, 1.1);
        break;
    case Qt::Key_Minus:
        event->accept();
        scale(0.9, 0.9);
        break;
    }

    QGraphicsView::keyPressEvent(event);
}
