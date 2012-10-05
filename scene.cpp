#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QTime>
#include <QStringList>
#include <QTimerEvent>
#include <QDebug>

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent)
{
    _state = 1;
    _timer = 0;
    startTimer(40);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);

    switch (_state) {
    case 1:
        // créer 1st obj
        _list << new Corpus(this, event->scenePos());
        _state++;
        break;
    case 2:
        // vol 1st obj
        _list.last()->setRadius(QVector2D(_list.last()->pos() - event->scenePos()).length());
        _state++;
        break;
    case 3:
        // vit 1st obj
        _list.last()->setSpeed(QVector2D(event->scenePos() - _list.last()->pos()));

        _state = 1;
        break;
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);

    emit mouseMoved(event->scenePos());

    switch (_state) {
    case 1:
        // créer 1st obj
        break;
    case 2:
        // vol 1st obj
        _list.last()->setRadius(QVector2D(_list.last()->pos() - event->scenePos()).length());
        break;
    case 3:
        // vit 1st obj
        _list.last()->setSpeed(QVector2D(event->scenePos() - _list.last()->pos()));
        break;
    }
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    QGraphicsScene::keyPressEvent(event);

    switch (event->key()) {
    case Qt::Key_Space:
        if (_timer == 0) {
            fixMomentum();
            // start
            _timer = startTimer(0);
            _timeline.start();
            _lasttime = 0;
            // set to an invalid state
            _state = 100;
        } else {
            // stop
            killTimer(_timer);
            _timer = 0;
            // set to first state
            _state = 1;
        }
        return;
    case Qt::Key_Return:
        if (_timer == 0) {
            while (!_list.isEmpty())
                delete _list.takeLast();
            _state = 1;
        } else {
            // stop
            killTimer(_timer);
            _timer = 0;
            while (!_list.isEmpty())
                delete _list.takeLast();
            _state = 1;
        }
        return;
    }

    if (_state == 2) { // volume
        if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
            _list.last()->setRadius((event->key() - Qt::Key_0) * 10);

            _state++;
            return;
        }
    }
    if (_state == 3) { // vitesse
        if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
            const QVector2D &s = _list.last()->speed();
            _list.last()->setSpeed(s / s.length() * (event->key() - Qt::Key_0) * 10);

            _state = 1;
            return;
        }
    }
}

void Scene::fixMomentum()
{
    QVector2D momentum(0.0, 0.0);
    double mass = 0.0;
    for (int i = 0; i < _list.size(); ++i) {
        momentum += _list[i]->speed() * _list[i]->mass();
        mass += _list[i]->mass();
    }

    QVector2D speed = momentum / mass;
    for (int i = 0; i < _list.size(); ++i) {
        _list[i]->setSpeed(_list[i]->speed() - speed);
    }
}

int k(int n, int i, int j)
{
    return (-i*i + i*(2*n-3) + 2*j - 2) / 2;
}

void Scene::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _timer) {
        double dt = _timeline.elapsed() - _lasttime;
        if (dt == 0.0)
            return;
        _lasttime = _timeline.elapsed();
        moveSystem(dt / 1000.0);
    } else {
        update();
    }
}

void Scene::moveSystem(double dt)
{
    qDebug() << "dt " << dt;
    int n = _list.size();
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
            forces.append(_list[i]->calculateForce(_list[j]));
        }
    }

    for (int i = 0; i < n; ++i) {
        QVector2D force = QVector2D(0,0);
        for (int j = i + 1; j < n; ++j) {
            force += forces[k(n,i,j)];
        }
        for (int p = 0; p < i; ++p) {
            force -= forces[k(n,p,i)];
        }
        _list[i]->setAcceleration(_list[i]->calculateAcceleration(force));
    }

    for (int i = 0; i < n; ++i) {
        _list[i]->move(dt);
    }
}


