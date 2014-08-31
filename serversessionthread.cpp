#include "serversessionthread.h"
#include <QDebug>
#include <server.h>
#include <QByteArray>
#include <QDebug>

ServerSessionThread::ServerSessionThread(int socketDescriptor,Server *parentServer, QObject *parent)
    : QThread(parent)
{
    this->socketDescriptor=socketDescriptor;
    this->parentServer=parentServer;
    connectionVerified=false;
}

ServerSessionThread::~ServerSessionThread()
{

}

void ServerSessionThread::run()
{
    qDebug()<<socketDescriptor<<" in Run"<<endl;
    socket=new QTcpSocket();
    if(!socket->setSocketDescriptor(socketDescriptor))
    {
        qDebug()<<socket->error()<<endl;
        emit error(socket->error());
        return;
    }
    isDisconncted=false;
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);
    exec();
}

void ServerSessionThread::sendAllDetails()
{

}

void ServerSessionThread::readyRead()
{
    if(!connectionVerified)
    {
        QByteArray Data = socket->readAll();
        QString x(Data);
        studentId=x.toInt();
        if(!parentServer->studentInStudentAllowedList(studentId))
        {
            QString x("Invalid Student Id");
            socket->write(x.toLatin1());
            socket->close();
        }
        else
        {
            QString tmp=socket->peerAddress().toString()+"::"+QString("%1").arg(socket->peerPort());
            if(parentServer->checkIp(studentId,tmp))
            {
                connectionVerified=true;
                sendAllDetails();
            }
            else
            {
                QString x("Invalid access from different IP");
                socket->write(x.toLatin1());
                socket->close();
            }

        }
    }
    else
    {

    }
    //socket->write(Data);
}

void ServerSessionThread::disconnected()
{
    if(!isDisconncted)
    {
        isDisconncted=true;
        qDebug() << socketDescriptor << " Disconnected";
        socket->deleteLater();
    }
    exit(0);
}
