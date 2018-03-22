#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QMetaType>

struct Version
{
    int major;
    int minor;
    int patch;
    QString suffix;

    Version() : major(-1), minor(-1), patch(-1) {}

    inline QString toString() const
    {
        return QString("%1.%2.%3%4").arg(major).arg(minor).arg(patch).arg(suffix.isEmpty() ? QString() : "-" + suffix);
    }

    friend inline bool operator<(const Version& v1, const Version& v2)
    {
        if (v1.major < v2.major)
            return true;

        if (v1.minor < v2.minor)
            return true;

        if (v1.patch < v2.patch)
            return true;

        if (v1.suffix == v2.suffix)
            return false;

        if (v1.suffix.isEmpty())
            return v2.suffix.startsWith("patch");

        if (v2.suffix.isEmpty())
            return !v1.suffix.startsWith("patch");

        if (v1.suffix == "dev")
            return v2.suffix != "dev";

        if (v1.suffix.startsWith("alpha"))
        {
            if (v2.suffix.startsWith("alpha"))
            {
                int alphaVersion1 = v1.suffix.midRef(5).toInt();
                int alphaVersion2 = v2.suffix.midRef(5).toInt();

                return alphaVersion1 < alphaVersion2;
            }

            return v2.suffix != "dev";
        }

        if (v1.suffix.startsWith("beta"))
        {
            if (v2.suffix.startsWith("beta"))
            {
                int betaVersion1 = v1.suffix.midRef(4).toInt();
                int betaVersion2 = v2.suffix.midRef(4).toInt();

                return betaVersion1 < betaVersion2;
            }

            return v2.suffix != "dev" && !v2.suffix.startsWith("alpha");
        }

        if (v1.suffix.startsWith("RC"))
        {
            if (v2.suffix.startsWith("RC"))
            {
                int rcVersion1 = v1.suffix.midRef(2).toInt();
                int rcVersion2 = v2.suffix.midRef(2).toInt();

                return rcVersion1 < rcVersion2;
            }

            return v2.suffix != "dev" && !v2.suffix.startsWith("alpha") && !v2.suffix.startsWith("beta");
        }

        if (v1.suffix.startsWith("patch"))
        {
            if (v2.suffix.startsWith("patch"))
            {
                int patchVersion1 = v1.suffix.midRef(5).toInt();
                int patchVersion2 = v2.suffix.midRef(5).toInt();

                return patchVersion1 < patchVersion2;
            }

            return v2.suffix != "dev" && !v2.suffix.startsWith("alpha") && !v2.suffix.startsWith("beta") && !v2.suffix.startsWith("RC");
        }

        return false;
    }
    friend inline bool operator>(const Version& v1, const Version& v2)
    {
        return v2 < v1;
    }
    friend inline bool operator<=(const Version& v1, const Version& v2)
    {
        return !(v1 > v2);
    }
    friend inline bool operator>=(const Version& v1, const Version& v2)
    {
        return !(v1 < v2);
    }
    friend inline bool operator==(const Version& v1, const Version& v2)
    {
        return v1.major == v2.major
                && v1.minor == v2.minor
                && v1.patch == v2.patch
                && v1.suffix == v2.suffix;
    }
    friend inline bool operator!=(const Version& v1, const Version& v2)
    {
        return !(v1 == v2);
    }

    static Version extractVersionFromStr(const QString& str, bool *ok = Q_NULLPTR);
};

QDebug operator<<(QDebug dbg, const Version& version);

Q_DECLARE_METATYPE(Version)

#endif // VERSION_H
