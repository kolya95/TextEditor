#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    friend class Runner;
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void fileSave();
    void fileOpen();
    void Run();
    void output(const QString & text);
    void reEnabled();
    void Stop();
    
private:
    Ui::MainWindow *ui;
    class Runner * runner_;
};

#endif // MAINWINDOW_H
