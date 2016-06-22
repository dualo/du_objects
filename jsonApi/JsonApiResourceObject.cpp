#include "JsonApiResourceObject.h"

#include <QJsonArray>

JsonApiResourceObject::JsonApiResourceObject(const QJsonObject &data) :
    m_isNull(true),
    m_id(-1)
{
    if (!data.contains("id") || !data.contains("type"))
        return;

    // ID
    const QJsonValue& idValue = data.value("id");
    if (!idValue.isDouble())
        return;

    m_id = idValue.toInt();

    // TYPE
    const QJsonValue& typeValue = data.value("type");
    if (!typeValue.isString())
        return;

    m_type = typeValue.toString();

    // ATTRIBUTES
    if (data.contains("attributes"))
    {
        const QJsonValue& attributesValue = data.value("attributes");
        if (!attributesValue.isObject())
            return;

        m_attributes = attributesValue.toObject();
    }

    // RELATIONSHIPS
    if (data.contains("relationships"))
    {
        const QJsonValue& relationshipsValue = data.value("relationships");
        if (!relationshipsValue.isObject())
            return;

        const QJsonObject& relationshipsObject = relationshipsValue.toObject();
        for (QJsonObject::const_iterator it = relationshipsObject.constBegin(); it != relationshipsObject.constEnd(); ++it)
        {
            const QString& key = it.key();
            const QJsonValue& relationshipValue = it.value();
            if (!relationshipValue.isObject())
                return;

            const QJsonObject& relationshipObject = relationshipValue.toObject();
            if (!relationshipObject.contains("data"))
                return;

            const QJsonValue& relationshipDataValue = relationshipObject.value("data");
            if (relationshipDataValue.isObject())
            {
                JsonApiResourceObject resource(relationshipDataValue.toObject());
                if (resource.m_isNull)
                    return;

                m_relationships.insert(m_relationships.constEnd(), key, resource);
            }
            else if (relationshipDataValue.isArray())
            {
                const QJsonArray& relationshipDataArray = relationshipDataValue.toArray();
                for (QJsonArray::const_iterator aIt = relationshipDataArray.constBegin(); aIt != relationshipDataArray.constEnd(); ++aIt)
                {
                    const QJsonValue& v = *(aIt);
                    if (!v.isObject())
                        return;

                    JsonApiResourceObject resource(v.toObject());
                    if (resource.m_isNull)
                        return;

                    m_relationships.insert(key, resource);
                }
            }
            else
                return;
        }
    }

    // META
    if (data.contains("meta"))
    {
        const QJsonValue& metaValue = data.value("meta");
        if (!metaValue.isObject())
            return;

        m_meta = metaValue.toObject();
    }

    m_isNull = false;
}

JsonApiResourceObject::JsonApiResourceObject(int id,
                                             const QString &type,
                                             const QJsonObject &attributes,
                                             const QMultiMap<QString, JsonApiResourceObject> &relationships,
                                             const QJsonObject &meta) :
    m_isNull(id < 0 || type.isEmpty()),
    m_id(id),
    m_type(type),
    m_attributes(attributes),
    m_relationships(relationships),
    m_meta(meta)
{
}

QJsonObject JsonApiResourceObject::toJsonObject() const
{
    if (m_isNull)
        return QJsonObject();

    QJsonObject object;

    object.insert("type", m_type);
    object.insert("id", m_id);
    object.insert("attributes", m_attributes);

    if (!m_relationships.isEmpty())
    {
        QJsonObject relationships;
        QMultiMap<QString, JsonApiResourceObject>::const_iterator it;
        for (it = m_relationships.constBegin(); it != m_relationships.constEnd(); ++it)
        {
            const QString& key = it.key();
            const JsonApiResourceObject& value = it.value();

            QJsonObject data;
            data.insert("type", value.type());
            data.insert("id", value.id());

            QJsonObject relationship;
            if (!relationships.contains(key))
            {
                relationship.insert("data", data);
            }
            else
            {
                const QJsonValue& relationshipValue = relationships.value(key);
                Q_ASSERT(relationshipValue.isObject());
                const QJsonObject& relationshipObject = relationshipValue.toObject();
                const QJsonValue& relationshipValueData = relationshipObject.value("data");

                QJsonArray relationshipArray;
                if (relationshipValueData.isObject())
                {
                    relationshipArray << relationshipValueData.toObject();
                    relationshipArray << data;
                }
                else if (relationshipValueData.isArray())
                {
                    relationshipArray = relationshipValueData.toArray();
                    relationshipArray << data;
                }

                relationship.insert("data", relationshipArray);
            }

            relationships.insert(key, relationship);
        }
        object.insert("relationships", relationships);
    }

    if (!m_meta.isEmpty())
    {
        object.insert("meta", m_meta);
    }

    return object;
}

QList<JsonApiResourceObject> JsonApiResourceObject::includedResources() const
{
    QList<JsonApiResourceObject> includedResources;
    QList<QPair<int, QString>> foundIncludedResources;

    foreach (const JsonApiResourceObject& resource, m_relationships)
    {
        QPair<int, QString> pair(resource.id(), resource.type());
        if (!foundIncludedResources.contains(pair))
        {
            includedResources << resource;
            foundIncludedResources << pair;
        }
    }

    return includedResources;
}

bool JsonApiResourceObject::isNull() const
{
    return m_isNull;
}

QMultiMap<QString, JsonApiResourceObject>& JsonApiResourceObject::relationships()
{
    return m_relationships;
}

QMultiMap<QString, JsonApiResourceObject> JsonApiResourceObject::relationships() const
{
    return m_relationships;
}

int JsonApiResourceObject::id() const
{
    return m_id;
}

QString JsonApiResourceObject::type() const
{
    return m_type;
}

QJsonObject JsonApiResourceObject::attributes() const
{
    return m_attributes;
}

QJsonObject JsonApiResourceObject::meta() const
{
    return m_meta;
}

void JsonApiResourceObject::setAttributes(const QJsonObject &attributes)
{
    m_attributes = attributes;
}

void JsonApiResourceObject::setRelationships(const QMultiMap<QString, JsonApiResourceObject> &relationships)
{
    m_relationships = relationships;
}

void JsonApiResourceObject::setMeta(const QJsonObject &meta)
{
    m_meta = meta;
}

QDebug operator<<(QDebug dbg, const JsonApiResourceObject &obj)
{
    dbg.nospace() << "JsonApiResourceObject(" << obj.toJsonObject() << ")";

    return dbg.space();
}

bool operator==(const JsonApiResourceObject &first, const JsonApiResourceObject &second)
{
    return first.isNull() == second.isNull()
            && first.id() == second.id()
            && first.type() == second.type()
            && first.attributes() == second.attributes()
            && first.relationships() == second.relationships()
            && first.meta() == second.meta();
}
