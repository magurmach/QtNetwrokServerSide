#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include "server.h"
#include <QDateTime>
#include <QIntValidator>

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
    ui->ButtonStackOnOff->setCurrentIndex(0);
}

void MainWindow::on_OnToggler_clicked()
{
    mainServer->stopServer();
    ui->ButtonStackOnOff->setCurrentIndex(1);
}
