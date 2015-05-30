#include "serverlistener.h"
#include "clientsettingsmanager.h"
#include <QFutureWatcher>
#include <QDebug>
#include <QFile>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QProcess>
#include <QThread>
#include <exception>

class ApiConnectionException : std::exception {};

ServerListener::ServerListener() {}

int ServerListener::run()
{
    QFuture<int> future =
            QtConcurrent::run(this, &ServerListener::concurrentServerListener);
    QFutureWatcher<int> watcher;
    watcher.setFuture(future);
    return 0;
}

int ServerListener::concurrentServerListener()
{
    ServerListener::registerWorker();
    while (true)
    {
        QString taskId = "";
        try {
            taskId = ServerListener::getTaskId();
        }
        catch (ApiConnectionException e) {
            emit appendToOutput("Cannot connect to API. Sleeping 5 min");
            QThread::sleep(300);
            emit appendToOutput("Operational\n\n");
            continue;
        }

        if (taskId == "no-task") {
            emit appendToOutput("No tasks. Sleeping 30 sec...");
            QThread::sleep(30);
            emit appendToOutput("Operational\n\n");
            continue;
        }

        QString scriptFilename = ServerListener::getTaskScript(taskId);
        QString dataFilename = ServerListener::getTaskData(taskId);
        QString outputFile = taskId + ".out.csv";
        QString kernelPath = "remote_v8.exe";
        QStringList kernelCallArguments;
        kernelCallArguments << scriptFilename << dataFilename << outputFile << "10" << "1000";

        emit appendToOutput("Launching kernel...");
        QProcess *kernel = new QProcess();
        kernel->start(kernelPath, kernelCallArguments);
        emit appendToOutput("Computing...");
        kernel->waitForFinished(-1);
        emit appendToOutput("Done!");

        ServerListener::postTaskResults(taskId, outputFile);
        emit appendToOutput("Results uploaded\n\n");
        emit incrementJobs();

        qDebug() << taskId;
        qDebug() << scriptFilename;
        qDebug() << dataFilename;
        qDebug() << outputFile;
    }
    return 0;
}


ServerListener::~ServerListener()
{

}

QString ServerListener::getTaskId()
{
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QEventLoop loop;
    QString taskId;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    connect(
        manager, &QNetworkAccessManager::finished,
        [&] (QNetworkReply* reply) {
            if (reply->error() != QNetworkReply::NoError) {
                reply->deleteLater();
                throw ApiConnectionException();
            }
            QByteArray bytes = reply->readAll();
            taskId = QString::fromUtf8(bytes.data(), bytes.size());
            reply->deleteLater();
        }
    );
    manager->get(
        QNetworkRequest(QUrl(ClientSettingsManager::getServerUrl() +
                            "getTask?worker_id=" + ClientSettingsManager::getWorkerId()))
    );
    loop.exec();
    emit appendToOutput("Got task " + taskId);
    delete manager;
    return taskId;
}

QString ServerListener::getTaskScript(QString taskId)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QEventLoop loop;
    QString filename;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    connect(
        manager, &QNetworkAccessManager::finished,
        [&] (QNetworkReply* reply) {
            filename = reply->rawHeader("X-File-Name");
            QByteArray bytes = reply->readAll();
            QFile file(filename);
            file.open(QIODevice::WriteOnly);
            file.write(bytes.data());
            file.close();
            reply->deleteLater();
        }
    );
    manager->get(
        QNetworkRequest(QUrl(
            ClientSettingsManager::getServerUrl() +
            "getScript?worker_id=" + ClientSettingsManager::getWorkerId() +
            "&task_assignment_id=" + taskId))
    );
    loop.exec();
    emit appendToOutput("Got script for task " + taskId);
    delete manager;
    return filename;
}

QString ServerListener::getTaskData(QString taskId)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QEventLoop loop;
    QString filename;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    connect(
        manager, &QNetworkAccessManager::finished,
        [&] (QNetworkReply* reply) {
            filename = reply->rawHeader("X-File-Name");
            QByteArray bytes = reply->readAll();
            QFile file(filename);
            file.open(QIODevice::WriteOnly);
            file.write(bytes.data());
            file.close();
            reply->deleteLater();
        }
    );
    manager->get(
        QNetworkRequest(QUrl(ClientSettingsManager::getServerUrl() +
                             "getData?worker_id=" + ClientSettingsManager::getWorkerId() +
                             "&task_assignment_id=" + taskId))
    );
    loop.exec();
    emit appendToOutput("Got data for task " + taskId);
    delete manager;
    return filename;
}

bool ServerListener::postTaskResults(QString taskId, QString filepath)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"task_assignment_id\""));
    textPart.setBody(taskId.toUtf8());

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("attachment; name=\"result_file\"; filename=\"out.csv\""));
    QFile *file = new QFile(filepath);
    file->open(QIODevice::ReadOnly);
    filePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

    multiPart->append(textPart);
    multiPart->append(filePart);

    QEventLoop loop;
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QNetworkRequest request = QNetworkRequest(QUrl(ClientSettingsManager::getServerUrl() + "postTaskData"));
    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    manager->post(request, multiPart);
    loop.exec();
    file->close();
    delete multiPart;
    delete manager;
    return true;
}

void ServerListener::registerWorker()
{
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QEventLoop loop;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    manager->get(
        QNetworkRequest(QUrl(
            ClientSettingsManager::getServerUrl() +
            "registerWorker?worker_id=" + ClientSettingsManager::getWorkerId()))
    );
    loop.exec();
    emit setWorker(ClientSettingsManager::getWorkerId());
    delete manager;
}















