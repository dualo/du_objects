#ifndef JSONAPIDOCUMENT_H
#define JSONAPIDOCUMENT_H

#include "JsonApiErrorObject.h"
#include "JsonApiResourceObject.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

class JsonApiDocument
{
    Q_DECLARE_TR_FUNCTIONS(JsonApiDocument)

public:
    explicit JsonApiDocument(const QJsonDocument& jsonDoc);
    explicit JsonApiDocument(const QList<JsonApiResourceObject>& primaryData, const QJsonObject& meta = QJsonObject());
    explicit JsonApiDocument(const QList<JsonApiErrorObject>& errors, const QJsonObject& meta = QJsonObject());

    QJsonDocument toJsonDocument() const;

    bool isNull() const;
    bool isError() const;

    QList<JsonApiResourceObject> primaryData() const;
    QList<JsonApiErrorObject> errors() const;
    QJsonObject meta() const;

    QString errorStr() const;

private:
    bool m_isNull;
    bool m_isError;

    QList<JsonApiResourceObject> m_primaryData;
    QList<JsonApiErrorObject> m_errors;
    QJsonObject m_meta;
};

bool operator==(const JsonApiDocument& first, const JsonApiDocument& second);
QDebug operator<<(QDebug dbg, const JsonApiDocument& obj);

#endif // JSONAPIDOCUMENT_H
