#ifndef DUURL_H
#define DUURL_H

#include "duvalue.h"


DU_OBJECT(DuUrl)

class DuUrl : public DuValue
{
public:
    DuUrl(const QUrl &url = QUrl());

    virtual DuObjectPtr clone() const;

    QByteArray toDuMusicBinary() const;
    QJsonValue toJson() const;

    QDebug debugPrint(QDebug dbg) const;

    QUrl getUrl() const;
    void setUrl(const QUrl &url);
};

#endif // DUURL_H