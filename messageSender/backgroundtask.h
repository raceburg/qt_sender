#ifndef BACKGROUNDTASK_H
#define BACKGROUNDTASK_H
#include <QObject>
#include <QRunnable>
#include <QList>
#include <QThreadPool>
#include <functional>

// --------------------------------
// класс для создания потока
// --------------------------------

typedef QObject* ObjectPointer;
typedef std::function<void (ObjectPointer)> FunctionPointer;

class BackgroundTask : public QObject, public QRunnable
{
    Q_OBJECT

public:
    BackgroundTask(FunctionPointer taskFunction,
                    ObjectPointer taskInputArgs)
    {
        fn = taskFunction;
        args = taskInputArgs;
    }

    void run() override
    {
        try
        {
            fn(args);
        }
        catch (...)
        {
            emit error("Error in task");
        }
        emit finished();
    }

signals:
//    end task
    void finished();
//    error task
    void error(QString);


private:
    FunctionPointer fn;
    ObjectPointer args;
};

#endif // BACKGROUNDTASK_H
