#include "mytextedit.h"
#include "mainwindow.h"
MyTextEdit::MyTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    QTextEdit::setWordWrapMode(QTextOption::NoWrap);
}
void MyTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Tab||e->key()==Qt::Key_Backspace||e->key()==Qt::Key_Backtab||(e->key() == Qt::Key_Tab && e->modifiers() & Qt::ShiftModifier) )
    {
        if(e->key() == Qt::Key_Backtab||(e->key() == Qt::Key_Tab && e->modifiers() & Qt::ShiftModifier))
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
            cur->setPosition(index/*+1*/);
            qDebug()<<cur->selectionStart()<<cur->selectionEnd();
            cur->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,count2+count);
            this->setTextCursor(*cur);
        }
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
