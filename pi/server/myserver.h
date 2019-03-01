#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

class myserver : public QObject
{
    Q_OBJECT
public:
    explicit myserver(QObject *parent = 0);

signals:

public slots:
    void newConnection();
private:
    QTcpServer *server;
};

#endif // MYSERVER_H
