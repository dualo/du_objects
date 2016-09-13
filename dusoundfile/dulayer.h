#ifndef DULAYER_H
#define DULAYER_H

#include "../general/ducontainer.h"

#if QT_VERSION < 0x050400
typedef QList<QByteArray> QByteArrayList;
#endif

#ifdef Q_OS_WIN
#include "../general/duarray.h"
#else
DU_OBJECT_TEMPLATE(DuArray);
#endif
DU_OBJECT(DuSample);

DU_OBJECT(DuLayer);

class DuLayer : public DuContainer
{
public:
    DuLayer();
    virtual ~DuLayer() = default;

    virtual DuObjectPtr clone() const override;

    static DuLayerPtr fromBinary(const QList<dream_ip> &dreamIPArray,
                                 const QVector<dream_sp> &dreamSPArray,
                                 const QByteArrayList &sampleDataArray);

    DU_KEY_ACCESSORS_OBJECT_TEMPLATE(SampleArray, DuArray, DuSample)

    DU_KEY_ACCESSORS(MinVelocity,   int)
    DU_KEY_ACCESSORS(MaxVelocity,   int)
};

#endif // DULAYER_H
