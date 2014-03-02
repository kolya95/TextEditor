#ifndef KUMIRINTERFACE_H
#define KUMIRINTERFACE_H
#include <QtCore>
#include <QtGui>
#include "actorinterface.h"
#include "ui_mainwindow.h"
class kumirInterface
{
    kumirInterface(const QString& path, QMainWindow* ui);
public:
    QVector<Shared::ActorInterface *> instanced;
    static kumirInterface* get(const QString& path = "", QMainWindow* ui = NULL);
};

#endif // KUMIRINTERFACE_H
