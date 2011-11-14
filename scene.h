#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QVector2D>
#include <QList>

class Object
{
public:
    Object(QGraphicsScene *s, const QPointF &pos);
    ~Object();
    void setRadius(double r);
    void setSpeed(const QVector2D &s);
    void setAcceleration(const QVector2D &a);
    QPointF pos() const;

    QVector2D calculateAcceleration(const Object *o);
    void move(double time);
private:
    QGraphicsScene *scene;

    QGraphicsEllipseItem *ellipseItem;
    QGraphicsLineItem *speedLineItem;
    QGraphicsLineItem *accelerationLineItem;
    double mass;
    QVector2D speed;
    QVector2D acceleration;

    QGraphicsPathItem *cm;
    QPainterPath path;
};

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *);

    int state;

    int timer;

    QList<Object *> list;
};

#endif // SCENE_H
