#include "serverreplylistener.h"

ServerReplyListener* ServerReplyListener::self = nullptr;
QNetworkReply* ServerReplyListener::reply = nullptr;


void ServerReplyListener::init()
{
    if (ServerReplyListener::self == nullptr)
        ServerReplyListener::self = new ServerReplyListener();
}


ServerReplyListener::ServerReplyListener() {}

void ServerReplyListener::replyFinished (QNetworkReply *reply)
{
    qDebug() << "ServerReplyListener::replyFinished (QNetworkReply *reply)";
    emit serverHeadRequestAquired(reply);
}

QNetworkReply* ServerReplyListener::getReply()
{
    ServerReplyListener::init();
    return ServerReplyListener::getInstance()->reply;
}

QNetworkReply* ServerReplyListener::setReply(QNetworkReply *reply)
{
    ServerReplyListener::init();
    return ServerReplyListener::getInstance()->reply = reply;
}

ServerReplyListener* ServerReplyListener::getInstance()
{
    ServerReplyListener::init();
    return ServerReplyListener::self;
}
