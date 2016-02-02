#include "dutrack.h"

#include "duloop.h"

#include <cstring>

#include <QJsonArray>
#include <QJsonObject>

#include "../general/duarray.h"
#include "../general/dunumeric.h"

#include "../miditodumusic/midiconversionhelper.h"

DU_OBJECT_IMPL(DuTrack)

DuTrack::DuTrack() :
    DuContainer()
{
    addChild(KeyChannel,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
                           MUSIC_MAXTRACK, 0));

    addChild(KeyCurrentLoop,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
                           MUSIC_MAXLAYER - 1, 0));

    DuArrayPtr loopsArray(new DuArray(MUSIC_MAXLAYER));
    for (int i = 0; i < MUSIC_MAXLAYER; ++i)
        loopsArray->append(new DuLoop);
    addChild(KeyLoops, loopsArray);
}

DuTrack::~DuTrack()
{
}

DuObjectPtr DuTrack::clone() const
{
    return DuTrackPtr(new DuTrack(*this));
}


DuTrackPtr DuTrack::fromDuMusicBinary(const music_track &du_track,
                                      const music_sample *du_sample_start,
                                      uint totalNbSamples)
{
    const DuTrackPtr track(new DuTrack);
    bool verif = true;

    verif = track->setChannel(du_track.t_midichannel) ? verif : false;
    verif = track->setCurrentLoop(du_track.t_currentloop) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuTrack::fromDuMusicBinary():\n"
                                     << "an attribute was not properly set";
    }

    DuArrayPtr loopsArray(new DuArray(MUSIC_MAXLAYER));

    for (int i = 0; i < MUSIC_MAXLAYER; i++)
    {
        const music_loop &du_loop = du_track.t_loop[i];

        DuLoopPtr loop;

        if (du_loop.l_state == REC_EMPTY)
        {
            loop = DuLoopPtr(new DuLoop);
        }
        else
        {
            if (du_loop.l_adress % MUSIC_SAMPLE_SIZE != 0)
            {
                qCCritical(LOG_CAT_DU_OBJECT)
                        << "DuTrack::fromDuMusicBinary():\n"
                        << "failed to generate DuTrack\n"
                        << "invalid loop address (not a multiple of MUSIC_SAMPLE_SIZE)\n"
                        << "du_loop.l_adress =" << du_loop.l_adress << "\n"
                        << "MUSIC_SAMPLE_SIZE =" << MUSIC_SAMPLE_SIZE;

                return DuTrackPtr();
            }

            music_sample_p firstSampleIndex = du_loop.l_adress / MUSIC_SAMPLE_SIZE;
            if (firstSampleIndex + du_loop.l_numsample > totalNbSamples)
            {
                qCCritical(LOG_CAT_DU_OBJECT)
                        << "DuTrack::fromDuMusicBinary():\n"
                        << "failed to generate DuTrack\n"
                        << "invalid number of events\n"
                        << "totalNbSamples =" << totalNbSamples << "\n"
                        << "firstSampleIndex =" << firstSampleIndex << "\n"
                        << "du_loop.l_numsample =" << du_loop.l_numsample;

                return DuTrackPtr();
            }

            const music_sample *du_sample_address = &du_sample_start[firstSampleIndex];

            loop = DuLoop::fromDuMusicBinary(du_loop, du_sample_address);
        }

        if (loop == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::fromDuMusicBinary():\n"
                                          << "failed to generate DuTrack\n"
                                          << "a DuLoop was not properly generated";

            return DuTrackPtr();
        }

        if (!loopsArray->append(loop))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::fromDuMusicBinary():\n"
                                          << "failed to generate DuTrack\n"
                                          << "a DuLoop was not properly appended";

            return DuTrackPtr();
        }
    }

    track->setLoops(loopsArray);

    return track;
}


DuTrackPtr DuTrack::fromJson(const QJsonObject &jsonTrack)
{
    QJsonValue jsonChannel      = jsonTrack[KeyChannel];
    QJsonValue jsonCurrentLoop  = jsonTrack[KeyCurrentLoop];
    QJsonValue jsonLoops        = jsonTrack[KeyLoops];

    if (        !jsonChannel.isDouble() ||  !jsonCurrentLoop.isDouble()
            ||  !jsonLoops.isArray())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::fromJson():\n"
                                      << "failed to generate DuTrack\n"
                                      << "a json key did not contain the proper type";

        return DuTrackPtr();
    }


    DuTrackPtr track(new DuTrack);
    bool verif = true;

    verif = track->setChannel(jsonChannel.toInt()) ? verif : false;
    verif = track->setCurrentLoop(jsonCurrentLoop.toInt()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuTrack::fromJson():\n"
                                     << "an attribute was not properly set";
    }

    const QJsonArray &jsonLoopArray = jsonLoops.toArray();
    if (jsonLoopArray.count() != MUSIC_MAXLAYER)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuTrack::fromJson():\n"
                << "failed to generate DuTrack\n"
                << "json file does not contain the proper amount of tracks";

        return DuTrackPtr();
    }

    for (int i = 0; i < jsonLoopArray.count(); i++)
    {
        const DuLoopPtr &loop = DuLoop::fromJson(jsonLoopArray[i].toObject());
        if (loop == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::fromJson():\n"
                                          << "failed to generate DuTrack\n"
                                          << "a DuLoop was not properly generated";

            return DuTrackPtr();
        }
        if (!track->appendLoop(loop))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::fromJson():\n"
                                          << "failed to generate DuTrack\n"
                                          << "a DuLoop was not properly appended";

            return DuTrackPtr();
        }
    }

    return track;
}


DuTrackPtr DuTrack::fromMidi(const MidiConversionHelper &helper, int trackIndex)
{
    if (!helper.isValid())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::fromMidi():\n"
                                      << "failed to generate DuTrack\n"
                                      << "invalid conversion helper";

        return DuTrackPtr();
    }

    DuTrackPtr track(new DuTrack);
    bool verif = true;

    verif = track->setChannel(MUSIC_MAXTRACK - trackIndex) ? verif : false;
//    verif = track->setCurrentLoop(0) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuTrack::fromMidi():\n"
                                     << "an attribute was not properly set";
    }

    DuArrayPtr loopsArray(new DuArray(MUSIC_MAXLAYER));
    for (int i = 0; i < MUSIC_MAXLAYER; i++)
    {
        int index = helper.findIndexes(trackIndex, i);

        if (index == -1)
        {
            if (!loopsArray->append(DuLoopPtr(new DuLoop)))
            {
                qCCritical(LOG_CAT_DU_OBJECT)
                        << "DuTrack::fromMidi():\n"
                        << "failed to generate DuTrack\n"
                        << "a DuLoop was not properly appended";

                return DuTrackPtr();
            }
        }
        else
        {
            const DuLoopPtr &loop = DuLoop::fromMidi(helper, index);
            if (loop == NULL)
            {
                qCCritical(LOG_CAT_DU_OBJECT)
                        << "DuTrack::fromMidi():\n"
                        << "failed to generate DuTrack\n"
                        << "a DuLoop was not properly generated";

                return DuTrackPtr();
            }
            if (!loopsArray->append(loop))
            {
                qCCritical(LOG_CAT_DU_OBJECT)
                        << "DuTrack::fromMidi():\n"
                        << "failed to generate DuTrack\n"
                        << "a DuLoop was not properly appended";

                return DuTrackPtr();
            }
        }
    }

    track->setLoops(loopsArray);

    return track;
}


QByteArray DuTrack::toDuMusicBinary() const
{
    music_track du_track;

    int tmpNum = 0;

    std::memset((char*)&du_track, 0, size());


    const DuArrayConstPtr &loops = getLoops();
    if (loops == NULL)
        return QByteArray();

    int loopSize = loops->size();
    if (loopSize == -1)
        return QByteArray();

    const QByteArray &loopsArray = loops->toDuMusicBinary();
    if (loopsArray.isNull())
        return QByteArray();

    std::memcpy(&(du_track.t_loop), loopsArray.data(), loopSize);


    tmpNum = getChannel();
    if (tmpNum == -1)
        return QByteArray();
    du_track.t_midichannel = tmpNum;

    tmpNum = getCurrentLoop();
    if (tmpNum == -1)
        return QByteArray();
    du_track.t_currentloop = tmpNum;


    return QByteArray((char *)&(du_track), size());
}


QVector<DuMidiTrackPtr> DuTrack::toDuMidiTrackArray(int durationRef,
                                                    int transpose) const
{
    const DuArrayConstPtr &loops = getLoops();
    if (loops == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuTrack::toDuMidiTrackArray():\n"
                << "KEY_TRACK_LOOPS is NULL";

        return QVector<DuMidiTrackPtr>();
    }

    int channel = getChannel();
    if (channel == -1)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuTrack::toDuMidiTrackArray():\n"
                << "invalid channel";

        return QVector<DuMidiTrackPtr>();
    }

    QVector<DuMidiTrackPtr> retList;

    for (int i = 0; i < MUSIC_MAXLAYER; i++)
    {
        const DuLoopConstPtr &loop = loops->at(i).dynamicCast<const DuLoop>();
        if (loop == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT)
                    << "DuTrack::toDuMidiTrackArray():\n"
                    << "a loop was NULL";

            return QVector<DuMidiTrackPtr>();
        }

        const DuMidiTrackPtr &midiTrack =
                loop->toDuMidiTrack(durationRef, channel, transpose);

        if (midiTrack != NULL)
            retList.append(midiTrack);
    }

    return retList;
}


int DuTrack::size() const
{
    return MUSIC_TRACK_SIZE;
}


DU_KEY_ACCESSORS_IMPL(DuTrack, Channel,     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuTrack, CurrentLoop, Numeric, int, -1)
DU_KEY_ACCESSORS_OBJECT_IMPL(DuTrack, Loops, DuArray)

bool DuTrack::appendLoop(const DuLoopPtr &loop)
{
    const DuArrayPtr &tmp = getLoops();

    if (tmp == NULL)
        return false;

    return tmp->append(loop);
}


int DuTrack::eventsSize() const
{
    int eventsSize = 0;
    int tmpSize = 0;

    const DuArrayConstPtr &loops = getLoops();
    if (loops == NULL)
        return -1;

    int count = loops->count();
    for (int i = 0; i < count; i++)
    {
        const DuLoopConstPtr &loop = loops->atAs<DuLoop>(i);
        if (loop == NULL)
            return -1;

        tmpSize = loop->eventsSize();
        if (tmpSize == -1)
            return -1;

        eventsSize += tmpSize;
    }

    return eventsSize;
}
