#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>
struct HighlightingRule
{
    QRegExp pattern;
    QTextCharFormat format;
};

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Highlighter(QTextDocument *parent = 0);
protected:
    void highlightBlock(const QString &text);
private:

    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat keywordFormat;
};

#endif
