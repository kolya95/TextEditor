#ifndef KUMIRINTERFACE_H
#define KUMIRINTERFACE_H
#include <QtCore>
#include <QtGui>
#include "actorinterface.h"
#include "ui_mainwindow.h"
class kumirInterface
{
    QVector<Shared::ActorInterface *> instanced;
public:
    kumirInterface(const QString& path, QMainWindow* ui);
};

#endif // KUMIRINTERFACE_H
