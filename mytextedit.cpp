#include "mytextedit.h"
#include "mainwindow.h"
MyTextEdit::MyTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    QTextEdit::setWordWrapMode(QTextOption::NoWrap);

    //*********************


    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));



    QAction* myShortcut = new QAction(this);
    myShortcut->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Tab));

    connect(myShortcut, SIGNAL(triggered()), this, SLOT(ShitTabSequency()));
    addAction(myShortcut);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

}
void MyTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Tab||e->key()==Qt::Key_Backspace)
    {


    }
    else
        QTextEdit::keyPressEvent(e);
}

void MyTextEdit::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Return||e->key()==Qt::Key_Enter||e->key()==Qt::Key_Backspace||e->key()==Qt::Key_Tab)
    {
        QTextCursor *cur = new QTextCursor;
        *cur = this->textCursor();
        if(cur->position()>1)
        {
            if ((e->key() == Qt::Key_Return) || (e->key() == Qt::Key_Enter))
            {
                int indent_ = 0;
                QString txt = toPlainText();
                if(txt[cur->position()-2]==':')
                {
                    indent_+=4;
                }
                QString line;
                int i = cur->position()-2;
                while(txt[i]!='\n')
                {
                    line.insert(0,txt[i]);
                    i--;
                    if(i<0)
                        break;
                }
                for(i = 0; i<line.length();i++)
                {
                    if(line[i]!=' ')
                        break;
                    indent_++;
                }
                for(i = 0; i<indent_/4; i++)
                {
                    cur->insertText("    ");
                }
                return;
            }
        }
        if(e->key() == Qt::Key_Tab)
        {

            if(cur->selectionStart()==cur->selectionEnd())
                cur->insertText("    ");
            else
            {
                QString txt = toPlainText();
                QTextCursor* c = new QTextCursor(*cur);
                c->setPosition(cur->selectionStart());
                qDebug()<<c->position();
                int index = cur->selectionStart();
                int count = 0;
                while(index>0&&txt[index]!=QChar::ParagraphSeparator&&txt[index]!='\n')
                {
                    index--;
                    count++;
                }
                c->setPosition(index);
                qDebug()<<c->position();
                c->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor, count+cur->selectedText().size());
                count+=cur->selectedText().size();
                int count2 = 0;

                while(c->selectionEnd()<txt.size()&&txt[c->selectionEnd()]!=QChar::ParagraphSeparator&&txt[c->selectionEnd()]!='\n')
                {
                    count++;
                    if(!c->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,1))
                    {
                        break;
                    }
                }
                QString t = c->selectedText();
                qDebug()<<t;
                if(t[0]!=QChar::ParagraphSeparator&&t[0]!='\n')
                {
                    count2+=4;
                    t.insert(0,"    ");
                }
                for(int i = 0; i < t.size(); i++)
                {
                    if(t[i]=='\n'||t[i]==QChar::ParagraphSeparator)
                    {
                        t.insert(i+1,"    ");
                        i+=4;
                        count2 +=4;
                    }
                }

                c->insertText(t);
                cur->setPosition(index);
                qDebug()<<cur->selectionStart()<<cur->selectionEnd();
                cur->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,count2+count);
                this->setTextCursor(*cur);
            }
        }

        if(e->key()==Qt::Key_Backspace)
        {
            bool spaces = true;
            int counter = 0;
            *cur = this->textCursor();
            QString text = this->toPlainText();
            if(cur->position()>0&&text[cur->position()-1]!=' ')
            {
                cur->deletePreviousChar();
                return;
            }
            text = toPlainText();
            *cur = textCursor();
            for(int k = cur->position();;k--)
            {
                if(k==0||text[k-1]=='\n')
                    break;
                counter++;
                if(text[k-1]!=' ')
                {
                    spaces = false;
                    break;
                }
            }
            if(spaces&&counter>3)
            {
                for(int k = 0; k<4; k++)
                    cur->deletePreviousChar();

            }
            else if(spaces)
                cur->deletePreviousChar();
            else
                cur->deletePreviousChar();

        }
    }
    else
    {
        return;
    }
    setCursorWidth(1);
}
void MyTextEdit::ShiftTabSequency()
{
    qDebug()<<"SHIFT+TAB";
    QString txt = toPlainText();
    QTextCursor* cur = new QTextCursor(this->textCursor());
    QTextCursor* c = new QTextCursor(*cur);
    c->setPosition(cur->selectionStart());
    qDebug()<<c->position();
    int index = cur->selectionStart();
    int count = 0;
    while(index>0&&txt[index]!=QChar::ParagraphSeparator&&txt[index]!='\n')
    {
        index--;
        count++;
    }
    c->setPosition(index);
    qDebug()<<c->position();
    c->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor, count+cur->selectedText().size());
    count+=cur->selectedText().size();
    int count2 = 0;

    while(c->selectionEnd()<txt.size()&&txt[c->selectionEnd()]!=QChar::ParagraphSeparator&&txt[c->selectionEnd()]!='\n')
    {
        count++;
        if(!c->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,1))
        {
            break;
        }
    }
    QString t = c->selectedText();
    qDebug()<<t;
    if(t[0]!=QChar::ParagraphSeparator&&t[0]!='\n')
    {
        for(int i = 0; i<4; i++)
        {
            if(t[0]!=' ')
            {
                break;
            }
            else
            {
                count2--;
                t.remove(0,1);
            }
        }
    }
    for(int i = 0; i < t.size(); i++)
    {
        if(t[i]=='\n'||t[i]==QChar::ParagraphSeparator)
        {
            for(int j = 0; j<4; j++)
            {
                if(t[i+1]==' ')
                {
                    t.remove(i+1,1);
                   count2--;
                }
                else break;
            }
        }
    }

    c->insertText(t);
    cur->setPosition(index+1);
    qDebug()<<cur->selectionStart()<<cur->selectionEnd();
    cur->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,count2+count);
    this->setTextCursor(*cur);

}

//*************


 int MyTextEdit::lineNumberAreaWidth()
 {
     int digits = 1;
     int max = qMax(1, this->document()->blockCount());
     while (max >= 10) {
         max /= 10;
         ++digits;
     }

     int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

     return space;
 }



 void MyTextEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
 {
     setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
 }



 void MyTextEdit::updateLineNumberArea(const QRect &rect, int dy)
 {
     if (dy)
         lineNumberArea->scroll(0, dy);
     else
         lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

     if (rect.contains(viewport()->rect()))
         updateLineNumberAreaWidth(0);
 }



 void MyTextEdit::resizeEvent(QResizeEvent *e)
 {
     QTextEdit::resizeEvent(e);

     QRect cr = contentsRect();
     lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}



 void MyTextEdit::highlightCurrentLine()
 {
     QList<QTextEdit::ExtraSelection> extraSelections;

     if (!isReadOnly()) {
         QTextEdit::ExtraSelection selection;

         QColor lineColor = QColor(Qt::yellow).lighter(160);

         selection.format.setBackground(lineColor);
         selection.format.setProperty(QTextFormat::FullWidthSelection, true);
         selection.cursor = textCursor();
         selection.cursor.clearSelection();
         extraSelections.append(selection);
     }

     setExtraSelections(extraSelections);
 }



 void MyTextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
 {
     QPainter painter(lineNumberArea);
     painter.fillRect(event->rect(), Qt::lightGray);


     QTextBlock block = this->document()->firstBlock();
     int blockNumber = block.blockNumber();

     int top = (int) 5;
     int bottom = top + (int) 16;

     while (block.isValid() && top <= event->rect().bottom()) {
         if (bottom >= event->rect().top())
         {
             QString number = QString::number(blockNumber + 1);
             painter.setPen(Qt::black);
             painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                              Qt::AlignRight, number);
         }

         block = block.next();
         top = bottom;
         bottom = top + (int) 16;
         ++blockNumber;
     }
 }
