#include "serversessionthread.h"
#include <QDebug>
#include <server.h>
#include <QByteArray>
#include <QDebug>
#include <QDataStream>
#include <QDir>

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
    qDebug()<<"Came"<<endl;
    QString root=parentServer->getRootDirectory();
    qDebug()<<root<<endl;
    QDir().mkdir(root+"/"+QString("%1").arg(studentId));
    dirName=root+"/"+QString("%1").arg(studentId);
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
            studentIp=tmp;
            if(parentServer->checkIp(studentId,tmp))
            {
                connectionVerified=true;
                sendAllDetails();
            }
            else
            {
                emit guiShowMessage(studentId,studentIp);
            }

        }
    }
    else
    {
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_4_0);
        QString fileOrFolder,fName;
        qint32 size;
        in>>fileOrFolder>>size>>fName;
        qDebug()<<fileOrFolder<<size<<fName;
        if(fileOrFolder=="Folder")
        {
            QDir().mkdir(dirName+"/"+fName);
        }
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

void ServerSessionThread::threadStop(bool value, int id, QString ip)
{

    if(value && studentId==id && studentIp==ip)
    {
        qDebug()<<"Came"<<endl;
        QString x("Invalid Access from "+ip);
        socket->write(x.toLatin1());
        //this->terminate();
    }
    else if(connectionVerified==false)
    {
        connectionVerified=true;
        sendAllDetails();
    }
}
