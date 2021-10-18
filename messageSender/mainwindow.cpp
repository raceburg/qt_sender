#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLabel>


void taskFunc(ObjectPointer in);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // инициализация окна
    mainFrame = new QFrame(this);
    label = new QLabel();
    dataList = new QComboBox();
    sendDataButton = new QToolButton();
    udpSender = new UdpSender();

    setCentralWidget(mainFrame);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(label);
    layout->addWidget(dataList);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    buttonLayout->addWidget(sendDataButton);
    layout->addItem(buttonLayout);
    mainFrame->setLayout(layout);

    label->setText("Message:");
    sendDataButton->setText("Send");

    customStyle();

    // параметры сетевого подлючения
    udpSender->connect(QHostAddress::LocalHost, 40302);
    // нажатие на кнопку
    connect(sendDataButton, &QToolButton::clicked, this, &MainWindow::sendDataButtonSlot);
    // заполнение сообщениями для отправки
    initDataList(QStringList()
                 << "Row 1 - AA"
                 << "Row 2 - ABC"
                 << "Row 3 - XYZN"
                 );
}

MainWindow::~MainWindow()
{
}

// заполнение сообщениями для отправки
void MainWindow::initDataList(QStringList data)
{
    dataList->clear();
    dataList->addItems(data);
}

// слот обраотки нажатия кнопки
void MainWindow::sendDataButtonSlot(bool a)
{
    Q_UNUSED(a)
    data.push_back(dataList->currentText());
    startTask();
}

// проверка очереди сообщений,
// запуск задачи для передачи данных по сети
void MainWindow::startTask()
{
    if (!backgroundRinning && !data.empty())
    {
        udpSender->setData(data);
        data.clear();
        ObjectPointer qi = qobject_cast<ObjectPointer>(udpSender);
        backgroundTask = new BackgroundTask(taskFunc, qi);
        connect(backgroundTask, &BackgroundTask::finished, this, &MainWindow::taskEnd);
        backgroundRinning = true;
        QThreadPool::globalInstance()->start(backgroundTask);
    }
}

// окончание передачи данных
void MainWindow::taskEnd()
{
    disconnect(backgroundTask, &BackgroundTask::finished, this, &MainWindow::taskEnd);
    while (!udpSender->sendBytes().isEmpty())
    {
        qDebug() << udpSender->sendBytes().front();
        udpSender->sendBytes().pop_front();
    }
    backgroundRinning = false;
    startTask();
}

// Настройка стиля инструментов управления
void MainWindow::customStyle()
{

    dataList->setStyleSheet(
        R"StyleSheet(
        QComboBox {
            border: 1px solid gray;
            border-radius: 3px;
            padding: 1px 18px 1px 3px;
            min-width: 6em;
        }

        QComboBox:!editable, QComboBox::drop-down:editable {
             background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                         stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,
                                         stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);
        }

        )StyleSheet");


    sendDataButton->setStyleSheet(
        R"StyleSheet(
        QToolButton:pressed {
          background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                            stop: 0 black, stop: 1 #f6f7fa);
        }
        )StyleSheet");
}

// функция выполняемая в потоке
// отправка данных по сети
void taskFunc(ObjectPointer in)
{
    UdpSender* sender = qobject_cast<UdpSender*>(in);
    sender->send();
}
