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
};



#endif // MYTEXTEDIT_H
