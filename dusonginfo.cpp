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
    DuSongInfo *songInfo = new DuSongInfo;
    bool verif = true;

    verif = verif && songInfo->setReferenceTrack(du_song.s_reftrack);
    verif = verif && songInfo->setReferenceLoopDuration(du_song.s_looptimer);

    verif = verif && songInfo->setVolume(du_song.s_volume);
    verif = verif && songInfo->setTempo(du_song.s_tempo);
    verif = verif && songInfo->setOffset(du_song.s_decaltempo);
    verif = verif && songInfo->setClickVolume(du_song.s_voltempo);

    verif = verif && songInfo->setGain(du_song.s_mix.m_inputgain);
    verif = verif && songInfo->setLowCutFilterFrequency(
                du_song.s_mix.m_locutfilterfrequency);
    verif = verif && songInfo->setHighCutFilterFrequency(
                du_song.s_mix.m_hicutfilterfrequency);

    verif = verif && songInfo->setTimeSignature(du_song.s_timesignature);
    verif = verif && songInfo->setScale(du_song.s_displaynote);
    verif = verif && songInfo->setTonality(du_song.s_scaletonality);

    if (!verif)
    {
        delete songInfo;
        return NULL;
    }

    return songInfo;
}


DuSongInfo *DuSongInfo::fromJson(const QJsonObject &jsonSongInfo)
{
    QJsonValue jsonRefTrack         = jsonSongInfo[KEY_SONG_REFERENCETRACK];
    QJsonValue jsonRefDuration      = jsonSongInfo[KEY_SONG_REFERENCELOOPDURATION];
    QJsonValue jsonTempo            = jsonSongInfo[KEY_SONG_TEMPO];
    QJsonValue jsonOffset           = jsonSongInfo[KEY_SONG_OFFSET];
    QJsonValue jsonGain             = jsonSongInfo[KEY_SONG_GAIN];
    QJsonValue jsonLoCutFreq        = jsonSongInfo[KEY_SONG_LOWCUTFILTERFREQ];
    QJsonValue jsonHiCutFreq        = jsonSongInfo[KEY_SONG_HIGHCUTFILTERFREQ];
    QJsonValue jsonScale            = jsonSongInfo[KEY_SONG_SCALE];
    QJsonValue jsonTonality         = jsonSongInfo[KEY_SONG_TONALITY];
    QJsonValue jsonTimeSignature    = jsonSongInfo[KEY_SONG_TIMESIGNATURE];

    if (!jsonRefTrack.isDouble()    ||  !jsonRefDuration.isDouble()
            ||  !jsonTempo.isDouble()       ||  !jsonOffset.isDouble()
            ||  !jsonGain.isDouble()        ||  !jsonLoCutFreq.isDouble()
            ||  !jsonHiCutFreq.isDouble()   ||  !jsonScale.isDouble()
            ||  !jsonTonality.isDouble()    ||  !jsonTimeSignature.isDouble())

        return NULL;


    DuSongInfo *songInfo = new DuSongInfo;
    bool verif = true;

    verif = verif && songInfo->setReferenceTrack(jsonRefTrack.toInt());
    verif = verif && songInfo->setReferenceLoopDuration(jsonRefDuration.toInt());

    verif = verif && songInfo->setTempo(jsonTempo.toInt());
    verif = verif && songInfo->setOffset(jsonOffset.toInt());

    verif = verif && songInfo->setGain(jsonGain.toInt());
    verif = verif && songInfo->setLowCutFilterFrequency(jsonLoCutFreq.toInt());
    verif = verif && songInfo->setHighCutFilterFrequency(jsonHiCutFreq.toInt());

    verif = verif && songInfo->setScale(jsonScale.toInt());
    verif = verif && songInfo->setTonality(jsonTonality.toInt());
    verif = verif && songInfo->setTimeSignature(jsonTimeSignature.toInt());

    if (!verif)
    {
        delete songInfo;
        return NULL;
    }

    return songInfo;
}


int DuSongInfo::size() const
{
    //TODO: add defines for dummy sizes in music_parameters_mng.h
    return SONGINFO_SIZE;
}


int DuSongInfo::getReferenceTrack() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_REFERENCETRACK));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setReferenceTrack(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_REFERENCETRACK));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getReferenceLoopDuration() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_REFERENCELOOPDURATION));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setReferenceLoopDuration(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_REFERENCELOOPDURATION));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuSongInfo::getVolume() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_VOLUME));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setVolume(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_VOLUME));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getTempo() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_TEMPO));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setTempo(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_TEMPO));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getOffset() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_OFFSET));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setOffset(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_OFFSET));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getClickVolume() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_CLICKVOLUME));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setClickVolume(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_CLICKVOLUME));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuSongInfo::getGain() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_GAIN));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setGain(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_GAIN));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getLowCutFilterFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_LOWCUTFILTERFREQ));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setLowCutFilterFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_LOWCUTFILTERFREQ));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getHighCutFilterFrequency() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_HIGHCUTFILTERFREQ));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setHighCutFilterFrequency(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_HIGHCUTFILTERFREQ));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuSongInfo::getTimeSignature() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_TIMESIGNATURE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setTimeSignature(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_TIMESIGNATURE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getScale() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_SCALE));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setScale(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_SCALE));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getTonality() const
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_TONALITY));

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setTonality(int value)
{
    DuNumeric *tmp = dynamic_cast<DuNumeric *>(getChild(KEY_SONG_TONALITY));

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
