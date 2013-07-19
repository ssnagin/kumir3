#ifndef SYNTAXANALIZER_H
#define SYNTAXANALIZER_H

#include <QtCore>
#include "statement.h"

#include "analizer.h"
#include "dataformats/ast.h"
#include "dataformats/ast_algorhitm.h"
#include "interfaces/analizerinterface.h"
#include "lexer.h"

typedef AST::Data AST_Data;
typedef AST::Algorithm AST_Algorhitm;

namespace KumirAnalizer {

using namespace AST;
using namespace Shared;

struct VariablesGroup {
    Lexem * groupLexem;
    QList<Lexem*> lexems;
    AST::VariableAccessType access;
    bool accessDefined;
};

struct SubexpressionElement {
    inline SubexpressionElement(AST::ExpressionPtr ee) { e = ee; o = 0; }
    inline SubexpressionElement(Lexem * oo) { o = oo; }
    AST::ExpressionPtr e;
    Lexem * o;
};


class SyntaxAnalizer : public QObject
{
    Q_OBJECT
public:
    explicit SyntaxAnalizer(class Lexer * lexer_,
                            const QStringList & alwaysEnabledModules_,
                            bool teacherMode,
                            Analizer *analizer);
    void init(QList<TextStatementPtr> & statements_
              , AST::DataPtr  ast_);
    QStringList unresolvedImports() const;
    void setSourceDirName(const QString & dirName);
    void buildTables(bool allowOperatorsDeclaration);
    QList<Shared::Suggestion> suggestAutoComplete(
            int lineNo,
            const TextStatementPtr statementBefore,
            const QList<Lexem*> lexemsAfter,
            const AST::ModulePtr contextModule,
            const AST::AlgorithmPtr contextAlgorithm
            ) const;
    void processAnalisys();
    ~SyntaxAnalizer();
private /*fields*/:

    Lexer * lexer_;
    Analizer * analizer_;
    AST::DataPtr ast_;
    AST::AlgorithmPtr algorhitm_;
    QList<TextStatement> statements_;
    QSet<QString> unresolvedImports_;
    QStringList alwaysEnabledModules_;
    QString sourceDirName_;
    int currentPosition_;
    bool teacherMode_;

private /*methods*/:
    void parseImport(int str);
    void parseModuleHeader(int str);
    void parseAlgHeader(int str, bool onlyName, bool allowOperatorsDeclaration);
    void parseVarDecl(int str);
    void parseAssignment(int str);
    void parseInput(int str);
    void parseAssertPrePost(int str);
    void parseOutput(int str);
    void parseOneLexemInstruction(int str);
    void parseEndNamedBlock(TextStatement & st);
    void parseEndLoop(int str);
    void parseIfCase(int str);
    void parseLoopBegin(int str);
    bool checkWrongDSUsage(AST::ExpressionPtr expression);
    AST::ModulePtr loadKodFile(const QString & fileName, QString &localError);

    const TextStatement & findSourceStatementByLexem(const Lexem* lexem) const;

    QList<Shared::Suggestion> suggestAssignmentAutoComplete(
            int lineNo,
            const TextStatementPtr statementBefore,
            const QList<Lexem *> lexemsAfter,
            const AST::ModulePtr contextModule,
            const AST::AlgorithmPtr contextAlgorithm
            ) const;

    QList<Shared::Suggestion> suggestInputOutputAutoComplete(
            int lineNo,
            const TextStatementPtr statementBefore,
            const QList<Lexem *> lexemsAfter,
            const AST::ModulePtr contextModule,
            const AST::AlgorithmPtr contextAlgorithm
            ) const;

    QList<Shared::Suggestion> suggestConditionAutoComplete(
            int lineNo,
            const TextStatementPtr statementBefore,
            const QList<Lexem *> lexemsAfter,
            const AST::ModulePtr contextModule,
            const AST::AlgorithmPtr contextAlgorithm
            ) const;

    QList<Shared::Suggestion> suggestLoopBeginAutoComplete(
            int lineNo,
            const TextStatementPtr statementBefore,
            const QList<Lexem *> lexemsAfter,
            const AST::ModulePtr contextModule,
            const AST::AlgorithmPtr contextAlgorithm
            ) const;

    QList<Shared::Suggestion> suggestImportAutoComplete(
            int lineNo,
            const TextStatementPtr statementBefore,
            const QList<Lexem *> lexemsAfter
            ) const;

    QList<Shared::Suggestion> suggestExpressionAutoComplete(
            int lineNo,
            const QList<Lexem*> lexemsBefore,
            const QList<Lexem*> lexemsAfter,
            const AST::ModulePtr contextModule,
            const AST::AlgorithmPtr contextAlgorithm,
            bool typeIsKnown,
            AST::Type baseType,
            unsigned int dimension,
            AST::VariableAccessType accessType,
            AST::ExpressionType expressionKind
            ) const;

    QList<Shared::Suggestion> suggestValueAutoComplete(
            int lineNo,
            const QList<Lexem*> lexemsBefore,
            const QList<Lexem*> lexemsAfter,
            const AST::ModulePtr contextModule,
            const AST::AlgorithmPtr contextAlgorithm,
            bool typeIsKnown,
            AST::Type baseType,
            unsigned int minimumDimension,
            AST::VariableAccessType accessType
            ) const;

    QList<Shared::Suggestion> suggestOperandAutoComplete(
            int lineNo,
            const QList<Lexem*> lexemsBefore,
            const QList<Lexem*> lexemsAfter,
            const AST::ModulePtr contextModule,
            const AST::AlgorithmPtr contextAlgorithm,
            const AST::ExpressionPtr leftExpression,
            const LexemType operatorr,
            const AST::Type & targetBaseType
            ) const;

    bool tryInputOperatorAlgorithm(
            const QString & lexem,
            AST::Type & type,
            QVariant & constantValue
            , const AST::ModulePtr currentModule
            ) const;
    bool findConversionAlgorithm(const AST::Type & from
                                 , const AST::Type & to
                                 , AST::ModulePtr & mod
                                 , AST::AlgorithmPtr & alg
                                 , const AST::ModulePtr currentModule
                                 ) const;
    AST::ExpressionPtr makeCustomBinaryOperation(const QString & operatorName
                                                 , AST::ExpressionPtr leftExpression
                                                 , AST::ExpressionPtr rightExpression
                                                 , const AST::ModulePtr currentModule
                                                 ) const;
    template <typename TOut>
    TOut makeCustomUnaryOperation(const QString & operatorName
                                  , AST::ExpressionPtr argument
                                  , const AST::ModulePtr currentModule
                                  ) const;
    bool findAlgorhitm(const QString &name
                       , const AST::ModulePtr currentModule
                       , const AST::AlgorithmPtr currentAlgorithm
                       , AST::AlgorithmPtr & algorhitm
                       ) const;
    bool findGlobalVariable(const QString &name, const AST::ModulePtr module, AST::VariablePtr & var) const;
    bool findLocalVariable(const QString &name
                           , const AST::AlgorithmPtr alg
                           , AST::VariablePtr & var) const;
    AST::ExpressionPtr parseExpression(QList<Lexem*> lexems
                                       , const AST::ModulePtr mod
                                       , const AST::AlgorithmPtr alg) const;
    bool findVariable(const QString &name
                      , const AST::ModulePtr module
                      , const AST::AlgorithmPtr algorhitm
                      , AST::VariablePtr & var) const;
    bool findUserType(const QString & name, AST::Type &type, AST::ModulePtr module, const AST::ModulePtr currentModule) const;
    QList<AST::VariablePtr> parseVariables(int statementIndex, VariablesGroup & group,
                                           AST::ModulePtr mod,
                                           AST::AlgorithmPtr alg, bool algHeader);
    QVariant parseConstant(const std::list<Lexem*> &constant
                           , const AST::VariableBaseType pt
                           , int& maxDim
                           ) const;
    AST::VariableBaseType testConst(const std::list<Lexem *> & lxs, int &err) const;
    QVariant createConstValue(const QString &str, const AST::VariableBaseType type) const;
    AST::ExpressionPtr parseFunctionCall(const QList<Lexem*> &lexems, const AST::ModulePtr mod, const AST::AlgorithmPtr alg) const;

    AST::ExpressionPtr parseSimpleName(const std::list<Lexem*> &lexems, const AST::ModulePtr mod, const AST::AlgorithmPtr alg) const;
    void updateSliceDSCall(AST::ExpressionPtr expr, AST::VariablePtr var) const;
    AST::ExpressionPtr parseElementAccess(const QList<Lexem*> &lexems, const AST::ModulePtr mod, const AST::AlgorithmPtr alg) const;
    AST::ExpressionPtr makeExpressionTree(const QList<SubexpressionElement> & s, const AST::ModulePtr currentModule) const;
    template <typename List1, typename List2>
    inline static void splitLexemsByOperator(
            const List1 &s
            , const LexemType & op
            , List2 & result
            , List1 & comas
            )
    {
        result.clear();
        comas.clear();
        int deep = 0;
        if (s.size()>0)
            result.push_back(List1());
        typename List1::const_iterator it;
        for (it=s.begin(); it!=s.end(); ++it) {
            if ( (*it)->type==op && deep==0 ) {
                result.push_back(List1());
                comas.push_back((*it));
            }
            else {
                if ( (*it)->type==LxOperLeftBr || (*it)->type==LxOperLeftSqBr || (*it)->type==LxOperLeftBrace )
                    deep++;
                else if ( (*it)->type==LxOperRightBr || (*it)->type==LxOperRightSqBr || (*it)->type==LxOperRightBrace )
                    deep--;
                result.back().push_back(*it);
            }
        }
    }
    static Lexem * findLexemByType(const QList<Lexem*> lxs, LexemType type);

};

} // namespace KumirAnalizer

#endif // SYNTAXANALIZER_H
