#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include "server.h"
#include <QDateTime>
#include <QIntValidator>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainServer=new Server(this);
    on_checkBox_stateChanged(0);

    ui->StudentIdStartEdit->setValidator(new QIntValidator(0,9999999));
    ui->StudentIdEndEdit->setValidator(new QIntValidator(0,9999999));

    connect(mainServer, SIGNAL(messageInQueue(QString)),this,SLOT(addMessage(QString)));
    ui->ButtonStackOnOff->setCurrentIndex(1);
    ui->RootDirectoryName->setText((rootDirectory=QDir::currentPath()));

    connect(mainServer,SIGNAL(mainGuiShowMessage(int,QString)),this,SLOT(showMessageBox(int,QString)));
    connect(this,SIGNAL(sayThreadToStop(bool,int,QString)),mainServer,SLOT(sayThreeadToStop(bool,int,QString)));
    ui->MaxFileSizeEdirt->setValidator(new QIntValidator(0,1000000000));
}

void MainWindow::addMessage(QString str)
{
    ui->MessageShower->appendPlainText(QDateTime::currentDateTime().toString("dd-MMM-yy hh:mm ap - ")+" "+str);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->FixedFileNameEdit->setEnabled(false);
    }
    else
    {
        ui->FixedFileNameEdit->setEnabled(true);
    }
}

void MainWindow::on_BrowseButton_clicked()
{
    rootDirectory=QFileDialog::getExistingDirectory(this,tr("Open Direcotry")
                                          ,"",QFileDialog::ShowDirsOnly
                                          | QFileDialog::DontResolveSymlinks);
    if(rootDirectory=="")
    {
        rootDirectory=QDir::currentPath();
    }
    ui->RootDirectoryName->setText(rootDirectory);
    mainServer->setRootDirectory(rootDirectory);

}

void MainWindow::on_pushButton_clicked()
{
    int start=ui->StudentIdStartEdit->text().toInt();
    int end=ui->StudentIdEndEdit->text().toInt();
    if(start<=end)
    {
        int i;
        for(i=start;i<=end;i++)
        {
            mainServer->addStudentInList(i);
        }
    }
    else
    {
        ui->StudentIdEndEdit->clear();
        ui->StudentIdStartEdit->clear();
    }
}

void MainWindow::on_StudentIdStartEdit_textChanged(const QString &arg1)
{
    ui->StudentIdEndEdit->setText(arg1);
}

void MainWindow::on_OffToggler_clicked()
{
    mainServer->startServer();
    ui->DoneButton->hide();
    ui->ButtonStackOnOff->setCurrentIndex(0);
}

void MainWindow::on_OnToggler_clicked()
{
    mainServer->stopServer();
    ui->DoneButton->show();
    ui->ButtonStackOnOff->setCurrentIndex(1);
}

void MainWindow::showMessageBox(int id,QString ip)
{
    QMessageBox x;
    x.setText(QString("%1 trying to connect different IP address").arg(id));
    x.setInformativeText("Do you want to allow?");
    x.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret=x.exec();
    switch (ret) {
    case QMessageBox::Ok:
        mainServer->addStudentIp(id,ip);
        emit sayThreadToStop(false,id,ip);
        break;
    case QMessageBox::Cancel:
        emit sayThreadToStop(true,id,ip);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_4_clicked()
{

}

void MainWindow::on_DoneButton_clicked()
{
    QStringList configation;
    configation.append(QString("%1").arg(ui->comboBox->currentIndex()));
    if(ui->checkBox->isChecked())
    {
        configation.append(ui->FixedFileNameEdit->text());
    }
    else
    {
        configation.append("-");
    }

    if(ui->FolderRadioButton->isChecked())
    {
        configation.append("Folder");
    }
    else
    {
        configation.append("File");
    }

    configation.append(ui->AllowedExtention->text());

    if(ui->MaxFileSizeEdirt->text()=="")
    {
        configation.append(QString("%1").arg(qint32(1000000000)));
    }
    else
    {
        configation.append(ui->MaxFileSizeEdirt->text());
    }

    if(ui->MaxSubMissionEdit->text()=="")
    {

        configation.append(QString("%1").arg(qint32(1000000000)));
    }
    else
    {
        configation.append(ui->MaxSubMissionEdit->text());
    }
    mainServer->setConfigurationList(configation);
    //qDebug()<<configation<<endl;
}
