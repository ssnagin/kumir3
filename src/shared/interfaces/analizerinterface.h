#ifndef ANALIZER_INTERFACE_H
#define ANALIZER_INTERFACE_H

#include "error.h"
#include "lineprop.h"
#include "abstractsyntaxtree/ast.h"

namespace Shared {

class AnalizerInterface {
public:
    virtual int newDocument() = 0;
    virtual void dropDocument(int documentId) = 0;
    virtual void setSourceText(int documentId, const QString &text) = 0;
    virtual void changeSourceText(int documentId, int pos, int len, const QString &repl) = 0;
    virtual QList<Error> errors(int documentId) const = 0;
    virtual QList<LineProp> lineProperties(int documentId) const = 0;
    virtual QList<QPoint> lineRanks(int documentId) const = 0;
    virtual QStringList imports(int documentId) const = 0;
    virtual const AST::Data * abstractSyntaxTree(int documentId) const = 0;
};

}

#endif
