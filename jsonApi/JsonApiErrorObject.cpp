#include "JsonApiErrorObject.h"

JsonApiErrorObject::JsonApiErrorObject(const QJsonObject &data) :
    m_isNull(true)
{
    if (!data.contains("id")
            || !data.contains("status")
            || !data.contains("code")
            || !data.contains("title")
            || !data.contains("detail"))
        return;

    const QJsonValue& idValue       = data.value("id");
    const QJsonValue& statusValue   = data.value("status");
    const QJsonValue& codeValue     = data.value("code");
    const QJsonValue& titleValue    = data.value("title");
    const QJsonValue& detailValue   = data.value("detail");

    if (!idValue.isDouble()
            || !statusValue.isDouble()
            || !codeValue.isDouble()
            || !titleValue.isString()
            || !detailValue.isString())
        return;

    m_id        = idValue.toInt();
    m_status    = statusValue.toInt();
    m_code      = codeValue.toInt();
    m_title     = titleValue.toString();
    m_detail    = detailValue.toString();

    if (data.contains("meta"))
    {
        const QJsonValue& metaValue = data.value("meta");
        if (!metaValue.isObject())
            return;

        m_meta = metaValue.toObject();
    }

    m_isNull = false;
}

JsonApiErrorObject::JsonApiErrorObject(int id,
                                       int status,
                                       int code,
                                       const QString &title,
                                       const QString &detail,
                                       const QJsonObject &meta) :
    m_isNull(id < 0 || status < 0 || code < 0 || title.isEmpty() || detail.isEmpty()),
    m_id(id),
    m_status(status),
    m_code(code),
    m_title(title),
    m_detail(detail),
    m_meta(meta)
{
}

QJsonObject JsonApiErrorObject::toJsonObject() const
{
    if (m_isNull)
        return QJsonObject();

    QJsonObject object;

    object.insert("id",     m_id);
    object.insert("status", m_status);
    object.insert("code",   m_code);
    object.insert("title",  m_title);
    object.insert("detail", m_detail);

    if (!m_meta.isEmpty())
    {
        object.insert("meta",   m_meta);
    }

    return object;
}

bool JsonApiErrorObject::isNull() const
{
    return m_isNull;
}

int JsonApiErrorObject::id() const
{
    return m_id;
}

int JsonApiErrorObject::status() const
{
    return m_status;
}

int JsonApiErrorObject::code() const
{
    return m_code;
}

QString JsonApiErrorObject::title() const
{
    return m_title;
}

QString JsonApiErrorObject::detail() const
{
    return m_detail;
}

QJsonObject JsonApiErrorObject::meta() const
{
    return m_meta;
}

QString JsonApiErrorObject::toString() const
{
    return tr("%1: %2").arg(m_title).arg(m_detail);
}

bool operator==(const JsonApiErrorObject &first, const JsonApiErrorObject &second)
{
    return first.isNull() == second.isNull()
            && first.id() == second.id()
            && first.status() == second.status()
            && first.code() == second.code()
            && first.title() == second.title()
            && first.detail() == second.detail()
            && first.meta() == second.meta();
}

QDebug operator<<(QDebug dbg, const JsonApiErrorObject &obj)
{
    dbg.nospace() << "JsonApiErrorObject(" << obj.toJsonObject() << ")";

    return dbg.space();
}
