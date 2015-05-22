#include "dutrack.h"

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


DuTrack *DuTrack::fromDuMusicFile(const music_track &du_track,
                                  const music_sample *du_sample)
{
    DuTrack *track = new DuTrack;
    bool verif = true;

    verif = verif && track->setChannel(du_track.t_midichannel);
    verif = verif && track->setCurrentLoop(du_track.t_currentloop);

    if (!verif)
    {
        delete track;
        return NULL;
    }

    for (int i = 0; i < MUSIC_MAXLAYER; i++)
    {
        const music_loop &du_loop = du_track.t_loop[i];
        const music_sample *du_sample_address = (music_sample*)
                ((long)du_sample + du_loop.l_adress);

        DuLoop *loop = DuLoop::fromDuMusicFile(du_loop, du_sample_address);
        if (loop == NULL)
        {
            delete track;
            return NULL;
        }
        if (!track->appendLoop(loop))
        {
            delete track;
            return NULL;
        }
    }

    return track;
}


DuTrack *DuTrack::fromJson(const QJsonObject &jsonTrack)
{
    QJsonValue jsonChannel      = jsonTrack[KEY_TRACK_CHANNEL];
    QJsonValue jsonCurrentLoop  = jsonTrack[KEY_TRACK_CURRENTLOOP];
    QJsonValue jsonLoops        = jsonTrack[KEY_TRACK_LOOPS];

    if (        !jsonChannel.isDouble() ||  !jsonCurrentLoop.isDouble()
            ||  !jsonLoops.isArray())

        return NULL;


    DuTrack *track = new DuTrack;
    bool verif = true;

    verif = verif && track->setChannel(jsonChannel.toInt());
    verif = verif && track->setCurrentLoop(jsonCurrentLoop.toInt());

    if (!verif)
    {
        delete track;
        return NULL;
    }

    const QJsonArray &jsonLoopArray = jsonLoops.toArray();
    for (int i = 0; i < jsonLoopArray.count(); i++)
    {
        DuLoop *loop = DuLoop::fromJson(jsonLoopArray[i].toObject());
        if (loop == NULL)
        {
            delete track;
            return NULL;
        }
        if (!track->appendLoop(loop))
        {
            delete track;
            return NULL;
        }
    }

    return track;
}


QByteArray DuTrack::toDuMusicFile() const
{
    music_track du_track;

    int tmpNum = 0;

    QByteArray tmpClear(size(), (char)0x00);
    std::memcpy((char *)&(du_track), tmpClear.data(), size());


    QSharedPointer<DuArray> loops = getLoops();
    if (loops == NULL)
        return QByteArray();
    const QByteArray &loopsArray = loops->toDuMusicFile();
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


int DuTrack::size() const
{
    return MUSIC_TRACK_SIZE;
}


int DuTrack::getChannel() const
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_TRACK_CHANNEL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuTrack::setChannel(int value)
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_TRACK_CHANNEL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuTrack::getCurrentLoop() const
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_TRACK_CURRENTLOOP);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuTrack::setCurrentLoop(int value)
{
    QSharedPointer<DuNumeric> tmp = getChildAs<DuNumeric>(KEY_TRACK_CURRENTLOOP);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

QSharedPointer<DuArray> DuTrack::getLoops() const
{
    return getChildAs<DuArray>(KEY_TRACK_LOOPS);
}

void DuTrack::setLoops(DuArray *array)
{
    addChild(KEY_TRACK_LOOPS, array);
}


bool DuTrack::appendLoop(DuLoop *loop)
{
    QSharedPointer<DuArray> tmp = getChildAs<DuArray>(KEY_TRACK_LOOPS);

    if (tmp == NULL)
        return false;

    tmp->append(loop);
    return true;
}


int DuTrack::eventsSize() const
{
    int eventsSize = 0;
    int tmpSize = 0;

    QSharedPointer<DuArray> loops = getChildAs<DuArray>(KEY_TRACK_LOOPS);
    if (loops == NULL)
        return -1;

    int count = loops->count();
    for (int i = 0; i < count; i++)
    {
        QSharedPointer<DuLoop> loop = loops->at(i).dynamicCast<DuLoop>();
        if (loop == NULL)
            return -1;

        tmpSize = loop->eventsSize();
        if (tmpSize == -1)
            return -1;

        eventsSize += tmpSize;
    }

    return eventsSize;
}
