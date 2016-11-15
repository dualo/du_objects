#include "duversion.h"



DU_OBJECT_IMPL(DuVersion)

DuVersion::DuVersion(const QString &version) :
    DuString(),
    m_major(-1),
    m_minor(-1),
    m_patch(-1)
{
    setVersion(version);
}

DuVersion::DuVersion(int major, int minor, int patch) :
    DuString(),
    m_major(-1),
    m_minor(-1),
    m_patch(-1)
{
    setVersion(major, minor, patch);
}

DuObjectPtr DuVersion::clone() const
{
    return DuVersionPtr(new DuVersion(*this));
}

QDebug DuVersion::debugPrint(QDebug dbg) const
{
    dbg.nospace() << "DuVersion(" << getVersion() << ")";

    return dbg.space();
}

int DuVersion::major() const
{
    return m_major;
}

int DuVersion::minor() const
{
    return m_minor;
}

int DuVersion::patch() const
{
    return m_patch;
}

QString DuVersion::getVersion() const
{
    return getString();
}

bool DuVersion::setVersion(const QString &version)
{
    QStringList nbs = version.split('.');

    if (nbs.size() != 3)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "failed:" << version;
        setString(tr("unknown"));
        return false;
    }

    bool ok = false;

    int major = nbs[0].toInt(&ok);
    if (!ok)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "failed:" << version;
        setString(tr("unknown"));
        return false;
    }

    int minor = nbs[1].toInt(&ok);
    if (!ok)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "failed:" << version;
        setString(tr("unknown"));
        return false;
    }

    int patch = nbs[2].toInt(&ok);
    if (!ok)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "failed:" << version;
        setString(tr("unknown"));
        return false;
    }

    m_major = major;
    m_minor = minor;
    m_patch = patch;
    setString(version);

    return true;
}

bool DuVersion::setVersion(int major, int minor, int patch)
{
    if (major < 0 || minor < 0 || patch < 0)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "failed:" << major << minor << patch;
        setString(tr("unknown"));
        return false;
    }

    QString value = QString::number(major) + "." + QString::number(minor) + "." + QString::number(patch);

    m_major = major;
    m_minor = minor;
    m_patch = patch;
    setString(value);

    return true;
}
