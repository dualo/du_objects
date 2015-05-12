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
    DuTrack *track = new DuTrack();

    track->setChannel(du_track.t_midichannel);
    track->setCurrentLoop(du_track.t_currentloop);

    DuArray *loops = track->getLoops();

    for (int i = 0; i < MUSIC_MAXLAYER; i++)
    {
        const music_loop &du_loop = du_track.t_loop[i];
        const music_sample *du_sample_address = (music_sample*)
                ((long)du_sample + du_loop.l_adress);
        DuLoop *loop = DuLoop::fromDuMusicFile(du_loop, du_sample_address);

        if (loop != NULL)
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
    DuTrack *track = new DuTrack();
    const QStringList &keyList = track->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonTrack.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    track->setChannel(jsonTrack[KEY_TRACK_CHANNEL].toInt());
    track->setCurrentLoop(jsonTrack[KEY_TRACK_CURRENTLOOP].toInt());

    DuArray *loops = track->getLoops();
    const QJsonArray &jsonLoops = jsonTrack[KEY_TRACK_LOOPS].toArray();

    for (int i = 0; i < jsonLoops.count(); i++)
    {
        DuLoop *loop = DuLoop::fromJson(jsonLoops[i].toObject());
        if (loop != NULL)
            loops->append(loop);
    }

    return track;
}


int DuTrack::getChannel() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_TRACK_CHANNEL));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuTrack::setChannel(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_TRACK_CHANNEL));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuTrack::getCurrentLoop() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_TRACK_CURRENTLOOP));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuTrack::setCurrentLoop(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_TRACK_CURRENTLOOP));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

DuArray *DuTrack::getLoops()
{
    return dynamic_cast<DuArray *>(getChild(KEY_TRACK_LOOPS));
}

void DuTrack::setLoops(DuArray *array)
{
    if (getChild(KEY_TRACK_LOOPS) != NULL)
        delete getChild(KEY_TRACK_LOOPS);

    addChild(KEY_TRACK_LOOPS, array);
}
