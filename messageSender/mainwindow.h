#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QToolButton>
#include <udpsender.h>
#include <backgroundtask.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initDataList(QStringList);

private:
    QFrame              *mainFrame;
    QLabel              *label;
    QComboBox           *dataList;
    QToolButton         *sendDataButton;
    UdpSender           *udpSender;
    QQueue<QString>     data;

    bool                backgroundRinning = false;
    BackgroundTask      *backgroundTask = nullptr;

    void startTask();
    void customStyle();

public slots:
    void sendDataButtonSlot(bool);
    void taskEnd();
};
#endif // MAINWINDOW_H
