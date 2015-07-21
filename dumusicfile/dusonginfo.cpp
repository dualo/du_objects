#include "dusonginfo.h"

#include <cstring>

#include <QJsonObject>

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

    addChild(KEY_SONG_MIXER, new DuMixer());


    addChild(KEY_SONG_SCALE,
             new DuNumeric(MAJOR_LED_MODE, NUMERIC_DEFAULT_SIZE,
                           NUM_LED_MODE, NONE_LED_MODE));

    addChild(KEY_SONG_TONALITY,
             new DuNumeric(1, NUMERIC_DEFAULT_SIZE,
                           23, 0));

    addChild(KEY_SONG_TIMESIGNATURE,
             new DuNumeric(TIME_4_4, NUMERIC_DEFAULT_SIZE,
                           NUM_TIMESIGNATURE, TIME_OFF));

    addChild(KEY_SONG_LEDS,
             new DuBinaryData(NUM_LED_VALUE));
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

    const DuMixerPtr &mixer = DuMixer::fromDuMusicBinary(du_song.s_mix);
    if (mixer == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuSongInfo::fromDuMusicBinary():\n"
                << "the DuMixer was not properly generated";

        return DuSongInfoPtr();
    }
    songInfo->setMixer(mixer);

    verif = songInfo->setReferenceTrack(du_song.s_reftrack) ? verif : false;
    verif = songInfo->setReferenceLoopDuration(du_song.s_looptimer) ? verif : false;

    verif = songInfo->setVolume(du_song.s_volume) ? verif : false;
    verif = songInfo->setTempo(du_song.s_tempo) ? verif : false;
    verif = songInfo->setOffset(du_song.s_decaltempo) ? verif : false;
    verif = songInfo->setClickVolume(du_song.s_voltempo) ? verif : false;

    verif = songInfo->setScale(du_song.s_displaynote) ? verif : false;
    verif = songInfo->setTonality(du_song.s_scaletonality) ? verif : false;
    verif = songInfo->setTimeSignature(du_song.s_timesignature) ? verif : false;

    verif = songInfo->setLeds(QByteArray((char *)du_song.s_leds, NUM_LED_VALUE)) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuSongInfo::fromDuMusicBinary():\n"
                                     << "an attribute was not properly set";
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
    QJsonValue jsonMixer            = jsonSongInfo[KEY_SONG_MIXER];
    QJsonValue jsonScale            = jsonSongInfo[KEY_SONG_SCALE];
    QJsonValue jsonTonality         = jsonSongInfo[KEY_SONG_TONALITY];
    QJsonValue jsonTimeSignature    = jsonSongInfo[KEY_SONG_TIMESIGNATURE];
    QJsonValue jsonLeds             = jsonSongInfo[KEY_SONG_LEDS];

    if (        !jsonRefTrack.isDouble()    ||  !jsonRefDuration.isDouble()
            ||  !jsonVolume.isDouble()      ||  !jsonTempo.isDouble()
            ||  !jsonClickVolume.isDouble() ||  !jsonOffset.isDouble()
            ||  !jsonMixer.isObject()       ||  !jsonScale.isDouble()
            ||  !jsonTonality.isDouble()    ||  !jsonTimeSignature.isDouble()
            ||  !jsonLeds.isString())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuSongInfo::fromJson():\n"
                << "failed to generate DuSongInfo\n"
                << "a json key did not contain the proper type";

        return DuSongInfoPtr();
    }


    DuSongInfoPtr songInfo(new DuSongInfo);
    bool verif = true;

    verif = songInfo->setReferenceTrack(jsonRefTrack.toInt()) ? verif : false;
    verif = songInfo->setReferenceLoopDuration(jsonRefDuration.toInt()) ? verif : false;

    verif = songInfo->setVolume(jsonVolume.toInt()) ? verif : false;
    verif = songInfo->setTempo(jsonTempo.toInt()) ? verif : false;
    verif = songInfo->setClickVolume(jsonClickVolume.toInt()) ? verif : false;
    verif = songInfo->setOffset(jsonOffset.toInt()) ? verif : false;

    verif = songInfo->setScale(jsonScale.toInt()) ? verif : false;
    verif = songInfo->setTonality(jsonTonality.toInt()) ? verif : false;
    verif = songInfo->setTimeSignature(jsonTimeSignature.toInt()) ? verif : false;

    verif = songInfo->setLeds(jsonLeds.toString().toUtf8()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuSongInfo::fromJson():\n"
                                     << "an attribute was not properly set";
    }

    const DuMixerPtr &mixer = DuMixer::fromJson(jsonMixer.toObject());
    if (mixer == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuSongInfo::fromJson():\n"
                << "the DuMixer was not properly generated";

        return DuSongInfoPtr();
    }
    songInfo->setMixer(mixer);

    return songInfo;
}


DuSongInfoPtr DuSongInfo::fromMidi(const MidiConversionHelper &helper)
{
    if (!helper.isValid())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuMusic::fromMidi():\n"
                                      << "failed to generate DuMusic\n"
                                      << "invalid conversion helper";

        return DuSongInfoPtr();
    }

    DuSongInfoPtr songInfo(new DuSongInfo);
    bool verif = true;

    verif = songInfo->setReferenceTrack(helper.getIndexes(0).first) ? verif : false;
    verif = songInfo->setReferenceLoopDuration(helper.getDuration()) ? verif : false;

//    verif = songInfo->setVolume() ? verif : false;
    verif = songInfo->setTempo(helper.getTempo()) ? verif : false;
//    verif = songInfo->setClickVolume() ? verif : false;
//    verif = songInfo->setOffset(jsonOffset.toInt()) ? verif : false;

    verif = songInfo->setScale(helper.getScale()) ? verif : false;
    verif = songInfo->setTonality(helper.getTonality()) ? verif : false;
    verif = songInfo->setTimeSignature(helper.getTimeSig()) ? verif : false;

//    verif = songInfo->setLeds() ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuSongInfo::fromMidi():\n"
                                     << "an attribute was not properly set";
    }

    return songInfo;
}


QByteArray DuSongInfo::toDuMusicBinary() const
{
    //TODO: restructure music_song into music_header and music_info
    music_song du_songinfo;

    QByteArray tmpArray;
    QString tmpStr;
    int tmpNum = 0;

    QByteArray tmpClear(MUSIC_SONG_SIZE, (char)0x00);
    std::memcpy((char *)&(du_songinfo), tmpClear.data(), MUSIC_SONG_SIZE);


    const DuMixerConstPtr &mixer = getMixer();
    if (mixer == NULL)
        return QByteArray();
    const QByteArray &mixerArray = mixer->toDuMusicBinary();
    if (mixerArray.isNull())
        return QByteArray();

    std::memcpy(&(du_songinfo.s_mix), mixerArray.data(), FX_MIX_SIZE);


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


    tmpClear = QByteArray(NUM_LED_VALUE, char(0x00));

    tmpArray = getLeds();
    if (tmpArray.isNull())
        return QByteArray();

    std::memcpy(du_songinfo.s_leds, tmpArray.data(), NUM_LED_VALUE);


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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_REFERENCETRACK);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_REFERENCELOOPDURATION);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_VOLUME);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_TEMPO);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_CLICKVOLUME);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_OFFSET);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


DuMixerConstPtr DuSongInfo::getMixer() const
{
    return getChildAs<DuMixer>(KEY_SONG_MIXER);
}

void DuSongInfo::setMixer(const DuMixerPtr &mixer)
{
    addChild(KEY_SONG_MIXER, mixer);
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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_SCALE);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_TONALITY);

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
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_SONG_TIMESIGNATURE);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


QByteArray DuSongInfo::getLeds() const
{
    const DuBinaryDataConstPtr &tmp = getChildAs<DuBinaryData>(KEY_SONG_LEDS);

    if (tmp == NULL)
        return QByteArray();

    return tmp->getData();
}

bool DuSongInfo::setLeds(const QByteArray &value)
{
    const DuBinaryDataPtr &tmp = getChildAs<DuBinaryData>(KEY_SONG_LEDS);

    if (tmp == NULL)
        return false;

    return tmp->setData(value);
}
