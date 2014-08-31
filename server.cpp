#include "server.h"

#include <QHostAddress>
#include "serversessionthread.h"
#include <QJsonObject>
#include <QThread>
#include <QMutex>
#include <QMessageBox>

Server::Server(QObject *parent)
    : QTcpServer(parent)
{


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
        qDebug()<<"Hello"<<endl;
        QMessageBox x;
        x.setText(QString("%1 trying to connect different IP address").arg(id));
        x.setInformativeText("Do you want to allow?");
        x.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        int ret=x.exec();
        switch (ret) {
        case QMessageBox::Ok:
            addStudentIp(id,ip);
            return true;
            break;
        case QMessageBox::Cancel:
            return false;
            break;
        default:
            return false;
            break;
        }
    }
    else
    {
        addStudentIp(id,ip);
        return true;
    }

}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<this->thread()<<endl;
    ServerSessionThread *thread=new ServerSessionThread(socketDescriptor,this,this);
    //list.append(thread);
    connect(thread,SIGNAL(messageShow(QString)),this,SLOT(messagePropagate(QString)));
    connect(this, SIGNAL(mainServerClose()),thread,SLOT(terminate()));
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
