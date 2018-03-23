#ifndef DUVERSION_H
#define DUVERSION_H

#include "dustring.h"

#include <QCoreApplication>

#include <Util/Version.h>


DU_OBJECT(DuVersion);

class DuVersion : public DuValue
{
    Q_DECLARE_TR_FUNCTIONS(DuVersion)

public:
    explicit DuVersion(const Version& version);
    explicit DuVersion();
    virtual ~DuVersion() = default;

    int major() const;
    int minor() const;
    int patch() const;

    Version getVersion() const;
    bool setVersion(const Version& version);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const Q_DECL_OVERRIDE;

    virtual QByteArray toDuMusicBinary() const Q_DECL_OVERRIDE;
    virtual QByteArray toMidiBinary() const Q_DECL_OVERRIDE;
    virtual QJsonValue toJson() const Q_DECL_OVERRIDE;
    virtual QHttpPart toHttpPart(const QString &name) const Q_DECL_OVERRIDE;

    QDebug debugPrint(QDebug dbg) const Q_DECL_OVERRIDE;

    // DuValue interface
protected:
    virtual QVariant checkValue(const QVariant &value, bool &success) Q_DECL_OVERRIDE;
};

#endif // DUVERSION_H
