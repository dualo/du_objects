#ifndef JSONAPIERROROBJECT_H
#define JSONAPIERROROBJECT_H

#include <QCoreApplication>
#include <QDebug>
#include <QJsonObject>

class JsonApiErrorObject
{
    Q_DECLARE_TR_FUNCTIONS(JsonApiErrorObject)

public:
    explicit JsonApiErrorObject(const QJsonObject& data);
    JsonApiErrorObject(int id,
                       int status,
                       int code,
                       const QString& title,
                       const QString& detail,
                       const QJsonObject& meta = QJsonObject());

    QJsonObject toJsonObject() const;

    bool isNull() const;
    int id() const;
    int status() const;
    int code() const;
    QString title() const;
    QString detail() const;
    QJsonObject meta() const;

    QString toString() const;

private:
    bool m_isNull;
    int m_id;
    int m_status;
    int m_code;
    QString m_title;
    QString m_detail;
    QJsonObject m_meta;
};

bool operator==(const JsonApiErrorObject& first, const JsonApiErrorObject& second);
QDebug operator<<(QDebug dbg, const JsonApiErrorObject& obj);

#endif // JSONAPIERROROBJECT_H
