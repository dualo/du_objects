#include "dulayer.h"

#include "dusample.h"

#include <du_objects/general/duarray.h>
#include <du_objects/general/dunumeric.h>

DU_OBJECT_IMPL(DuLayer)

DuLayer::DuLayer() :
    DuContainer()
{
    addChild(KeySampleArray, new DuArray);

    addChild(KeyMinVelocity, new DuNumeric(0x01, NUMERIC_DEFAULT_SIZE, 0x7F, 0x01));
    addChild(KeyMaxVelocity, new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE, 0x7F, 0x01));
}

DuObjectPtr DuLayer::clone() const
{
    return DuLayerPtr(new DuLayer(*this));
}

DuLayerPtr DuLayer::fromBinary(const QList<dream_ip>& dreamIPArray,
                               const QList<dream_sp>& dreamSPArray,
                               const QByteArrayList& sampleDataArray)
{
    DuLayerPtr layer(new DuLayer);

    int nbSamples = sampleDataArray.size();
    Q_ASSERT(nbSamples == dreamIPArray.size());
    Q_ASSERT(nbSamples == dreamSPArray.size());

    if (nbSamples == 0)
    {
        return layer;
    }

    uint8_t min_vel = dreamIPArray[0].min_vel;
    uint8_t max_vel = dreamIPArray[0].max_vel;

    bool verif = true;

    verif = layer->setMinVelocity(min_vel + 1) ? verif : false;
    verif = layer->setMaxVelocity(max_vel) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "an attribute was not properly set";
    }

    DuArrayPtr sampleArray(new DuArray);
    for (int i = 0; i < nbSamples; ++i)
    {
        if (dreamIPArray[i].min_vel != min_vel || dreamIPArray[i].max_vel != max_vel)
        {
            qCritical(LOG_CAT_DU_OBJECT) << "Some samples in this layer doesn't have the same bounds :\n"
                                         << "min_vel 1:" << min_vel << "\n"
                                         << "max_vel 1:" << max_vel << "\n"
                                         << "min_vel 2:" << dreamIPArray[i].min_vel << "\n"
                                         << "max_vel 2:" << dreamIPArray[i].max_vel << "\n";
            return DuLayerPtr();
        }

        DuSamplePtr sample = DuSample::fromBinary(dreamIPArray[i], dreamSPArray[i], sampleDataArray[i]);
        if (sample != NULL)
        {
            sampleArray->append(sample);
        }
        else
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Failed to generate DuLayer:\n"
                                          << "Sample" << i << "was not properly generated";
            return DuLayerPtr();
        }
    }
    layer->setSampleArray(sampleArray);

    return layer;
}

DU_KEY_ACCESSORS_OBJECT_IMPL(DuLayer, SampleArray, DuArray)

DU_KEY_ACCESSORS_IMPL(DuLayer, MinVelocity, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuLayer, MaxVelocity, Numeric, int, -1)
