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

    QString getRootDirectory() const;
    void setRootDirectory(const QString &value);

    QStringList getConfigurationList() const;
    void setConfigurationList(const QStringList &value);

protected:
    void incomingConnection(qintptr socketDescriptor);

signals:
    void messageInQueue(QString);
    void mainServerClose();
    void threadStop(bool value, int id, QString ip);
    void mainGuiShowMessage(int,QString);

public slots:
    void messagePropagate(QString);
    void sayThreeadToStop(bool value, int id, QString ip);
    void guiShowMessage(int id, QString ip);


private:
    QString msg;
    QMap<int,int> studentIdAllowed;
    QMap<int,QString> studentConnectIP;
    QMutex mutex;
    QMutex ipMutex;
    QString rootDirectory;
    QStringList configurationList;

};

#endif // SERVER_H
