#ifndef UDPSENDER_H
#define UDPSENDER_H
#include <QObject>
#include <QUdpSocket>
#include <QQueue>


class UdpSender : public QObject
{
    Q_OBJECT
public:
    explicit UdpSender(QObject *parent = nullptr);
    bool connect(const QHostAddress&, quint32);
    void setData(QQueue<QString>& sendQueue);
    void send();
    QQueue<qint64>& sendBytes();


private:
    QQueue<QByteArray>  sendQueue;
    QQueue<qint64>      sendingBytes;
//    QUdpSocket*         socket;
    QHostAddress        host;
    quint16             port;

signals:


};

#endif // UDPSENDER_H
