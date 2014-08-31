#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QMap>
#include <QThread>
#include <QMutex>


class Server : public QTcpServer
{
    Q_OBJECT

public:

    Server(QObject *parent=0);

    void startServer();
    void stopServer();
    bool studentInStudentAllowedList(int id);
    void addStudentInList(int id);
    void addStudentIp(int id, QString ip);
    void askForDifferentIp();
    bool checkIp(int id, QString ip);

protected:
    void incomingConnection(qintptr socketDescriptor);

signals:
    void messageInQueue(QString);
    void mainServerClose();

public slots:
    void messagePropagate(QString);


private:
    QString msg;
    QMap<int,int> studentIdAllowed;
    QMap<int,QString> studentConnectIP;
    QMutex mutex;
    QMutex ipMutex;
};

#endif // SERVER_H
