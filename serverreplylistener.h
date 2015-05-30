#ifndef SERVERREPLYLISTENER_H
#define SERVERREPLYLISTENER_H

#include <QObject>
#include <QtNetwork/QNetworkReply>

class ServerReplyListener : public QObject
{
    Q_OBJECT
private:
    static ServerReplyListener* self;
    static QNetworkReply *reply;

    ServerReplyListener();
    static void init();

public:
    QNetworkReply* getReply();
    QNetworkReply* setReply(QNetworkReply *reply);
    static ServerReplyListener* getInstance();

public slots:
    void replyFinished (QNetworkReply *reply);

signals:
    void serverHeadRequestAquired(QNetworkReply *reply);
    void sendMessageToGUI(QString message);
};


#endif // SERVERREPLYLISTENER_H
