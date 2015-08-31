#ifndef DUSCALEMODEL_H
#define DUSCALEMODEL_H

#include <QObject>

class DuScaleModel : public QObject
{
    Q_OBJECT
public:
    explicit DuScaleModel(QObject *parent = 0);
    ~DuScaleModel();

signals:

public slots:
};

#endif // DUSCALEMODEL_H
