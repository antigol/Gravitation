#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QGraphicsView>
#include <QLineEdit>
#include "scene.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    void keyPressEvent(QKeyEvent *event);

    QGraphicsView *view;
    Scene *scene;
    QLineEdit *lineEdit;
};

#endif // WIDGET_H
