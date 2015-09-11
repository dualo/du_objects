#ifndef DUVERSION_H
#define DUVERSION_H

#include "dustring.h"


DU_OBJECT(DuVersion);

class DuVersion : public DuString
{
public:
    DuVersion(const QString& version);
    DuVersion(int major = 0, int minor = 0, int patch = 0);

    virtual DuObjectPtr clone() const;

    QDebug debugPrint(QDebug dbg) const;

    int major() const;
    int minor() const;
    int patch() const;

    QString getVersion() const;
    bool setVersion(const QString& version);
    bool setVersion(int major, int minor, int patch);

private:
    int m_major;
    int m_minor;
    int m_patch;
};

#endif // DUVERSION_H
