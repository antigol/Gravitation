#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QTime>

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
            // set to an invalid state
            state = 100;
        } else {
            // stop
            killTimer(timer);
            timer = 0;
            // set to first state
            state = 1;
        }
        break;
    case Qt::Key_Return:
        if (timer == 0) {
            while (!list.isEmpty())
                delete list.takeLast();
            state = 1;
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

int k(int n, int i, int j)
{
    return (-i*i + i*(2*n-3) + 2*j - 2) / 2;
}

void Scene::timerEvent(QTimerEvent *)
{
    QTime t; t.start();

    int n = list.size();
    QVector<QVector2D> forces;
    // c'est comme l'histoire des poignées de mains, y en a (n²-n) sur 2
    forces.reserve((n * (n - 1)) / 2);

    /**************************************
      Exemple avec n = 5

      i j k
      0 1 0
        2 1
        3 2
        4 3
      1 2 4
        3 5
        4 6
      2 3 7
        4 8
      3 4 9   ==> k = (-i²+i(2n-3)+2j-2)/2

      *************************************/
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            forces.append(list[i]->calculateForce(list[j]));
        }
    }

    for (int i = 0; i < n; ++i) {
        QVector2D acceleration = QVector2D(0,0);
        for (int j = i + 1; j < n; ++j) {
            acceleration += list[i]->calculateAcceleration(forces[k(n,i,j)]);
        }
        for (int p = 0; p < i; ++p) {
            acceleration += list[i]->calculateAcceleration(-forces[k(n,p,i)]);
        }
        list[i]->setAcceleration(acceleration);
    }

    for (int i = 0; i < n; ++i) {
        list[i]->move(0.02);
    }

    update();
}

Object::Object(QGraphicsScene *s, const QPointF &pos)
{
    scene = s;
    ellipseItem = scene->addEllipse(0, 0, 0, 0, QPen(), QBrush(QColor(100, 100, 100, 50)));
    ellipseItem->setPos(pos);
    speedLineItem = scene->addLine(0, 0, 0, 0, QPen(Qt::darkGreen));
    accelerationLineItem = scene->addLine(0, 0, 0, 0, QPen(Qt::red));
    mass = 0;
    speed = QVector2D(0, 0);
    path.moveTo(pos);
    pathItem = scene->addPath(path, QPen(QColor(50, 50, 255, 50)));
}

Object::~Object()
{
    scene->removeItem(ellipseItem);
    scene->removeItem(speedLineItem);
    scene->removeItem(accelerationLineItem);
    scene->removeItem(pathItem);
}

void Object::setRadius(double r)
{
    ellipseItem->setRect(-r, -r, 2*r, 2*r);
    mass = r*r*r;
}

void Object::setSpeed(const QVector2D &s)
{
    speed = s;
    speedLineItem->setLine(QLineF(QPointF(0, 0), s.toPointF()));
    speedLineItem->setPos(pos());
}

void Object::setAcceleration(const QVector2D &a)
{
    acceleration = a;
    accelerationLineItem->setLine(QLineF(QPointF(0, 0), a.toPointF()));
    accelerationLineItem->setPos(pos());
}

QPointF Object::pos() const
{
    return ellipseItem->pos();
}

QVector2D Object::calculateForce(const Object *o)
{
    QVector2D force = QVector2D(o->ellipseItem->pos() - ellipseItem->pos());
    double k = 4.0 * o->mass*mass / force.lengthSquared();
    force.normalize();
    force *= k;

    return force;
}

QVector2D Object::calculateAcceleration(const QVector2D &force)
{
    return force / mass;
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

    ellipseItem->setPos(ellipseItem->pos() + speed.toPointF() * time);

    path.lineTo(ellipseItem->pos());
    pathItem->setPath(path);

    setSpeed(speed);
    setAcceleration(acceleration);
}

