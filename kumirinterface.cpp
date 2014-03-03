#include "kumirinterface.h"
#include "ui_mainwindow.h"
#include "plugininterface.h"
#include <QtGui>
#include <QtCore>
kumirInterface::kumirInterface(const QString& path, QMainWindow* ui)
{
    //****
    int moduleId = 0;
    //****

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
                QString fileName = QDesktopServices::storageLocation(QDesktopServices::TempLocation)+'/' + p->asciiModuleName().replace(" ","_")+".py";

                qDebug()<<fileName;
                QFile codeGenerateFile(fileName);
                codeGenerateFile.open(QIODevice::WriteOnly);
                codeGenerateFile.write("import _myModule\n");


                p1->initialize(path+"/"+p->asciiModuleName().toLower().replace(' ', ""));
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

                QStringList codeGenerator;
                for(int i = 0; i < p->functionList().size(); i++)
                {
                    Shared::ActorInterface::Function func = p->functionList().at(i);
                    if (func.asciiName.startsWith("@"))
                        continue;
                    QString funcName ="def ";
                    funcName.append(func.asciiName);
                    funcName.replace(" ", "_");
                    funcName[3] = ' ';
                    funcName.append('(');
                    for(int k = 0; k < func.arguments.size(); k++)
                    {

                        funcName.append("arg");
                        funcName.append(QString::number(k));
                        if(k<func.arguments.size()-1)
                            funcName.append(',');
                    }
                    funcName.append("):\n    ");
                    funcName.append("return ");
                    funcName.append("_myModule.actor_call("); //0, 1, [arg1, arg1, arg3])

                    funcName.append(QString::number(moduleId));
                    funcName.append(", ");

                    funcName.append(QString::number(func.id));
                    funcName.append(", [");
                    for(int k = 0; k < func.arguments.size(); k++)
                    {

                        funcName.append("arg");
                        funcName.append(QString::number(k));
                        if(k<func.arguments.size()-1)
                            funcName.append(',');
                    }
                    funcName.append("])\n");
                    qDebug()<<funcName;
                    QTextStream ts(&codeGenerateFile);
                    ts.operator <<(funcName);


                    codeGenerator.append(funcName);
                }
                codeGenerateFile.close();

                moduleId++;
            }
        }
    }
    ui->addToolBar(kumirToolBar);
}
kumirInterface* kumirInterface::get(const QString &path, QMainWindow *ui)
{
    static kumirInterface* a = NULL;
    if(a == NULL)
    {
        a = new kumirInterface(path, ui);
    }
    return a;
}
