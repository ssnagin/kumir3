#ifndef LEXER_H
#define LEXER_H

#include "interfaces/lineprop.h"
#include "statement.h"
#include "interfaces/error.h"
#include "dataformats/ast_variabletype.h"

#include <QtCore>

namespace KumirAnalizer {

class Lexer : public QObject
{
    Q_OBJECT
public:
    explicit Lexer(QObject *parent = 0);
    static void setLanguage(const QLocale::Language & language);
    QString testName( const QString &stroka );
    AST::VariableBaseType baseTypeByClassName(const QString &clazz) const;
    QString classNameByBaseType(const AST::VariableBaseType &type) const;
    bool isArrayClassName(const QString &clazz) const;
    bool boolConstantValue(const QString &val) const;
    bool isReturnVariable(const QString & name) const;
public slots:
    int splitIntoStatements(const QStringList &lines
                             , int baseLineNo
                             , QList<Statement*> &statements
                             ) const;
    void splitIntoLexems(const QString & text, QList<Lexem*> & lexems);
private:
    class LexerPrivate * d;

};

}

#endif // LEXER_H
