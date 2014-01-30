#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "runner.h"

#include "kumirinterface.h"
#include <QtCore>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    runner_(0)
{
    ui->setupUi(this);
#ifdef Q_WS_X11
    ui->actionOpen->setIcon (QIcon::fromTheme ("document-open"));
    ui->actionSave->setIcon (QIcon::fromTheme ("document-save"));
    ui->actionCopy->setIcon (QIcon::fromTheme ("edit-copy"));
    ui->actionPaste->setIcon (QIcon::fromTheme ("edit-paste"));
    ui->actionCut->setIcon (QIcon::fromTheme ("edit-cut"));
    ui->actionRedo->setIcon (QIcon::fromTheme ("edit-redo"));
    ui->actionUndo->setIcon (QIcon::fromTheme ("edit-undo"));
#endif

#ifdef Q_WS_WIN32
    ui->actionOpen->setIcon (QIcon(QApplication::applicationDirPath () + "/document-open.png"));
    ui->actionSave->setIcon (QIcon(QApplication::applicationDirPath () + "/document-save"));
    ui->actionCopy->setIcon (QIcon(QApplication::applicationDirPath () + "/edit-copy"));
    ui->actionPaste->setIcon (QIcon(QApplication::applicationDirPath () + "/edit-paste"));
    ui->actionCut->setIcon (QIcon(QApplication::applicationDirPath () + "/edit-cut"));
    ui->actionRedo->setIcon (QIcon(QApplication::applicationDirPath () + "/edit-redo"));
    ui->actionUndo->setIcon (QIcon(QApplication::applicationDirPath ()+ "/edit-undo"));
#endif
    connect(ui->actionSave, SIGNAL(triggered()),
            this, SLOT(fileSave()));
    connect(ui->actionOpen, SIGNAL(triggered()),
            this, SLOT(fileOpen()));
    connect(ui->actionRun, SIGNAL(triggered()),
            this, SLOT(Run()));


    runner_ = new Runner(this);

    highlighter = new Highlighter(ui->textEdit->document ());
    cursor_ = new QTextCursor();

    cur = new QTextCursor();

    connect(runner_, SIGNAL(doOutput(QString)),
            this , SLOT(output(QString)));
    connect(runner_, SIGNAL(doErrOutput(QString)),
            this , SLOT(errOutput(QString)));
    connect(runner_,SIGNAL(finished()),
            this,SLOT(reEnabled()));
    connect(ui->actionStop,SIGNAL(triggered()),
            this, SLOT(Stop()));
    readSettings();
    ui->textEdit->setFocusPolicy(Qt::StrongFocus);
    ui->textEdit->setTabStopWidth(28); //ширина 4-х пробелов(чтобы курсор не мелькал когда заменяется на пробелы)
    ui->textOutput->setTabStopWidth(28);




    QString path = QCoreApplication::applicationDirPath()+"/../TextEditor/kumir-libs/plugins";
    path = QDir::cleanPath(path);
    path = QDir::toNativeSeparators(path);
    k = new kumirInterface(path,this);

}
void MainWindow::closeEvent(QCloseEvent *event)
{
        writeSettings();
        event->accept();
}
void MainWindow::readSettings()
{
    QSettings settings("Settings", "TextEditor");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}
void MainWindow::writeSettings()
{
    QSettings settings("Settings", "TextEditor");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

void MainWindow::fileSave ()
{
    QString filter;
    filter = tr("Python files (*.py)");
    QString a;
    QString fn = QFileDialog::getSaveFileName (this, tr("Select file name"),
                                  "", filter,&a);
    QString sufTxt = ".py";
    QRegExp suffix (".*" + sufTxt);
    if(a.compare (filter) == 0)
    {
        if(suffix.indexIn (fn) == -1 && fn.length ()>0)
        {
            fn += sufTxt;
        }
    }
    if (fn.length () > 0) {
        QFile f(fn);

        if (f.open (QIODevice::WriteOnly)) {
            QTextStream ts(&f);
            ts.setCodec ("UTF-8"); // UTF-8, WINDOWS-1251, OEM-866, KOI8-R
            ts << ui->textEdit->toPlainText ();
            f.close ();
        }
        else {
            /// error
        }
    }
}
void MainWindow::fileOpen ()
{
    const QString fn = QFileDialog::getOpenFileName (this,tr("Set file name"),"",tr("Python files (*.py)"));
    if(fn.length ()>0)
    {
        QFile f(fn);
        if(f.open (QIODevice::ReadOnly))
        {
            QTextStream ts(&f);
            ts.setCodec ("UTF-8");
            ui->textEdit->setText (ts.readAll ());
            f.close ();
        }
        else{
            /// error
        }
    }
}

void MainWindow::Run()
{

    ui->actionStop->setEnabled (true);
    ui->textEdit->setEnabled (false);
    undoAv = ui->actionUndo->isEnabled ();
    redoAv = ui->actionRedo->isEnabled ();
    QTextCursor cursor = ui->textEdit->textCursor();
    *cursor_ = cursor;
    cursor.clearSelection();
    ui->textEdit->setTextCursor(cursor);
    ui->actionPaste->setEnabled (false);

    ui->actionUndo->setEnabled (false);
    ui->actionRedo->setEnabled (false);
    ui->actionRun->setEnabled (false);
    ui->actionOpen->setEnabled (false);

    ui->textOutput->setText ("");
    runner_->init(ui->textEdit->toPlainText ());
    runner_->start ();
}

void MainWindow::output (const QString &text)
{
    QTextCursor cursor = ui->textOutput->textCursor();
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat fmt = cursor.charFormat();
    fmt.setForeground(QColor(Qt::black));
    cursor.setCharFormat(fmt);
    cursor.insertText(text);
}

void MainWindow::errOutput (const QString &text)
{
    QTextCursor cursor = ui->textOutput->textCursor();
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat fmt = cursor.charFormat();
    fmt.setForeground(QColor(Qt::red));
    cursor.setCharFormat(fmt);
    cursor.insertText(text);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::reEnabled()
{
    ui->actionStop->setEnabled (false);
    ui->textEdit->setTextCursor(*cursor_);
    ui->textEdit->setEnabled (true);
    ui->actionRun->setEnabled (true);
    ui->actionOpen->setEnabled (true);
    ui->actionUndo->setEnabled (undoAv);
    ui->actionRedo->setEnabled (redoAv);
    ui->actionPaste->setEnabled (true);
}

void MainWindow::Stop ()
{
    runner_->stop ();
}











