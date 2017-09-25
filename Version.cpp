#include "Version.h"

#include <QDebug>
#include <QRegularExpression>


Version Version::extractVersionFromStr(const QString &str, bool *ok)
{
    Version versionStruct = {0, 0, 0, QString()};

    if (!str.contains(QRegularExpression("^[0-9]+\\.[0-9]+(\\.[0-9]+(-(dev|(beta|alpha|RC|patch)[0-9]+))?)?$")))
    {
        qCritical() << "Version string format incorrect. Should be major.minor.patch(-suffix) : " << str;
        if (ok != Q_NULLPTR)
            *ok = false;
        return versionStruct;
    }

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
