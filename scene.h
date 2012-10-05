#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QList>
#include <QTime>
#include "corpus.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    void interpretCommand(const QString &cmd);

signals:
    void mouseMoved(const QPointF &);

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent * event);

    void moveSystem(double dt);
    void fixMomentum();

    int _state;
    int _timer;
    int _lasttime;
    QTime _timeline;
    QList<Corpus *> _list;
};

#endif // SCENE_H
