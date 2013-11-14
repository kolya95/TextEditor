#ifndef RUNNER_H
#define RUNNER_H

#include <QThread>
#include <QtCore>
#include <QtGui>

class Runner : public QThread
{
    Q_OBJECT
public:
    explicit Runner(class MainWindow *parent);

    void init(const QStringList & lines);
    
    void run();

    void stop();
    ~Runner();


signals:
    void doOutput(const QString &);
    
public slots:

private:
    class MainWindow * mw_;
    QStringList lines;
    QTextEdit * out;
    bool mustrun_;
    QMutex * mutex_;
    
};

#endif // RUNNER_H
