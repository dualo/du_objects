#ifndef DUURL_H
#define DUURL_H

#include "duvalue.h"


DU_OBJECT(DuUrl);

class DuUrl : public DuValue
{
public:
    explicit DuUrl(const QUrl &url = QUrl());

    QUrl getUrl() const;
    bool setUrl(const QUrl &url);

    // DuObject interface
public:
    virtual DuObjectPtr clone() const override;

    virtual QByteArray toDuMusicBinary() const override;
    virtual QJsonValue toJson() const override;

    virtual QDebug debugPrint(QDebug dbg) const override;

    // DuValue interface
public:
    virtual QVariant checkValue(const QVariant &value, bool &success) override;
};

#endif // DUURL_H
