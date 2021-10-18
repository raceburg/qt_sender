#include "udpsender.h"
#include <QThread>

UdpSender::UdpSender(QObject *parent)
    : QObject(parent)
//    , socket(nullptr)
{

}

// инициализация парамтров подключения
bool UdpSender::connect(const QHostAddress &address, quint32 port)
{
    host = address;
    this->port = port;
    return true;
}

// заполнение очереди данных для передачи
void UdpSender::setData(QQueue<QString>& sendQueue)
{
    while (!sendQueue.empty())
    {
       QByteArray sendData = sendQueue.front().toUtf8();
       this->sendQueue.push_back(sendData);
       sendQueue.pop_front();
    }
}

// передача данных в порт
void UdpSender::send()
{
    QUdpSocket* socket = new QUdpSocket();
    sendingBytes.clear();
    while (!sendQueue.empty())
    {
        qint64 bytes = socket->writeDatagram(sendQueue.front(), host, port);
        sendingBytes.push_back(bytes);
        sendQueue.pop_front();
    }
    delete socket;
}

// получение очереди с количеством переданных байт
QQueue<qint64>& UdpSender::sendBytes()
{
    return sendingBytes;
}
