#include "dutrack.h"

#include "duloop.h"

#include <cstring>

#include <QJsonArray>
#include <QJsonObject>

#include "../general/duarray.h"
#include "../general/dunumeric.h"

#ifndef NO_MIDI_IMPORT
#include "../miditodumusic/midiconversionhelper.h"
#endif

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

    DuArrayPtr<DuLoop> loopsArray(new DuArray<DuLoop>(MUSIC_MAXLAYER));
    for (int i = 0; i < MUSIC_MAXLAYER; ++i)
        loopsArray->append(new DuLoop);
    addChild(KeyLoops, loopsArray);
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

    DuArrayPtr<DuLoop> loopsArray(new DuArray<DuLoop>(MUSIC_MAXLAYER));

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


#ifndef NO_MIDI_IMPORT
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

    DuArrayPtr<DuLoop> loopsArray(new DuArray<DuLoop>(MUSIC_MAXLAYER));
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
#endif


QByteArray DuTrack::toDuMusicBinary() const
{
    music_track du_track;

    int tmpNum = 0;

    std::memset(&du_track, 0, static_cast<size_t>(size()));


    const DuArrayConstPtr<DuLoop> &loops = getLoops();
    if (loops == NULL)
        return QByteArray();

    int loopSize = loops->size();
    if (loopSize == -1)
        return QByteArray();

    const QByteArray &loopsArray = loops->toDuMusicBinary();
    if (loopsArray.isNull())
        return QByteArray();

    std::memcpy(&(du_track.t_loop), loopsArray.data(), static_cast<size_t>(loopSize));


    tmpNum = getChannel();
    if (tmpNum == -1)
        return QByteArray();
    du_track.t_midichannel = static_cast<quint8>(tmpNum);

    tmpNum = getCurrentLoop();
    if (tmpNum == -1)
        return QByteArray();
    du_track.t_currentloop = static_cast<quint8>(tmpNum);


    return QByteArray(reinterpret_cast<char*>(&du_track), size());
}


QVector<DuMidiTrackPtr> DuTrack::toDuMidiTrackArray(int durationRef,
                                                    int transpose) const
{
    const DuArrayConstPtr<DuLoop> &loops = getLoops();
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
DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuTrack, Loops, DuArray, DuLoop)

bool DuTrack::appendLoop(const DuLoopPtr &loop)
{
    const DuArrayPtr<DuLoop> &tmp = getLoops();

    if (tmp == NULL)
        return false;

    return tmp->append(loop);
}


int DuTrack::eventsSize() const
{
    int eventsSize = 0;
    int tmpSize = 0;

    const DuArrayConstPtr<DuLoop> &loops = getLoops();
    if (loops == NULL)
        return -1;

    int count = loops->count();
    for (int i = 0; i < count; i++)
    {
        const DuLoopConstPtr &loop = loops->at(i);
        if (loop == NULL)
            return -1;

        tmpSize = loop->eventsSize();
        if (tmpSize == -1)
            return -1;

        eventsSize += tmpSize;
    }

    return eventsSize;
}
