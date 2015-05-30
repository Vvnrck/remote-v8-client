#ifndef SERVERLISTENER_H
#define SERVERLISTENER_H

#include <string>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QtConcurrent/QtConcurrentRun>
#include <QThread>
#include <QObject>
#include <QString>
#include <QList>
#include <QSslError>
#include <QEventLoop>

#include "serverreplylistener.h"


class ServerListener : public QObject
{
    Q_OBJECT
    static QString serverUrl;

public:
    ServerListener();
    int run();
    int concurrentServerListener();
    ~ServerListener();

private:
    QString getTaskId();
    QString getTaskScript(QString taskId);
    QString getTaskData(QString taskId);
    bool postTaskResults(QString taskId, QString filepath);
    void registerWorker();

public:
signals:
    void appendToOutput(QString);
    void setWorker(QString);
    void incrementJobs();
};



#endif // SERVERLISTENER_H
