#ifndef WIDGET_H
#define WIDGET_H

#include <QGraphicsView>

class Widget : public QGraphicsView
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    void keyPressEvent(QKeyEvent *event);
};

#endif // WIDGET_H
