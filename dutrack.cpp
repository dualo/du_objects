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

    DuArray *loops = track->getLoops();

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
        loops->append(loop);
    }

    if (loops->count() == 0)
    {
        delete track;
        return NULL;
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

    DuArray *loops = track->getLoops();
    const QJsonArray &jsonLoopArray = jsonLoops.toArray();

    for (int i = 0; i < jsonLoopArray.count(); i++)
    {
        DuLoop *loop = DuLoop::fromJson(jsonLoopArray[i].toObject());
        if (loop == NULL)
        {
            delete track;
            return NULL;
        }
        loops->append(loop);
    }

    return track;
}


int DuTrack::size() const
{
    //TODO: add defines for dummy sizes in music_parameters_mng.h
    int size = 0;
    int tmpSize;

    DuNumeric *channel = dynamic_cast<DuNumeric *>(getChild(KEY_TRACK_CHANNEL));
    if (channel == NULL)
        return -1;

    tmpSize = channel->size();
    if (tmpSize == -1)
        return -1;

    size += tmpSize;

    DuNumeric *currentLoop = dynamic_cast<DuNumeric *>(getChild(KEY_TRACK_CURRENTLOOP));
    if (currentLoop == NULL)
        return -1;

    tmpSize = currentLoop->size();
    if (tmpSize == -1)
        return -1;

    size += tmpSize;

    DuArray *loops = dynamic_cast<DuArray *>(getChild(KEY_TRACK_LOOPS));
    if (loops == NULL)
        return -1;

    tmpSize = loops->size();
    if (tmpSize == -1)
        return -1;

    size += tmpSize;

    return size + TRACK_DUMMY_SIZE;
}


int DuTrack::getChannel() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_TRACK_CHANNEL));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuTrack::setChannel(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_TRACK_CHANNEL));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuTrack::getCurrentLoop() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_TRACK_CURRENTLOOP));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuTrack::setCurrentLoop(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_TRACK_CURRENTLOOP));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

DuArray *DuTrack::getLoops()
{
    return dynamic_cast<DuArray *>(getChild(KEY_TRACK_LOOPS));
}

void DuTrack::setLoops(DuArray *array)
{
    DuObject *tmp = getChild(KEY_TRACK_LOOPS);

    if (tmp != NULL)
        delete tmp;

    addChild(KEY_TRACK_LOOPS, array);
}
