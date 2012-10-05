#include "widget.h"
#include <QKeyEvent>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    scene = new Scene(this);
    scene->setSceneRect(-100000, -100000, 200000, 200000);

    view = new QGraphicsView(scene, this);
    view->setMouseTracking(true);

    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(view);

    //    setWindowState(windowState() | Qt::WindowFullScreen);
}

Widget::~Widget()
{

}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        event->accept();
        view->scale(1.1, 1.1);
        break;
    case Qt::Key_Minus:
        event->accept();
        view->scale(0.9, 0.9);
        break;
    }

    QWidget::keyPressEvent(event);
}
