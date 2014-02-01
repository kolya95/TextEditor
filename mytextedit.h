#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QTextEdit>
class MyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyTextEdit(QWidget *parent = 0);

    void keyReleaseEvent(QKeyEvent *e);
    void keyPressEvent(QKeyEvent *e);
//**********************************
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event);

public Q_SLOTS:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);


    void ShiftTabSequency();


private:

    QWidget *lineNumberArea;
};



 class LineNumberArea : public QWidget
 {
 public:
     LineNumberArea(MyTextEdit *editor) : QWidget(editor) {
         codeEditor = editor;
     }

     QSize sizeHint() const {
         return QSize(codeEditor->lineNumberAreaWidth(), 0);
     }

 protected:
     void paintEvent(QPaintEvent *event) {
         codeEditor->lineNumberAreaPaintEvent(event);
     }

 private:
     MyTextEdit *codeEditor;
 };
#endif // MYTEXTEDIT_H
