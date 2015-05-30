#ifndef CLIENTSETTINGSMANAGER_H
#define CLIENTSETTINGSMANAGER_H

#include <QString>
#include <QFileInfo>
#include <QFile>
#include <QUuid>


class ClientSettingsManager
{
    static QString workerId;
    static QString serverUrl;

public:
    ClientSettingsManager();
    static QString getServerUrl()
    {
        if (ClientSettingsManager::serverUrl != "")
            return ClientSettingsManager::serverUrl;
        QFileInfo fileInfo("server_url");
        if (fileInfo.exists() && fileInfo.isFile())
        {
            QFile file("server_url");
            file.open(QIODevice::ReadOnly);
            QString serverUrl(file.readAll());
            ClientSettingsManager::serverUrl = serverUrl;
            file.close();
            return serverUrl;
        }
        return "http://127.0.0.1:8000/";
    }

    static QString getWorkerId()
    {
        if (ClientSettingsManager::workerId != "")
            return ClientSettingsManager::workerId;

        QFileInfo fileInfo("worker_id");
        if (fileInfo.exists() && fileInfo.isFile())
        {
            QFile file("worker_id");
            file.open(QIODevice::ReadOnly);
            QString workerId(file.readAll());
            ClientSettingsManager::workerId = workerId;
            file.close();
            return workerId;
        }
        else
        {
            QFile file("worker_id");
            file.open(QIODevice::WriteOnly);
            QString workerId(QUuid::createUuid().toString());
            file.write(workerId.toLocal8Bit());
            ClientSettingsManager::workerId = workerId;
            file.close();
            return workerId;
        }
    }
};

QString ClientSettingsManager::workerId = "";
QString ClientSettingsManager::serverUrl = "";

#endif // CLIENTSETTINGSMANAGER_H
