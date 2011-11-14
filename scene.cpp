#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent)
{
    state = 1;
    timer = 0;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);

    switch (state) {
    case 1:
        // créer 1st obj
        list << new Object(this, event->scenePos());
        break;
    case 2:
        // vol 1st obj
        list.last()->setRadius(QVector2D(list.last()->pos() - event->scenePos()).length());
        break;
    case 3:
        // vit 1st obj
        list.last()->setSpeed(QVector2D(event->scenePos() - list.last()->pos()));

        state = 0;
        break;
    }

    state++;
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);

    switch (state) {
    case 1:
        // créer 1st obj
        break;
    case 2:
        // vol 1st obj
        list.last()->setRadius(QVector2D(list.last()->pos() - event->scenePos()).length());
        break;
    case 3:
        // vit 1st obj
        list.last()->setSpeed(QVector2D(event->scenePos() - list.last()->pos()));
        break;
    }
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    QGraphicsScene::keyPressEvent(event);

    switch (event->key()) {
    case Qt::Key_Space:
        if (timer == 0) {
            // start
            timer = startTimer(20);
            state = 100;
        } else {
            // stop
            killTimer(timer);
            timer = 0;
            state = 1;
        }
        break;
    case Qt::Key_Return:
        if (timer == 0) {
            while (!list.isEmpty())
                delete list.takeLast();
        } else {
            // stop
            killTimer(timer);
            timer = 0;
            while (!list.isEmpty())
                delete list.takeLast();
            state = 1;
        }
        break;
    }
}

void Scene::timerEvent(QTimerEvent *)
{
    for (int i = 0; i < list.size(); ++i) {
        QVector2D acceleration = QVector2D(0,0);

        for (int j = 0; j < list.size(); ++j) {
            if (i != j) {
                acceleration += list[i]->calculateAcceleration(list[j]);
            }
        }

        list[i]->setAcceleration(acceleration);
    }

    for (int i = 0; i < list.size(); ++i) {
        list[i]->move(0.03);
    }

    update();
}

Object::Object(QGraphicsScene *s, const QPointF &pos)
{
    scene = s;
    ellipseItem = scene->addEllipse(0, 0, 0, 0);
    ellipseItem->setPos(pos);
    speedLineItem = scene->addLine(0, 0, 0, 0, QPen(Qt::darkGreen));
    accelerationLineItem = scene->addLine(0, 0, 0, 0, QPen(Qt::red));
    mass = 0;
    speed = QVector2D(0, 0);
    path.moveTo(pos);
    cm = scene->addPath(path, QPen(Qt::blue));
}

Object::~Object()
{
    scene->removeItem(ellipseItem);
    scene->removeItem(speedLineItem);
    scene->removeItem(accelerationLineItem);
    scene->removeItem(cm);
}

void Object::setRadius(double r)
{
    ellipseItem->setRect(-r, -r, 2*r, 2*r);
    speedLineItem->setLine(QLineF(ellipseItem->pos(), ellipseItem->pos()));
    mass = r*r*r;
}

void Object::setSpeed(const QVector2D &s)
{
    speed = s;
    speedLineItem->setLine(QLineF(ellipseItem->pos(), ellipseItem->pos() + s.toPointF()));
}

void Object::setAcceleration(const QVector2D &a)
{
    acceleration = a;
    accelerationLineItem->setLine(QLineF(ellipseItem->pos(), ellipseItem->pos() + a.toPointF()));
}

QPointF Object::pos() const
{
    return ellipseItem->pos();
}

QVector2D Object::calculateAcceleration(const Object *o)
{
    QVector2D force = QVector2D(o->ellipseItem->pos() - ellipseItem->pos());
    double k = 4.0 * o->mass*mass / force.lengthSquared();
    force.normalize();
    force *= k;

    return force / mass;
}

void Object::move(double time)
{
    speed += acceleration * time;
    setSpeed(speed);

    ellipseItem->setPos(ellipseItem->pos() + speed.toPointF() * time);
    path.lineTo(ellipseItem->pos());
    cm->setPath(path);
}

