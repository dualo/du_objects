#include "dutrack.h"

#include <cstring>

#include <QJsonArray>
#include <QJsonObject>

DU_OBJECT_IMPL(DuTrack)

DuTrack::DuTrack() :
    DuContainer()
{
    addChild(KEY_TRACK_CHANNEL,
             new DuNumeric(1, NUMERIC_DEFAULT_SIZE,
                           MUSIC_MAXTRACK, 1));

    addChild(KEY_TRACK_CURRENTLOOP,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
                           MUSIC_MAXLAYER - 1, 0));

    addChild(KEY_TRACK_LOOPS, new DuArray(MUSIC_MAXLAYER));
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
                                      int totalSample)
{
    const DuTrackPtr track(new DuTrack);
    bool verif = true;

    verif = verif && track->setChannel(du_track.t_midichannel);
    verif = verif && track->setCurrentLoop(du_track.t_currentloop);

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuTrack::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    for (int i = 0; i < MUSIC_MAXLAYER; i++)
    {
        const music_loop &du_loop = du_track.t_loop[i];

        int sampleSize = sizeof(music_sample);
        if (totalSample * sampleSize
                < (int)du_loop.l_adress + du_loop.l_numsample * sampleSize
            || RECORD_SAMPLEBUFFERSIZE * sampleSize
                < (int)du_loop.l_adress + du_loop.l_numsample * sampleSize)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::fromDuMusicBinary():\n"
                        << "failed to generate DuTrack\n"
                        << "invalid number of events\n"
                        << "(du_sample size =" << (totalSample * sampleSize)
                        << "(max size =" << (RECORD_SAMPLEBUFFERSIZE * sampleSize)
                        << ", du_loop.l_adress =" << du_loop.l_adress
                        << ", du_loop.l_numsample =" << du_loop.l_numsample
                        << ", sizeof(music_sample) =" << sampleSize << ")";

            return DuTrackPtr();
        }

        const music_sample *du_sample_address = (music_sample*)
                ((long)du_sample_start + du_loop.l_adress);

        const DuLoopPtr &loop =
                DuLoop::fromDuMusicBinary(du_loop, du_sample_address);
        if (loop == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::fromDuMusicBinary():\n"
                        << "failed to generate DuTrack\n"
                        << "a DuLoop was not properly generated";

            return DuTrackPtr();
        }
        if (!track->appendLoop(loop))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::fromDuMusicBinary():\n"
                        << "failed to generate DuTrack\n"
                        << "a DuLoop was not properly appended";

            return DuTrackPtr();
        }
    }

    return track;
}

DuTrackPtr DuTrack::fromDuMusicBinary(const music_track &du_track)
{
    const DuTrackPtr track(new DuTrack);
    bool verif = true;

    verif = verif && track->setChannel(du_track.t_midichannel);
    verif = verif && track->setCurrentLoop(du_track.t_currentloop);

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuTrack::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    for (int i = 0; i < MUSIC_MAXLAYER; i++)
    {
        const DuLoopPtr &loop =
                DuLoop::fromDuMusicBinary(du_track.t_loop[i]);
        if (loop == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::fromDuMusicBinary():\n"
                        << "failed to generate DuTrack\n"
                        << "a DuLoop was not properly generated";

            return DuTrackPtr();
        }
        if (!track->appendLoop(loop))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::fromDuMusicBinary():\n"
                        << "failed to generate DuTrack\n"
                        << "a DuLoop was not properly appended";

            return DuTrackPtr();
        }
    }

    return track;
}


DuTrackPtr DuTrack::fromJson(const QJsonObject &jsonTrack)
{
    QJsonValue jsonChannel      = jsonTrack[KEY_TRACK_CHANNEL];
    QJsonValue jsonCurrentLoop  = jsonTrack[KEY_TRACK_CURRENTLOOP];
    QJsonValue jsonLoops        = jsonTrack[KEY_TRACK_LOOPS];

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

    verif = verif && track->setChannel(jsonChannel.toInt());
    verif = verif && track->setCurrentLoop(jsonCurrentLoop.toInt());

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuTrack::fromJson():\n"
                   << "an attribute was not properly set";
    }

    const QJsonArray &jsonLoopArray = jsonLoops.toArray();
    if (jsonLoopArray.count() != MUSIC_MAXLAYER)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::fromJson():\n"
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


QByteArray DuTrack::toDuMusicBinary() const
{
    music_track du_track;

    int tmpNum = 0;

    QByteArray tmpClear(size(), (char)0x00);
    std::memcpy((char *)&(du_track), tmpClear.data(), size());


    const DuArrayConstPtr &loops = getLoops();
    if (loops == NULL)
        return QByteArray();
    const QByteArray &loopsArray = loops->toDuMusicBinary();
    if (loopsArray.isNull())
        return QByteArray();

    std::memcpy(&(du_track.t_loop), loopsArray.data(), loops->size());


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


QList<DuMidiTrackPtr> DuTrack::toDuMidiTrackArray(int durationRef) const
{
    const DuArrayConstPtr &loops = getLoops();
    if (loops == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::toDuMidiTrackArray():\n"
                    << "could not retrieve loop array";

        return QList<DuMidiTrackPtr>();
    }

    QList<DuMidiTrackPtr> retList;

    for (int i = 0; i < MUSIC_MAXLAYER; i++)
    {
        const DuLoopConstPtr &loop = loops->at(i).dynamicCast<const DuLoop>();
        if (loop == NULL)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "DuTrack::toDuMidiTrackArray():\n"
                        << "a loop was NULL";

            return QList<DuMidiTrackPtr>();
        }

        const DuMidiTrackPtr &midiTrack = loop->toDuMidiTrack(durationRef);

        if (midiTrack != NULL)
            retList.append(midiTrack);
    }

    return retList;
}


int DuTrack::size() const
{
    return MUSIC_TRACK_SIZE;
}


int DuTrack::getChannel() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_TRACK_CHANNEL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuTrack::setChannel(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_TRACK_CHANNEL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuTrack::getCurrentLoop() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_TRACK_CURRENTLOOP);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuTrack::setCurrentLoop(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_TRACK_CURRENTLOOP);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

DuArrayConstPtr DuTrack::getLoops() const
{
    return getChildAs<DuArray>(KEY_TRACK_LOOPS);
}

void DuTrack::setLoops(const DuArrayPtr& array)
{
    addChild(KEY_TRACK_LOOPS, array);
}

bool DuTrack::appendLoop(const DuLoopPtr &loop)
{
    const DuArrayPtr &tmp = getChildAs<DuArray>(KEY_TRACK_LOOPS);

    if (tmp == NULL)
        return false;

    return tmp->append(loop);
}


int DuTrack::eventsSize() const
{
    int eventsSize = 0;
    int tmpSize = 0;

    const DuArrayConstPtr &loops = getChildAs<DuArray>(KEY_TRACK_LOOPS);
    if (loops == NULL)
        return -1;

    int count = loops->count();
    for (int i = 0; i < count; i++)
    {
        const DuLoopConstPtr &loop = loops->at(i).dynamicCast<const DuLoop>();
        if (loop == NULL)
            return -1;

        tmpSize = loop->eventsSize();
        if (tmpSize == -1)
            return -1;

        eventsSize += tmpSize;
    }

    return eventsSize;
}
