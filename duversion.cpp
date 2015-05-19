#include "duversion.h"

#include <QDebug>

DuVersion::DuVersion(const QString &version) :
    DuString(),
    m_major(0),
    m_minor(0),
    m_patch(0)
{
    bool res = setVersion(version);
    Q_ASSERT(res);
}

DuVersion::DuVersion(int major, int minor, int patch) :
    DuString(),
    m_major(0),
    m_minor(0),
    m_patch(0)
{
    bool res = setVersion(major, minor, patch);
    Q_ASSERT(res);
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
    QStringList nbs = version.split(".");

    if (nbs.size() != 3)
        return false;

    bool ok;

    int major = nbs[0].toInt(&ok);
    if (!ok)
        return false;

    int minor = nbs[1].toInt(&ok);
    if (!ok)
        return false;

    int patch = nbs[2].toInt(&ok);
    if (!ok)
        return false;

    m_major = major;
    m_minor = minor;
    m_patch = patch;
    setString(version);

    return true;
}

bool DuVersion::setVersion(int major, int minor, int patch)
{
    if (major < 0 || minor < 0 || patch < 0)
        return false;

    QString value = QString::number(major) + "." + QString::number(minor) + "." + QString::number(patch);

    m_major = major;
    m_minor = minor;
    m_patch = patch;
    setString(value);

    return true;
}
