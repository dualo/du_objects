#include "dusonginfo.h"

#include <QJsonObject>
#include <QDebug>

DU_OBJECT_IMPL(DuSongInfo)

DuSongInfo::DuSongInfo() :
    DuContainer()
{
    addChild(KEY_SONG_REFERENCETRACK,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
                           MUSIC_MAXTRACK, 0));

    addChild(KEY_SONG_REFERENCELOOPDURATION,
             new DuNumeric(0));

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

    addChild(KEY_SONG_SCALE,
             new DuNumeric(1, NUMERIC_DEFAULT_SIZE,
                           5, 0));

    addChild(KEY_SONG_TONALITY,
             new DuNumeric(1, NUMERIC_DEFAULT_SIZE,
                           23, 0));

    addChild(KEY_SONG_TIMESIGNATURE,
             new DuNumeric(TIME_4_4, NUMERIC_DEFAULT_SIZE,
                           NUM_TIMESIGNATURE, TIME_OFF));
}

DuSongInfo::~DuSongInfo()
{
}

DuObjectPtr DuSongInfo::clone() const
{
    return DuSongInfoPtr(new DuSongInfo(*this));
}


DuSongInfoPtr DuSongInfo::fromDuMusicBinary(const music_song &du_song)
{
    DuSongInfoPtr songInfo(new DuSongInfo);
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

    verif = verif && songInfo->setScale(du_song.s_displaynote);
    verif = verif && songInfo->setTonality(du_song.s_scaletonality);
    verif = verif && songInfo->setTimeSignature(du_song.s_timesignature);

    if (!verif)
    {
        qCritical() << "DuSongInfo::fromDuMusicBinary():\n"
                    << "failed to generate DuSongInfo\n"
                    << "a child was not set properly";

        return DuSongInfoPtr();
    }

    return songInfo;
}


DuSongInfoPtr DuSongInfo::fromJson(const QJsonObject &jsonSongInfo)
{
    QJsonValue jsonRefTrack         = jsonSongInfo[KEY_SONG_REFERENCETRACK];
    QJsonValue jsonRefDuration      = jsonSongInfo[KEY_SONG_REFERENCELOOPDURATION];
    QJsonValue jsonVolume           = jsonSongInfo[KEY_SONG_VOLUME];
    QJsonValue jsonTempo            = jsonSongInfo[KEY_SONG_TEMPO];
    QJsonValue jsonClickVolume      = jsonSongInfo[KEY_SONG_CLICKVOLUME];
    QJsonValue jsonOffset           = jsonSongInfo[KEY_SONG_OFFSET];
    QJsonValue jsonGain             = jsonSongInfo[KEY_SONG_GAIN];
    QJsonValue jsonLoCutFreq        = jsonSongInfo[KEY_SONG_LOWCUTFILTERFREQ];
    QJsonValue jsonHiCutFreq        = jsonSongInfo[KEY_SONG_HIGHCUTFILTERFREQ];
    QJsonValue jsonScale            = jsonSongInfo[KEY_SONG_SCALE];
    QJsonValue jsonTonality         = jsonSongInfo[KEY_SONG_TONALITY];
    QJsonValue jsonTimeSignature    = jsonSongInfo[KEY_SONG_TIMESIGNATURE];

    if (        !jsonRefTrack.isDouble()    ||  !jsonRefDuration.isDouble()
            ||  !jsonVolume.isDouble()      ||  !jsonTempo.isDouble()
            ||  !jsonClickVolume.isDouble() ||  !jsonOffset.isDouble()
            ||  !jsonGain.isDouble()        ||  !jsonLoCutFreq.isDouble()
            ||  !jsonHiCutFreq.isDouble()   ||  !jsonScale.isDouble()
            ||  !jsonTonality.isDouble()    ||  !jsonTimeSignature.isDouble())
    {
        qCritical() << "DuSongInfo::fromJson():\n"
                    << "failed to generate DuSongInfo\n"
                    << "a json key did not contain the proper type";

        return DuSongInfoPtr();
    }


    DuSongInfoPtr songInfo(new DuSongInfo);
    bool verif = true;

    verif = verif && songInfo->setReferenceTrack(jsonRefTrack.toInt());
    verif = verif && songInfo->setReferenceLoopDuration(jsonRefDuration.toInt());

    verif = verif && songInfo->setVolume(jsonVolume.toInt());
    verif = verif && songInfo->setTempo(jsonTempo.toInt());
    verif = verif && songInfo->setClickVolume(jsonClickVolume.toInt());
    verif = verif && songInfo->setOffset(jsonOffset.toInt());

    verif = verif && songInfo->setGain(jsonGain.toInt());
    verif = verif && songInfo->setLowCutFilterFrequency(jsonLoCutFreq.toInt());
    verif = verif && songInfo->setHighCutFilterFrequency(jsonHiCutFreq.toInt());

    verif = verif && songInfo->setScale(jsonScale.toInt());
    verif = verif && songInfo->setTonality(jsonTonality.toInt());
    verif = verif && songInfo->setTimeSignature(jsonTimeSignature.toInt());

    if (!verif)
    {
        qCritical() << "DuSongInfo::fromJson():\n"
                    << "failed to generate DuSongInfo\n"
                    << "a child was not set properly";

        return DuSongInfoPtr();
    }

    return songInfo;
}


QByteArray DuSongInfo::toDuMusicBinary() const
{
    //TODO: restructure music_song into music_header and music_info
    music_song du_songinfo;

    int tmpNum = 0;

    QByteArray tmpClear(MUSIC_SONG_SIZE, (char)0x00);
    std::memcpy((char *)&(du_songinfo), tmpClear.data(), MUSIC_SONG_SIZE);


    tmpNum = getReferenceTrack();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_reftrack = tmpNum;

    tmpNum = getReferenceLoopDuration();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_looptimer = tmpNum;

    tmpNum = getVolume();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_volume = tmpNum;

    tmpNum = getTempo();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_tempo = tmpNum;

    tmpNum = getClickVolume();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_voltempo = tmpNum;

    tmpNum = getOffset();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_decaltempo = tmpNum;

    tmpNum = getGain();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_mix.m_inputgain = tmpNum;

    tmpNum = getLowCutFilterFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_mix.m_locutfilterfrequency = tmpNum;

    tmpNum = getHighCutFilterFrequency();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_mix.m_hicutfilterfrequency = tmpNum;

    tmpNum = getScale();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_displaynote = tmpNum;

    tmpNum = getTonality();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_scaletonality = tmpNum;

    tmpNum = getTimeSignature();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_timesignature = tmpNum;


    du_songinfo.s_currenttrack = 0x00;
    du_songinfo.s_quantification = 0x00;
    du_songinfo.s_totalsample = 0x00;

    du_songinfo.s_mix.m_ouputlevel = 0x7F;
    du_songinfo.s_mix.m_outputpanning = 0x40;
    du_songinfo.s_mix.m_ouputfrontrear = 0x00;

    du_songinfo.s_mix.m_sendtoreverb = 0x00;
    du_songinfo.s_mix.m_sendtochorus = 0x00;

    for(int i = 0; i < NUM_LED_VALUE; i++)
    {
        du_songinfo.s_leds[i] = 0x00;
    }


    return QByteArray((char *)&(du_songinfo), MUSIC_SONG_SIZE);
}


int DuSongInfo::size() const
{
    //TODO: add defines for dummy sizes in music_parameters_mng.h
    return SONGINFO_SIZE;
}


int DuSongInfo::getReferenceTrack() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_REFERENCETRACK);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setReferenceTrack(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_SONG_REFERENCETRACK);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getReferenceLoopDuration() const
{
    const DuNumericConstPtr &tmp =
            getChildAs<DuNumeric>(KEY_SONG_REFERENCELOOPDURATION);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setReferenceLoopDuration(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_SONG_REFERENCELOOPDURATION);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuSongInfo::getVolume() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_VOLUME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setVolume(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_SONG_VOLUME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getTempo() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_TEMPO);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setTempo(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_SONG_TEMPO);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getClickVolume() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_CLICKVOLUME);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setClickVolume(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_SONG_CLICKVOLUME);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getOffset() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_OFFSET);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setOffset(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_SONG_OFFSET);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuSongInfo::getGain() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_GAIN);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setGain(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_SONG_GAIN);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getLowCutFilterFrequency() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_LOWCUTFILTERFREQ);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setLowCutFilterFrequency(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_SONG_LOWCUTFILTERFREQ);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getHighCutFilterFrequency() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_HIGHCUTFILTERFREQ);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setHighCutFilterFrequency(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_SONG_HIGHCUTFILTERFREQ);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuSongInfo::getScale() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_SCALE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setScale(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_SONG_SCALE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getTonality() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_TONALITY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setTonality(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_SONG_TONALITY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}

int DuSongInfo::getTimeSignature() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_TIMESIGNATURE);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuSongInfo::setTimeSignature(int value)
{
    DuNumericPtr tmp = getChildAs<DuNumeric>(KEY_SONG_TIMESIGNATURE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
