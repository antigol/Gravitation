#include "corpus.h"

Corpus::Corpus(QGraphicsScene *s, const QPointF &pos)
{
    _scene = s;

    //    QRadialGradient gradient(-3, -3, 10);
    //        gradient.setCenter(3, 3);
    //        gradient.setFocalPoint(3, 3);
    //        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
    //        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));


    _ellipseItem = _scene->addEllipse(0, 0, 0, 0, QPen(Qt::black, 0), QBrush(QColor(50, 50, 50, 50)));
    _ellipseItem->setZValue(0);
    _ellipseItem->setPos(pos);
    _speedLineItem = _scene->addLine(0, 0, 0, 0, QPen(Qt::darkGreen));
    _speedLineItem->setZValue(1);
    _accelLineItem = _scene->addLine(0, 0, 0, 0, QPen(Qt::red));
    _accelLineItem->setZValue(1);
    _mass = 0;
    _speed = QVector2D(0, 0);
    _path.moveTo(pos);
    _pathItem = _scene->addPath(_path, QPen(QColor(50, 50, 255, 50)));
    _pathItem->setZValue(-1);
}

Corpus::~Corpus()
{
    _scene->removeItem(_ellipseItem);
    _scene->removeItem(_speedLineItem);
    _scene->removeItem(_accelLineItem);
    _scene->removeItem(_pathItem);
}

void Corpus::setRadius(double r)
{
    _ellipseItem->setRect(-r, -r, 2*r, 2*r);
    _mass = r*r*r;
}

void Corpus::setSpeed(const QVector2D &s)
{
    _speed = s;
    _speedLineItem->setLine(QLineF(QPointF(0, 0), s.toPointF()));
    _speedLineItem->setPos(pos());
}

void Corpus::setAcceleration(const QVector2D &a)
{
    _accel = a;
    _accelLineItem->setLine(QLineF(QPointF(0, 0), a.toPointF()));
    _accelLineItem->setPos(pos());
}

QVector2D Corpus::calculateForce(const Corpus *other)
{
    QVector2D force = QVector2D(other->_ellipseItem->pos() - _ellipseItem->pos());
    if (force.isNull())
        return QVector2D(0, 0);
    double k = 4.0 * other->_mass*_mass / force.lengthSquared();
    force.normalize();
    force *= k;

    return force;
}

QVector2D Corpus::calculateAcceleration(const QVector2D &force)
{
    return force / _mass;
}

void Corpus::move(double time)
{
    _speed += _accel * time;

    _ellipseItem->setPos(_ellipseItem->pos() + _speed.toPointF() * time);

    _path.lineTo(_ellipseItem->pos());
    _pathItem->setPath(_path);

    setSpeed(_speed);
    setAcceleration(_accel);
}

QPointF Corpus::pos() const
{
    return _ellipseItem->pos();
}

const QVector2D & Corpus::speed()
{
    return _speed;
}

double Corpus::mass()
{
    return _mass;
}

