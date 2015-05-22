#ifndef DUURL_H
#define DUURL_H

#include "duvalue.h"

class DuUrl : public DuValue
{
public:
    DuUrl(const QUrl& url = QUrl());

    // DuObject interface
public:
    QByteArray toDuMusicFile() const;
    QJsonValue toJson() const;
    QDebug debugPrint(QDebug dbg) const;

    QUrl getUrl() const;
    void setUrl(const QUrl& url);
};

#endif // DUURL_H
