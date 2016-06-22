#ifndef JSONAPIRESOURCEOBJECT_H
#define JSONAPIRESOURCEOBJECT_H

#include <QDebug>
#include <QJsonObject>
#include <QMultiMap>

class JsonApiResourceObject
{
public:
    explicit JsonApiResourceObject(const QJsonObject& data);
    JsonApiResourceObject(int id,
                          const QString& type,
                          const QJsonObject& attributes = QJsonObject(),
                          const QMultiMap<QString, JsonApiResourceObject>& relationships = QMultiMap<QString, JsonApiResourceObject>(),
                          const QJsonObject& meta = QJsonObject());

    QJsonObject toJsonObject() const;
    QList<JsonApiResourceObject> includedResources() const;

    bool isNull() const;
    QMultiMap<QString, JsonApiResourceObject>& relationships();
    QMultiMap<QString, JsonApiResourceObject> relationships() const;
    int id() const;
    QString type() const;
    QJsonObject attributes() const;
    QJsonObject meta() const;

    void setAttributes(const QJsonObject &attributes);
    void setRelationships(const QMultiMap<QString, JsonApiResourceObject> &relationships);
    void setMeta(const QJsonObject &meta);

private:
    bool m_isNull;
    int m_id;
    QString m_type;
    QJsonObject m_attributes;
    QMultiMap<QString, JsonApiResourceObject> m_relationships;
    QJsonObject m_meta;
};

bool operator==(const JsonApiResourceObject& first, const JsonApiResourceObject& second);
QDebug operator<<(QDebug dbg, const JsonApiResourceObject& obj);

#endif // JSONAPIRESOURCEOBJECT_H
