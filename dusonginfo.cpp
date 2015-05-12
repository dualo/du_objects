#include "dusonginfo.h"

DuSongInfo::DuSongInfo() :
    DuContainer()
{
    addChild(KEY_SONG_REFERENCETRACK,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
                           MUSIC_MAXTRACK, 0));

    addChild(KEY_SONG_REFERENCELOOPDURATION, new DuNumeric(0));

    addChild(KEY_SONG_VOLUME,
             new DuNumeric(MUSIC_VOL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           MUSIC_VOL_MAXVALUE, MUSIC_VOL_MINVALUE));

    addChild(KEY_SONG_TEMPO,
             new DuNumeric(MUSIC_TEMPO_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           MUSIC_TEMPO_MAXVALUE, MUSIC_TEMPO_MINVALUE));

    addChild(KEY_SONG_OFFSET,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0xFF, 0x00));

    addChild(KEY_SONG_CLICKVOLUME,
             new DuNumeric(MUSIC_TEMPOVOL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           MUSIC_TEMPOVOL_MAXVALUE, MUSIC_TEMPOVOL_MINVALUE));

    addChild(KEY_SONG_GAIN,
             new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_SONG_LOWCUTFILTERFREQ,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_SONG_HIGHCUTFILTERFREQ,
             new DuNumeric(0x7F, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KEY_SONG_TIMESIGNATURE,
             new DuNumeric(TIME_4_4, NUMERIC_DEFAULT_SIZE,
                           NUM_TIMESIGNATURE, TIME_OFF));

    addChild(KEY_SONG_SCALE,
             new DuNumeric(1, NUMERIC_DEFAULT_SIZE,
                           5, 0));

    addChild(KEY_SONG_TONALITY,
             new DuNumeric(1, NUMERIC_DEFAULT_SIZE,
                           11, 0));
}

DuSongInfo::~DuSongInfo()
{
}


DuSongInfo *DuSongInfo::fromDuMusicFile(const music_song &du_song)
{
    DuSongInfo *songInfo = new DuSongInfo();

    songInfo->setReferenceTrack(du_song.s_reftrack);
    songInfo->setReferenceLoopDuration(du_song.s_looptimer);

    songInfo->setVolume(du_song.s_volume);
    songInfo->setTempo(du_song.s_tempo);
    songInfo->setOffset(du_song.s_decaltempo);
    songInfo->setClickVolume(du_song.s_voltempo);

    songInfo->setGain(du_song.s_mix.m_inputgain);
    songInfo->setLowCutFilterFrequency(du_song.s_mix.m_locutfilterfrequency);
    songInfo->setHighCutFilterFrequency(du_song.s_mix.m_hicutfilterfrequency);

    songInfo->setTimeSignature(du_song.s_timesignature);
    songInfo->setScale(du_song.s_displaynote);
    songInfo->setTonality(du_song.s_scaletonality);

    return songInfo;
}


DuSongInfo *DuSongInfo::fromJson(const QJsonObject &jsonSongInfo)
{
    DuSongInfo *songInfo = new DuSongInfo();
    const QStringList &keyList = songInfo->keys();

    bool test = true;
    for (int i = 0; i < keyList.count(); i++)
    {
        test = test && jsonSongInfo.contains(keyList[i]);
    }

    if (!test)
        return NULL;

    songInfo->setReferenceTrack(jsonSongInfo[KEY_SONG_REFERENCETRACK].toInt());
    songInfo->setReferenceLoopDuration(
                jsonSongInfo[KEY_SONG_REFERENCELOOPDURATION].toInt());

    songInfo->setTempo(jsonSongInfo[KEY_SONG_TEMPO].toInt());
    songInfo->setOffset(jsonSongInfo[KEY_SONG_OFFSET].toInt());

    songInfo->setGain(jsonSongInfo[KEY_SONG_GAIN].toInt());
    songInfo->setLowCutFilterFrequency(
                jsonSongInfo[KEY_SONG_LOWCUTFILTERFREQ].toInt());
    songInfo->setHighCutFilterFrequency(
                jsonSongInfo[KEY_SONG_HIGHCUTFILTERFREQ].toInt());

    songInfo->setScale(jsonSongInfo[KEY_SONG_SCALE].toInt());
    songInfo->setTonality(jsonSongInfo[KEY_SONG_TONALITY].toInt());
    songInfo->setTimeSignature(jsonSongInfo[KEY_SONG_TIMESIGNATURE].toInt());

    return songInfo;
}


int DuSongInfo::getReferenceTrack() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_REFERENCETRACK));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuSongInfo::setReferenceTrack(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_REFERENCETRACK));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuSongInfo::getReferenceLoopDuration() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_REFERENCELOOPDURATION));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuSongInfo::setReferenceLoopDuration(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_REFERENCELOOPDURATION));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuSongInfo::getVolume() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_VOLUME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuSongInfo::setVolume(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_VOLUME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuSongInfo::getTempo() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_TEMPO));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuSongInfo::setTempo(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_TEMPO));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuSongInfo::getOffset() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_OFFSET));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuSongInfo::setOffset(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_OFFSET));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuSongInfo::getClickVolume() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_CLICKVOLUME));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuSongInfo::setClickVolume(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_CLICKVOLUME));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuSongInfo::getGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_GAIN));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuSongInfo::setGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_GAIN));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuSongInfo::getLowCutFilterFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_LOWCUTFILTERFREQ));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuSongInfo::setLowCutFilterFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_LOWCUTFILTERFREQ));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuSongInfo::getHighCutFilterFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_HIGHCUTFILTERFREQ));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuSongInfo::setHighCutFilterFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_HIGHCUTFILTERFREQ));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}


int DuSongInfo::getTimeSignature() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_TIMESIGNATURE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuSongInfo::setTimeSignature(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_TIMESIGNATURE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuSongInfo::getScale() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_SCALE));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuSongInfo::setScale(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_SCALE));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}

int DuSongInfo::getTonality() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_TONALITY));

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

void DuSongInfo::setTonality(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_TONALITY));

    if (tmp == NULL)
        return;

    tmp->setNumeric(value);
}
