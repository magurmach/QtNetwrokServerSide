#ifndef SERVERSESSIONTHREAD_H
#define SERVERSESSIONTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "server.h"

class ServerSessionThread : public QThread
{
    Q_OBJECT
public:
    ServerSessionThread(int socketDescriptor, Server* parentServer, QObject *parent);
    ~ServerSessionThread();
    void run();
    void sendAllDetails();

signals:
    void error(QTcpSocket::SocketError socketError);

    void messageShow(QString);

public slots:
    void readyRead();
    void disconnected();

private:
    int socketDescriptor;
    QString msg;
    QTcpSocket *socket;
    Server *parentServer;

    bool connectionVerified;
    bool isDisconncted;

    int studentId;

};

#endif // SERVERSESSIONTHREAD_H
