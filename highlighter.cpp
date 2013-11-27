#include <QtGui>
#include <QtCore>
#include "highlighter.h"
#include "ui_mainwindow.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns<<"\\bFalse\\b"<<"\\bclass\\b"<<"\\bfinally\\b"<<"\\bis\\b"<<"\\breturn\\b"
    <<"\\bNone\\b"<<"\\bcontinue\\b"<<"\\bfor\\b"<<"\\blambda\\b"<<"\\btry\\b"
    <<"\\bTrue\\b"<<"\\bdef\\b"<<"\\bfrom\\b"<<"\\bnonlocal\\b"<<"\\bwhile\\b"
    <<"\\band\\b"<<"\\bdel\\b"<<"\\bglobal\\b"<<"\\bnot\\b"<<"\\bwith\\b"
    <<"\\bas\\b"<<"\\belif\\b"<<"\\bif\\b"<<"\\bor\\b"<<"\\byield\\b"
    <<"\\bassert\\b"<<"\\belse\\b"<<"\\bimport\\b"<<"\\bpass\\b"
    <<"\\bbreak\\b"<<"\\bexcept\\b"<<"\\bin\\b"<<"\\braise\\b";
    foreach (QString pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    singleLineCommentFormat.setForeground(Qt::gray);
    quotationFormat.setForeground(Qt::red);
}
void Highlighter::highlightBlock(const QString &text)
{
    foreach (HighlightingRule rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }
    QVector<lexeme> rules = lexAutomat(text);
    foreach(lexeme rule, rules)
    {
        setFormat(rule.pos_, rule.len_, rule.format);
    }
}
QVector<lexeme> Highlighter::lexAutomat(const QString& text)
{
    unsigned int state = 0; // 0 - нач сост, 1 - состояние комментария, 2 - состояние строки, 3 - состояние спецсимвола
    QVector<lexeme> result;
    lexeme temp;
    QChar str;
    QRegExp expression = QRegExp("#[^\n]*");
    for(int i = 0; i < text.length(); i++)
    {
        switch(state)
        {
        case 0:
            temp.pos_ = i;
            if(text[i] == '\"' || text[i] == '\'')
            {
                str = text[i];
                temp.format = quotationFormat;
                result.push_back(temp);
                state = 2;
            }
            else if(text[i] == '#')
            {
                temp.format = singleLineCommentFormat;
                result.push_back(temp);
                state = 1;
            }
            else
                state = 0;
           break;
        case 1:
            temp.len_++;
            temp.format = singleLineCommentFormat;
            text.indexOf(expression,temp.pos_);
            temp.len_ = expression.matchedLength();
            result.push_back(temp);
            if(text[i] == '\n')
            {
                state = 0;
                temp.pos_ = 0;
                temp.len_ = 1;
            }
            break;
        case 2:
            temp.len_++;
            temp.format = quotationFormat;
            setFormat(i, 1, quotationFormat);
            if(text[i] == '\\')
               state = 3;
            if(text[i] == str)
            {
                result.push_back(temp);
                state = 0;
                temp.pos_ = 0;
                temp.len_ = 1;
            }
            break;
        case 3:
            setFormat(i, 1, quotationFormat);
            temp.len_++;
            state = 2;
            break;
        default:
            /// error
            break;
        }
    }
    return result;
}
