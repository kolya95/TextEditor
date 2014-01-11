#include "runner.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

Runner::Runner(MainWindow *parent) :
    QThread(parent),
    mw_(parent)
{
    PyImport_AppendInittab("_myModule", &(init_functions));
    Py_Initialize();
    PyEval_InitThreads();
    mutex_ = new QMutex;
}
Runner::~Runner ()
{
    delete mutex_;

    //delete[] programmText;
    //Py_Finalize();
}

void Runner::init (const QString& lines)
{
    mustrun_ = true;
    try{
        programmText = new char[lines.size()+1];
        for(int i = 0; i<lines.size(); i++)
        {
            programmText[i] = lines[i].toAscii();
        }
        programmText[lines.size()] = '\0';
    }
    catch(std::bad_alloc&)
    {
        ///error
    }
}
PyObject* Runner::pythonPrint(PyObject *, PyObject *args)
{
    PyObject * msg = PyTuple_GetItem(args, 0);
    wchar_t * wcs = PyUnicode_AsWideCharString(msg, NULL);
       QString qs = QString::fromWCharArray(wcs);
       //qDebug() << qs;
       Q_EMIT RUN->doOutput(qs);
       PyMem_Free(wcs);
       Py_INCREF(Py_None);
       return Py_None;
}
void Runner::pyImport()
{
    PyObject * sysPath = PySys_GetObject("path");
    char * dir;
    //char* dir;
    QString path = QCoreApplication::applicationDirPath();
    path+= "/../TextEditor/python";
    //qDebug()<<path;
    path = QDir::cleanPath(path);
    path = QDir::toNativeSeparators(path);
    //qDebug()<<path;
    /*QString QDir::cleanPath ( const QString & path ) [static]
    -- приведение пути к нормализованному виду
    QString QDir::toNativeSeparators ( const QString & pathName ) [static]
    -- для Unix не имеет значения, для Windows -- меняет / на \
    */
    dir = path.toUtf8().data();

    PyObject * myFunctionsPath = PyUnicode_FromString(dir);
    PyList_Insert(sysPath, 0, myFunctionsPath);

    //runModule = PyImport_ImportModule("myFunctions");

    RUN->runModule = PyImport_ImportModule("myFunctions");

    if(0==RUN->runModule)
    {
        PyObject * ptype, *pvalue, *ptraceback;
               PyErr_Fetch(&ptype, &pvalue, &ptraceback);
               PyObject_Print(pvalue, stderr,0);
               PyObject_Print(ptraceback, stderr,0);
    }
}

void Runner::stop ()
{
    QMutexLocker lock(mutex_);
    mustrun_ = false;
    Py_EndInterpreter(interpretator);
}
static PyObject* init_functions()
{
    static PyMethodDef _myMethods[] = {
       { "myPrint", Runner::pythonPrint, METH_VARARGS, ""},
       { NULL, NULL, 0, NULL }
    };

    static PyModuleDef _myModule = {
       PyModuleDef_HEAD_INIT, "_myModule", NULL, -1, _myMethods,
       NULL, NULL, NULL, NULL
    };
    return PyModule_Create(&_myModule);
}

void Runner::run()
{
    RUN = this;
    if(RUN->runModule == 0)
        return;
    interpretator = Py_NewInterpreter();
    pyImport();
    mutex_->lock ();
    bool b = mustrun_;
    mutex_->unlock ();
   if (b) {
       PyObject * pyProgrammText = PyUnicode_FromString(programmText);
        //PyRun_SimpleString(programmText);
       PyObject* func = PyObject_GetAttrString(runModule, "func");
       // возвращает объект-функцию из модуля, либо 0, если функция в модуле не найдена
       if(0==func)
       {
           PyErr_Print();
       }
       else
       {
            PyObject* args = PyTuple_New(1);
            // Создает объект-кортеж, который должен содержать аргументы вызываемой функции
            PyTuple_SetItem(args, 0, pyProgrammText);
            // добавляет элемент в кортеж
            PyObject_CallObject(func, args);
            // Вызывает функцию и возвращает ее результат, либо 0 в случае ошибки
            Py_DECREF(args);
       }
    }
   else {
        Py_EndInterpreter(interpretator);

        return;
   }
   Py_EndInterpreter(interpretator);
}
