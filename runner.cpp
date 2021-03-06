#include "runner.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "actorinterface.h"
#include "kumirinterface.h"
Runner::Runner(MainWindow *parent) :
    QThread(parent),
    mw_(parent)
{
    PyImport_AppendInittab("_myModule", &(init_functions));
    Py_Initialize();
    PyEval_InitThreads();
    mutex_ = new QMutex;

    actorDoneMutex_ = new QMutex();
    actorDone_ = false;
}
Runner::~Runner ()
{
    delete mutex_;
    Py_Finalize();

}

void Runner::init (const QString& lines)
{
    Q_FOREACH (Shared::ActorInterface* actor, kumirInterface::get()->instanced) {
        actor->reset();
    }
    mustrun_ = true;
    programmText = lines;
}
PyObject* Runner::pythonPrint(PyObject *, PyObject *args)
{
    PyObject * msg = PyTuple_GetItem(args, 0);
    wchar_t * wcs = PyUnicode_AsWideCharString(msg, NULL);
       QString qs = QString::fromWCharArray(wcs);
       Q_EMIT RUN->doOutput(qs);
       PyMem_Free(wcs);
       Py_INCREF(Py_None);
       return Py_None;
}

PyObject* Runner::pythonErrPrint(PyObject *, PyObject *args)
{
    PyObject * msg = PyTuple_GetItem(args, 0);
    wchar_t * wcs = PyUnicode_AsWideCharString(msg, NULL);
    QString qs = QString::fromWCharArray(wcs);
    Q_EMIT RUN->doErrOutput(qs);
    PyMem_Free(wcs);
    Py_INCREF(Py_None);
    return Py_None;
}
PyObject* Runner::py_call(PyObject *, PyObject *args)
{
    PyObject * modIdObj = PyTuple_GetItem(args, 0);
    PyObject * funIdObj = PyTuple_GetItem(args, 1);
    PyObject * argsObj  = PyTuple_GetItem(args, 2);
    int moduleId = PyLong_AsLong(modIdObj);
    int functionId = PyLong_AsLong(funIdObj);
    return call(moduleId, functionId, argsObj);
}
void Runner::handleActorDone()
{
    actorDoneMutex_->lock();
    actorDone_ = true;
    actorDoneMutex_->unlock();
}

PyObject* Runner::call(int moduleId, int funcId, PyObject *args)
{
    int size = PyList_Size(args);
    kumirInterface * temp = kumirInterface::get();
    int num = moduleId;
    /*for(int i = 0; i < temp->instanced.size(); i++)
    {
        if(temp->instanced[i]->  == moduleId)
        {
            num = i;
            break;
        }
    }*/
    QVariantList argQVar;
    Shared::ActorInterface::FunctionList functions = temp->instanced[num]->functionList();
    temp->instanced[num]->connectSync(RUN, SLOT(handleActorDone()));
    for(int i = 0; i<functions.size(); i++)
    {
        Shared::ActorInterface::Function func = functions.at(i);
        if(func.id == funcId)
        {
            if(func.arguments.size() == size)
            {

                for(int j = 0; j < size; j++)
                {

                    PyObject * pyArr = PyList_GetItem(args,j);
                    Shared::ActorInterface::Argument argType = func.arguments.at(j);

                    if(argType.type == Shared::ActorInterface::Bool)
                    {
                        bool Variable = PyLong_AsLong(pyArr);
                        argQVar.append(QVariant(Variable));
                    }
                    else if (argType.type == Shared::ActorInterface::Int)
                    {
                        int Variable = PyLong_AsLong(pyArr);
                        argQVar.append(QVariant(Variable));
                    }
                    else if (argType.type == Shared::ActorInterface::Real)
                    {
                        double Variable = PyLong_AsDouble(pyArr);
                        argQVar.append(QVariant(Variable));
                    }
                    else if (argType.type == Shared::ActorInterface::Char || argType.type == Shared::ActorInterface::String)
                    {
                        wchar_t * wcs = PyUnicode_AsWideCharString(pyArr, NULL);
                        QString Variable = QString::fromWCharArray(wcs);
                        PyMem_Free(wcs);
                        argQVar.append(QVariant(Variable));
                    }
                }
                Shared::EvaluationStatus evStat = temp->instanced[num]->evaluate(funcId, argQVar);
                switch(evStat)
                {
                case Shared::ES_Async:
                     while (true) {
                     bool v;
                     actorDoneMutex_->lock();
                     v = actorDone_;
                     actorDoneMutex_->unlock();
                     if (v) break;
                     msleep(1);
                    }
                    actorDone_ = false;
                    break;
                case Shared::ES_Error:
                    Q_EMIT RUN->doErrOutput(temp->instanced[num]->errorText());
                    break;
                case Shared::ES_NoResult:
                    break;
                case Shared::ES_StackRezResult:
                    break;
                case Shared::ES_RezResult:
                    break;
                case Shared::ES_StackResult:
                    PyObject * res = new PyObject();
                    if(func.returnType == Shared::ActorInterface::Bool)
                    {
                        res = PyLong_FromLong(temp->instanced[num]->result().toBool());
                        qDebug()<<temp->instanced[num]->result().toBool();
                        Py_INCREF(res);
                        return res;
                    }
                    else if (func.returnType == Shared::ActorInterface::Int)
                    {

                    }
                    else if (func.returnType == Shared::ActorInterface::Real)
                    {

                    }
                    else if (func.returnType == Shared::ActorInterface::Char || func.returnType == Shared::ActorInterface::String)
                    {

                    }
                    break;


                }
                //actorDone_ = false;

            }
            else
            {
                /// error
            }
        }
    }
    for(int i = 0; i< argQVar.size(); i++)
        qDebug()<<argQVar[i];
    qDebug()<<moduleId<<funcId;
    Py_INCREF(Py_None);
    return Py_None;
}

void Runner::pyImport()
{
    PyObject * sysPath = PySys_GetObject("path");
    char dir[512]; qMemSet(dir, 0, sizeof(dir));
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
    qMemCopy(dir, path.toUtf8().constData(), path.toUtf8().size());

    PyObject * myFunctionsPath = PyUnicode_FromString(dir);
    PyList_Insert(sysPath, 0, myFunctionsPath);

    const QByteArray tempPath = QDir::toNativeSeparators(
                QDesktopServices::storageLocation(QDesktopServices::TempLocation)
                ).toUtf8();
    PyObject* pyTempPath = PyUnicode_FromString(tempPath.constData());
    PyList_Insert(sysPath, 0, pyTempPath);



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
       { "myErrPrint", Runner::pythonErrPrint, METH_VARARGS, ""},
        {"actor_call",Runner::py_call,METH_VARARGS,""},
        //{"call", Runner::call, METH_VARARGS, "" },
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
   if (b)
   {
       wchar_t * buffer = (wchar_t*) malloc((1+programmText.length()) * sizeof(wchar_t));
       buffer[programmText.length()] = L'\0';
       programmText.toWCharArray(buffer);
       PyObject * pyProgrammText = PyUnicode_FromUnicode(buffer, programmText.length());
       free(buffer);
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
   else
   {
        //Py_EndInterpreter(interpretator);

        return;
   }
   //Py_EndInterpreter(interpretator);
}
