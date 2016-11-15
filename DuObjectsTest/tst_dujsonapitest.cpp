#include "tst_dujsonapitest.h"

#include <QtTest>

#include <jsonApi/JsonApiDocument.h>

DuJsonApiTest::DuJsonApiTest()
    : QObject()
{
}

void DuJsonApiTest::testJsonApiEmptyDocument()
{
    QJsonDocument doc;
    JsonApiDocument apiDoc(doc);
    QVERIFY(apiDoc.isNull());
    QCOMPARE(apiDoc.toJsonDocument(), doc);
}

void DuJsonApiTest::testJsonApiEmptyResource()
{
    QJsonObject obj;
    JsonApiResourceObject res(obj);
    QVERIFY(res.isNull());
    QCOMPARE(res.toJsonObject(), obj);
}

void DuJsonApiTest::testJsonApiEmptyError()
{
    QJsonObject obj;
    JsonApiErrorObject res(obj);
    QVERIFY(res.isNull());
    QCOMPARE(res.toJsonObject(), obj);
}

void DuJsonApiTest::testJsonApiDocument()
{
    QJsonDocument doc = QJsonDocument::fromJson(
        "{"
        "  \"data\": [{"
        "    \"type\": \"articles\","
        "    \"id\": 1,"
        "    \"attributes\": {"
        "      \"title\": \"JSON API paints my bikeshed!\""
        "    },"
        "    \"relationships\": {"
        "      \"author\": {"
        "        \"data\": { \"type\": \"people\", \"id\": 9 }"
        "      },"
        "      \"comments\": {"
        "        \"data\": ["
        "          { \"type\": \"comments\", \"id\": 5 },"
        "          { \"type\": \"comments\", \"id\": 12 }"
        "        ]"
        "      }"
        "    }"
        "  }],"
        "  \"included\": [{"
        "    \"type\": \"people\","
        "    \"id\": 9,"
        "    \"attributes\": {"
        "      \"first-name\": \"Dan\","
        "      \"last-name\": \"Gebhardt\","
        "      \"twitter\": \"dgeb\""
        "    }"
        "  }, {"
        "    \"type\": \"comments\","
        "    \"id\": 5,"
        "    \"attributes\": {"
        "      \"body\": \"First!\""
        "    },"
        "    \"relationships\": {"
        "      \"author\": {"
        "        \"data\": { \"type\": \"people\", \"id\": 2 }"
        "      }"
        "    }"
        "  }, {"
        "    \"type\": \"comments\","
        "    \"id\": 12,"
        "    \"attributes\": {"
        "      \"body\": \"I like XML better\""
        "    },"
        "    \"relationships\": {"
        "      \"author\": {"
        "        \"data\": { \"type\": \"people\", \"id\": 9 }"
        "      }"
        "    }"
        "  }]"
        "}"
    );
    JsonApiDocument apiDoc(doc);
    QVERIFY(!apiDoc.isNull());
    QVERIFY(!apiDoc.isError());
    QVERIFY(!apiDoc.toJsonDocument().isEmpty());
    QVERIFY(apiDoc.toJsonDocument().isObject());
    QCOMPARE(apiDoc.toJsonDocument().toJson(QJsonDocument::Compact).size(), doc.toJson(QJsonDocument::Compact).size());

    QJsonObject people9attributes;
    people9attributes.insert("first-name", "Dan");
    people9attributes.insert("last-name", "Gebhardt");
    people9attributes.insert("twitter", "dgeb");
    JsonApiResourceObject people9Resource(9, "people", people9attributes);

    QJsonObject comments5attributes;
    comments5attributes.insert("body", "First!");
    QMultiMap<QString, JsonApiResourceObject> comments5relationships;
    comments5relationships.insert("author", JsonApiResourceObject(2, "people"));
    JsonApiResourceObject comments5Resource(5, "comments", comments5attributes, comments5relationships);

    QJsonObject comments12attributes;
    comments12attributes.insert("body", "I like XML better");
    QMultiMap<QString, JsonApiResourceObject> comments12relationships;
    comments12relationships.insert("author", JsonApiResourceObject(9, "people"));
    JsonApiResourceObject comments12Resource(12, "comments", comments12attributes, comments12relationships);

    QJsonObject attributes;
    attributes.insert("title", "JSON API paints my bikeshed!");
    QMultiMap<QString, JsonApiResourceObject> relationships;
    relationships.insert("author", people9Resource);
    relationships.insert("comments", comments5Resource);
    relationships.insert("comments", comments12Resource);
    JsonApiResourceObject primaryData(1, "articles", attributes, relationships);
    JsonApiDocument homemadeApiDoc(QList<JsonApiResourceObject>() << primaryData);

    QCOMPARE(QString(apiDoc.toJsonDocument().toJson(QJsonDocument::Compact)), QString(homemadeApiDoc.toJsonDocument().toJson(QJsonDocument::Compact)));
    QCOMPARE(apiDoc, homemadeApiDoc);
}

void DuJsonApiTest::testJsonApiDocumentError()
{
    QJsonDocument doc = QJsonDocument::fromJson(
        "{"
        "  \"errors\": ["
        "    {"
        "      \"id\":     1564684,"
        "      \"status\": 200,"
        "      \"code\":   123,"
        "      \"title\":  \"Value is too short\","
        "      \"detail\": \"First name must contain at least three characters.\""
        "    },"
        "    {"
        "      \"id\":     489494,"
        "      \"status\": 404,"
        "      \"code\":   225,"
        "      \"title\": \"Passwords must contain a letter, number, and punctuation character.\","
        "      \"detail\": \"The password provided is missing a punctuation character.\""
        "    },"
        "    {"
        "      \"id\":     3789648,"
        "      \"status\": 500,"
        "      \"code\":   226,"
        "      \"title\": \"Password and password confirmation do not match.\","
        "      \"detail\": \"They have to match !\""
        "    }"
        "  ]"
        "}"
    );
    JsonApiDocument apiDoc(doc);
    QVERIFY(!apiDoc.isNull());
    QVERIFY(apiDoc.isError());
    QVERIFY(!apiDoc.toJsonDocument().isEmpty());
    QVERIFY(apiDoc.toJsonDocument().isObject());
    QCOMPARE(QString(apiDoc.toJsonDocument().toJson(QJsonDocument::Compact)), QString(doc.toJson(QJsonDocument::Compact)));

    JsonApiErrorObject error1(1564684, 200, 123, "Value is too short", "First name must contain at least three characters.");
    JsonApiErrorObject error2(489494, 404, 225, "Passwords must contain a letter, number, and punctuation character.", "The password provided is missing a punctuation character.");
    JsonApiErrorObject error3(3789648, 500, 226, "Password and password confirmation do not match.", "They have to match !");
    JsonApiDocument homemadeApiDoc(QList<JsonApiErrorObject>() << error1 << error2 << error3);

    QCOMPARE(QString(apiDoc.toJsonDocument().toJson(QJsonDocument::Compact)), QString(homemadeApiDoc.toJsonDocument().toJson(QJsonDocument::Compact)));
    QCOMPARE(apiDoc, homemadeApiDoc);
}
