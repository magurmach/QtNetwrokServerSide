#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_BrowseButton_clicked();

    void on_pushButton_clicked();

    void on_StudentIdStartEdit_textChanged(const QString &arg1);

    void on_OffToggler_clicked();

    void on_OnToggler_clicked();

public slots:
    void addMessage(QString str);
private:
    Ui::MainWindow *ui;
    QString rootDirectory;

    Server *mainServer;

};

#endif // MAINWINDOW_H
