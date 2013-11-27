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
struct lexeme
{
    unsigned int pos_, len_;
    QTextCharFormat format;
    lexeme()
    {
        pos_ = 0;
        len_ = 1;
    }
};

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Highlighter(QTextDocument *parent = 0);
protected:
    void highlightBlock(const QString &text);
private:
    QVector<lexeme> lexAutomat(const QString& text);

    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat quotationFormat;
};

#endif
