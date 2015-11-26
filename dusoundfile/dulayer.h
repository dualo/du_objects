#ifndef DULAYER_H
#define DULAYER_H

#include <du_objects/general/ducontainer.h>

#if QT_VERSION < 0x050400
typedef QList<QByteArray> QByteArrayList;
#endif

DU_OBJECT(DuArray);

DU_OBJECT(DuLayer);

class DuLayer : public DuContainer
{
public:
    DuLayer();

    virtual DuObjectPtr clone() const override;

    static DuLayerPtr fromBinary(const QList<dream_ip> &dreamIPArray,
                                 const QVector<dream_sp> &dreamSPArray,
                                 const QByteArrayList &sampleDataArray);

    DU_KEY_ACCESSORS_OBJECT(SampleArray, DuArray)

    DU_KEY_ACCESSORS(MinVelocity,   int)
    DU_KEY_ACCESSORS(MaxVelocity,   int)
};

#endif // DULAYER_H
