#ifndef DUJSONAPITEST_H
#define DUJSONAPITEST_H

#include <QObject>

class DuJsonApiTest : public QObject
{
    Q_OBJECT
public:
    DuJsonApiTest();

private slots:
    void testJsonApiEmptyDocument();
    void testJsonApiEmptyResource();
    void testJsonApiEmptyError();

    void testJsonApiDocument();
    void testJsonApiDocumentError();
};

#endif // DUJSONAPITEST_H
