#include "mainwindow.h"
#include "serverreplylistener.h"
#include "ui_mainwindow.h"

#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->serverListener = new ServerListener();
    this->serverReplyListener = ServerReplyListener::getInstance();

    connect(serverListener,      SIGNAL(appendToOutput(QString)),
            this,                SLOT(appendToOutputWindow(QString)));
    connect(serverListener,      SIGNAL(setWorker(QString)),
            this,                SLOT(setWorkerId(QString)));
    connect(serverListener,      SIGNAL(incrementJobs()),
            this,                SLOT(incrementJobsDone()));

    this->serverListener->run();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete serverListener;
    delete serverReplyListener;
}

void MainWindow::appendToOutputWindow(QString s)
{
    QTime time = QTime::currentTime();
    QString stime = time.toString("[hh:mm:ss.zzz] ");
    ui->executionLog->append(stime + s);
}

void MainWindow::setWorkerId(QString id)
{
    ui->workerIdDisplay->append(id);
}

void MainWindow::incrementJobsDone()
{
    auto text = ui->tasksDoneCounter->text();
    auto jobs = text.toInt();
    ui->tasksDoneCounter->setNum(jobs + 1);
}
