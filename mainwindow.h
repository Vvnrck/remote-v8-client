#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serverlistener.h"
#include "serverreplylistener.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ServerListener *serverListener;
    ServerReplyListener *serverReplyListener;

public slots:
    void appendToOutputWindow(QString s);
    void setWorkerId(QString id);
    void incrementJobsDone();
};

#endif // MAINWINDOW_H
