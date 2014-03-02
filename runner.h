#ifndef RUNNER_H
#define RUNNER_H
#include <Python.h>
#include <QThread>
#include <QtCore>
#include <QtGui>
#include "actorinterface.h"
class Runner : public QThread
{
    Q_OBJECT
public:

    explicit Runner(class MainWindow *parent);

    void init(const QString& lines);
    
    void run();

    void stop();
    ~Runner();

    static PyObject* pythonPrint(PyObject*,PyObject* args);
    static PyObject* pythonErrPrint(PyObject*,PyObject* args);
    static PyObject* py_call(PyObject*,PyObject* args);

    static PyObject* call(int moduleId, int funcId, PyObject* args);

    static void pyImport();
Q_SIGNALS:
    void doOutput(const QString &);
    void doErrOutput(const QString &);
    
public Q_SLOTS:
      void handleActorDone();

private:
    PyObject * runModule;
    class MainWindow * mw_;
    QStringList lines;
    QString programmText;
    PyThreadState* interpretator;
    QTextEdit * out;
    bool mustrun_;
    QMutex * mutex_;




//    static PyObject* init_functions();
};

static bool actorDone_ = false;
static QMutex * actorDoneMutex_ = new QMutex();


static PyObject* init_functions();
static Runner* RUN;
#endif // RUNNER_H
