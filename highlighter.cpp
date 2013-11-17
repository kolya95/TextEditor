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
    rule.pattern = QRegExp("#[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\'.*\'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);


    quotation = QRegExp("\"");
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


    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(quotation);
    while (startIndex >= 0) {
        int endIndex = text.indexOf(quotation, startIndex);
        int Length;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            Length = text.length() - startIndex;
        }
        else {
             Length = endIndex - startIndex
             + quotation.matchedLength();
        }
        setFormat(startIndex, Length, quotationFormat);
        startIndex = text.indexOf(quotation,
                                           startIndex + Length);
    }
}
