#ifndef DUVERSION_H
#define DUVERSION_H

#include "dustring.h"

#include <QCoreApplication>


DU_OBJECT(DuVersion);

class DuVersion : public DuString
{
    Q_DECLARE_TR_FUNCTIONS(DuVersion)

public:
    explicit DuVersion(const QString& version);
    explicit DuVersion(int major, int minor, int patch);
    explicit DuVersion();
    virtual ~DuVersion() = default;

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
