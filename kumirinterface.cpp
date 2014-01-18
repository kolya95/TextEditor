#include "kumirinterface.h"
#include "ui_mainwindow.h"
#include "plugininterface.h"
#include <QtGui>
#include <QtCore>
kumirInterface::kumirInterface(const QString& path, QMainWindow* ui)
{

    qDebug()<<"asd";
    //QString path = QCoreApplication::applicationDirPath()+"/../TextEditor/";
    //path = QDir::cleanPath(path);
    //path = QDir::toNativeSeparators(path);
    QDir dir(path);
    QStringList filters;
    QStringList libNames;
    filters << "*.so";
    dir.setNameFilters(filters);
    libNames = dir.entryList();

    QToolBar* kumirToolBar = new QToolBar();
    Q_FOREACH(QString name, libNames)
    {
        qDebug()<<name;

        QPluginLoader L(dir.absoluteFilePath(name));
        L.load();
        qDebug()<<L.errorString();
        QObject* obj = L.instance();
        qDebug()<<obj;
        if(!obj)
        {
            continue;
        }
        else
        {
            Shared::ActorInterface * p = qobject_cast<Shared::ActorInterface*>(obj);
            Shared::PluginInterface * p1 = qobject_cast<Shared::PluginInterface*>(obj);
            qDebug()<<"asd";
            if(!p||!p1)
            {
                continue;
            }
            else
            {
                p1->initialize(path+"/"+p->asciiModuleName().toLower());
                qDebug()<<name<<"\n";
                qDebug()<<p->asciiModuleName()<<"\n";
                instanced.push_back(p);
                ui->menuBar()->addSeparator();

                QList<class QMenu*> b = p->moduleMenus();
                Q_FOREACH(QMenu* a, b)
                {
                    ui->menuBar()->addMenu(a);
                    //p->mainWidget()->show();
                    QAction* act = kumirToolBar->addAction(p->asciiModuleName()+" show");
                    QObject::connect(act, SIGNAL(triggered()),
                                     p->mainWidget(), SLOT(show()));

                }
            }
        }
    }
    ui->addToolBar(kumirToolBar);
}
