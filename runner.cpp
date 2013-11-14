#include "runner.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

Runner::Runner(MainWindow *parent) :
    QThread(parent),
    mw_(parent)
{
    mutex_ = new QMutex;
}
Runner::~Runner ()
{
    delete mutex_;
}

void Runner::init (const QStringList &lines)
{
    mustrun_ = true;
    this->lines = lines;
}

void Runner::stop ()
{
    QMutexLocker lock(mutex_);

    mustrun_ = false;

}

void Runner::run()
{

    for(int i = 0; i < lines.size (); i++)
    {
        mutex_->lock ();
        bool b = mustrun_;
        mutex_->unlock ();
        if (b) {
            qDebug() << lines[i];
            emit doOutput (lines[i]);
            sleep(1);
        }
        else {
            break;
        }
    }
}
