#include "server.h"

#include <QHostAddress>
#include "serversessionthread.h"
#include <QJsonObject>
#include <QThread>
#include <QMutex>
#include <QMessageBox>
#include <QDir>

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    rootDirectory=QDir::currentPath();
}

void Server::startServer()
{
    if(this->listen(QHostAddress::LocalHost,9001))
    {
        emit messageInQueue(QString("Server Listening to 127.0.0.1::9001"));
    }
    else
    {
        emit messageInQueue(QString("Failure: server couldn't be started"));
    }
}

bool Server::studentInStudentAllowedList(int id)
{
    if(studentIdAllowed.find(id)!=studentIdAllowed.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Server::addStudentInList(int id)
{
    //mutex.lock();
    studentIdAllowed[id]=1;
    //mutex.unlock();
    emit messageInQueue(QString("Student ID %1 added to allowed list").arg(id));
}

void Server::addStudentIp(int id, QString ip)
{
    ipMutex.lock();
    studentConnectIP[id]=ip;
    ipMutex.unlock();
}

bool Server::checkIp(int id, QString ip)
{
    if(studentConnectIP.find(id)!=studentConnectIP.end())
    {
        if(studentConnectIP[id]==ip)
        {
            return true;
        }
        return false;

    }
    else
    {
        addStudentIp(id,ip);
        return true;
    }

}

void Server::incomingConnection(qintptr socketDescriptor)
{

    ServerSessionThread *thread=new ServerSessionThread(socketDescriptor,this,this);
    //list.append(thread);
    connect(thread,SIGNAL(messageShow(QString)),this,SLOT(messagePropagate(QString)));
    connect(this, SIGNAL(mainServerClose()),thread,SLOT(terminate()));
    connect(thread,SIGNAL(guiShowMessage(int,QString)),this,SLOT(guiShowMessage(int,QString)));
    connect(this,SIGNAL(threadStop(bool,int,QString)),thread,SLOT(threadStop(bool,int,QString)));
    thread->start();
}

void Server::stopServer()
{
    if(this->isListening())
    {
        this->close();
        emit messageInQueue("Stopped Listening to 127.0.0.1::9001");
    }
    else
    {
        emit messageInQueue("Server already disconnected from 127.0.0.1::9001");
    }
}

void Server::messagePropagate(QString x)
{
    emit messageInQueue(x);
}

void Server::sayThreeadToStop(bool value,int id, QString ip)
{
    emit threadStop(value,id,ip);
}

void Server::guiShowMessage(int id, QString ip)
{
    emit mainGuiShowMessage(id,ip);
}
QString Server::getRootDirectory() const
{
    return rootDirectory;
}

void Server::setRootDirectory(const QString &value)
{
    rootDirectory = value;
}

