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

    void guiShowMessage(int, QString);


public slots:
    void readyRead();
    void disconnected();

    void threadStop(bool value, int id,QString ip);

private:
    int socketDescriptor;
    QString msg;
    QTcpSocket *socket;
    Server *parentServer;

    bool connectionVerified;
    bool isDisconncted;

    int studentId;
    QString studentIp;

    bool isFileorFolder;

    QString dirName;
    bool lock;

};

#endif // SERVERSESSIONTHREAD_H
