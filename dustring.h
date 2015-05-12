#ifndef DUSTRING_H
#define DUSTRING_H

#include "duvalue.h"


class DuString : public DuValue
{
public:
    explicit DuString(const QString &value, int maxSize);
    explicit DuString(int maxSize);
    virtual ~DuString();

    QByteArray toDuMusicFile() const;
    QJsonValue toJson() const;

    QString getString() const;
    bool setString(const QString &value);
};

#endif // DUSTRING_H
