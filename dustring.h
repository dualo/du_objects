#ifndef DUSTRING_H
#define DUSTRING_H

#include "duvalue.h"


class DuString : public DuValue
{
public:
    explicit DuString(const QString &value, int maxSize = -1);
    explicit DuString(int maxSize = -1);
    virtual ~DuString();

    QByteArray toDuMusicFile() const;
    QJsonValue toJson() const;
    virtual QDebug debugPrint(QDebug dbg) const;

    QString getString() const;
    bool setString(const QString &value);
};

#endif // DUSTRING_H
