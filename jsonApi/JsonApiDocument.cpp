#include "JsonApiDocument.h"

JsonApiDocument::JsonApiDocument(const QJsonDocument &jsonDoc) :
    m_isNull(true),
    m_isError(true)
{
    if (jsonDoc.isEmpty() || !jsonDoc.isObject())
        return;

    const QJsonObject& jsonObject = jsonDoc.object();
    if (jsonObject.isEmpty())
        return;

    if (jsonObject.contains("data"))
    {
        // DATA
        const QJsonValue& dataArrayValue = jsonObject.value("data");
        if (!dataArrayValue.isArray())
            return;

        const QJsonArray& dataArray = dataArrayValue.toArray();
        for (QJsonArray::const_iterator it = dataArray.constBegin(); it != dataArray.constEnd(); ++it)
        {
            const QJsonValue& dataValue = *(it);
            if (!dataValue.isObject())
                return;

            JsonApiResourceObject resource(dataValue.toObject());
            if (resource.isNull())
                return;

            m_primaryData << resource;
        }

        // INCLUDED
        const QJsonValue& includedArrayValue = jsonObject.value("included");
        if (!includedArrayValue.isArray())
            return;

        const QJsonArray& includedArray = includedArrayValue.toArray();
        for (QJsonArray::const_iterator it = includedArray.constBegin(); it != includedArray.constEnd(); ++it)
        {
            const QJsonValue& includedValue = *(it);
            if (!includedValue.isObject())
                return;

            JsonApiResourceObject resource(includedValue.toObject());
            if (resource.isNull())
                return;

            // Replace in primary data's relationships
            QList<JsonApiResourceObject>::iterator primaryDataIt;
            for (primaryDataIt = m_primaryData.begin(); primaryDataIt != m_primaryData.end(); ++primaryDataIt)
            {
                JsonApiResourceObject& primaryDataObj = *(primaryDataIt);

                QMultiMap<QString, JsonApiResourceObject>::iterator relationshipIt;
                for (relationshipIt = primaryDataObj.relationships().begin(); relationshipIt != primaryDataObj.relationships().end(); ++relationshipIt)
                {
                    JsonApiResourceObject& relationshipObj = relationshipIt.value();

                    if (relationshipObj.id() == resource.id() && relationshipObj.type() == resource.type())
                    {
                        relationshipObj.setAttributes(resource.attributes());
                        relationshipObj.setRelationships(resource.relationships());
                        relationshipObj.setMeta(resource.meta());
                    }
                }
            }
        }

        m_isError = false;
    }
    else if (jsonObject.contains("errors"))
    {
        const QJsonValue& errorsVal = jsonObject.value("errors");
        if (!errorsVal.isArray())
            return;

        const QJsonArray& errors = errorsVal.toArray();
        for (QJsonArray::const_iterator it = errors.constBegin(); it != errors.constEnd(); ++it)
        {
            const QJsonValue& errorVal = (*it);
            if (!errorVal.isObject())
                continue;

            JsonApiErrorObject errorObject(errorVal.toObject());
            if (errorObject.isNull())
                return;

            m_errors << errorObject;
        }

        m_isError = true;
    }
    else
        return;

    m_isNull = false;
}

JsonApiDocument::JsonApiDocument(const QList<JsonApiResourceObject> &primaryData, const QJsonObject &meta) :
    m_isNull(primaryData.isEmpty()),
    m_isError(false),
    m_primaryData(primaryData),
    m_meta(meta)
{
}

JsonApiDocument::JsonApiDocument(const QList<JsonApiErrorObject> &errors, const QJsonObject &meta) :
    m_isNull(errors.isEmpty()),
    m_isError(true),
    m_errors(errors),
    m_meta(meta)
{
}

QJsonDocument JsonApiDocument::toJsonDocument() const
{
    if (m_isNull)
    {
        return QJsonDocument();
    }

    QJsonObject documentObject;

    if (m_isError)
    {
        QJsonArray errors;
        foreach (const JsonApiErrorObject& error, m_errors)
        {
            errors << error.toJsonObject();
        }

        documentObject.insert("errors", errors);
    }
    else
    {
        QJsonArray dataArray;
        QJsonArray includedArray;
        QList<QPair<int, QString>> foundIncludedResources;
        foreach (const JsonApiResourceObject& data, m_primaryData)
        {
            dataArray << data.toJsonObject();

            const QList<JsonApiResourceObject>& includedResources = data.includedResources();
            foreach (const JsonApiResourceObject& includedResource, includedResources)
            {
                QPair<int, QString> pair(includedResource.id(), includedResource.type());
                if (!foundIncludedResources.contains(pair))
                {
                    includedArray << includedResource.toJsonObject();
                    foundIncludedResources << pair;
                }
            }
        }

        documentObject.insert("data", dataArray);
        documentObject.insert("included", includedArray);
    }

    return QJsonDocument(documentObject);
}

bool JsonApiDocument::isNull() const
{
    return m_isNull;
}

bool JsonApiDocument::isError() const
{
    return m_isError;
}

QList<JsonApiResourceObject> JsonApiDocument::primaryData() const
{
    return m_primaryData;
}

QList<JsonApiErrorObject> JsonApiDocument::errors() const
{
    return m_errors;
}

QJsonObject JsonApiDocument::meta() const
{
    return m_meta;
}

QString JsonApiDocument::errorStr() const
{
    if (m_isNull || !m_isError || m_errors.size() == 0)
        return QString();

    if (m_errors.size() == 1)
    {
        return m_errors.first().toString();
    }

    QString errorsStr = QObject::tr("Errors:");
    foreach (const JsonApiErrorObject& error, m_errors)
    {
        errorsStr += tr("\n  - %1").arg(error.toString());
    }

    return errorsStr;
}

bool operator==(const JsonApiDocument &first, const JsonApiDocument &second)
{
    return first.primaryData() == second.primaryData()
            && first.errors() == second.errors()
            && first.meta() == second.meta()
            && first.isError() == second.isError()
            && first.isNull() == second.isNull();
}

QDebug operator<<(QDebug dbg, const JsonApiDocument &obj)
{
    dbg.nospace() << "JsonApiDocument(" << obj.toJsonDocument() << ")";

    return dbg.space();
}
