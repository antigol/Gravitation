#ifndef CORPUS_H
#define CORPUS_H

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QVector2D>

class Corpus
{
public:
    Corpus(QGraphicsScene *s, const QPointF &pos);
    ~Corpus();
    void setRadius(double r);
    void setSpeed(const QVector2D &s);
    void setAcceleration(const QVector2D &a);

    QPointF pos() const;
    const QVector2D &speed();
    double mass();

    QVector2D calculateForce(const Corpus *other);
    QVector2D calculateAcceleration(const QVector2D &force);
    void move(double time);

private:
    QGraphicsScene *_scene;

    QGraphicsEllipseItem *_ellipseItem;
    QGraphicsLineItem *_speedLineItem;
    QGraphicsLineItem *_accelLineItem;
    double _mass;
    QVector2D _speed;
    QVector2D _accel;

    QGraphicsPathItem *_pathItem;
    QPainterPath _path;
};


#endif // CORPUS_H
