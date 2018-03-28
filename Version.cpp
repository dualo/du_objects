#include "Version.h"

#include <QDebug>
#include <QRegularExpression>


Version::Version() : Version(-1, -1, -1, QString())
{
}

Version::Version(int maj) : Version(maj, 0, 0, QString())
{
}

Version::Version(int maj, int min) : Version(maj, min, 0, QString())
{
}

Version::Version(int maj, int min, int pat) : Version(maj, min, pat, QString())
{
}

Version::Version(int maj, int min, int pat, const QString& suf) :
    major(maj),
    minor(min),
    patch(pat),
    suffix(suf)
{
}

Version Version::extractVersionFromStr(const QString &str, bool *ok)
{
    if (!str.contains(QRegularExpression("^[0-9]+\\.[0-9]+(\\.[0-9]+(-(dev|(beta|alpha|RC|patch)[0-9]+))?)?$")))
    {
        qCritical() << "Version string format incorrect. Should be major.minor.patch(-suffix) : " << str;
        if (ok != Q_NULLPTR)
            *ok = false;
        return {};
    }

    Version versionStruct(0);

    QStringList splitVersionStr1 = str.split('-');
    QStringList splitVersionStr2 = splitVersionStr1[0].split('.');
    versionStruct.major = splitVersionStr2[0].toInt();
    versionStruct.minor = splitVersionStr2[1].toInt();
    if (splitVersionStr2.size() == 2)
    {
        qWarning() << "No patch number in version" << str;
    }
    else
    {
        versionStruct.patch = splitVersionStr2[2].toInt();
        if (splitVersionStr1.size() == 2)
        {
            versionStruct.suffix = splitVersionStr1[1];
        }
    }

    if (ok != Q_NULLPTR)
        *ok = true;

    return versionStruct;
}

QDebug operator<<(QDebug dbg, const Version &version)
{
    dbg.nospace() << version.major << "." << version.minor << "." << version.patch;
    if (!version.suffix.isEmpty())
#if QT_VERSION >= 0x050400
        dbg.nospace().noquote() << "-" << version.suffix;
#else
        dbg.nospace() << "-" << version.suffix;
#endif
    return dbg.space();
}
