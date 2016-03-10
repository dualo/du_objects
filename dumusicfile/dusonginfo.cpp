#include "dusonginfo.h"

#include <cstring>

#include <QJsonObject>

#include "../instrument/effects/dumixer.h"

#include "../general/dubinarydata.h"
#include "../general/dunumeric.h"
#include "../general/dustring.h"

#include "../miditodumusic/midiconversionhelper.h"


DU_OBJECT_IMPL(DuSongInfo)

DuSongInfo::DuSongInfo() :
    DuContainer()
{
    addChild(KeySongId, new DuNumeric(0));

    addChild(KeySongName, new DuString(MUSIC_SONG_NAME_SIZE));

    addChild(KeySongVersion, new DuNumeric(0));

    addChild(KeyReferenceTrack,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
                           MUSIC_MAXTRACK, 0));

    addChild(KeyReferenceLoopDuration,
             new DuNumeric(0));

    addChild(KeyCurrentTrack,
             new DuNumeric(0, NUMERIC_DEFAULT_SIZE,
                           MUSIC_MAXTRACK, 0));

    addChild(KeyVolume,
             new DuNumeric(MUSIC_VOL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           MUSIC_VOL_MAXVALUE, MUSIC_VOL_MINVALUE));

    addChild(KeyTempo,
             new DuNumeric(MUSIC_TEMPO_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           MUSIC_TEMPO_MAXVALUE, MUSIC_TEMPO_MINVALUE));

    addChild(KeyOffset,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0xFF, 0x00));

    addChild(KeyClickVolume,
             new DuNumeric(MUSIC_TEMPOVOL_DEFAULTVALUE, NUMERIC_DEFAULT_SIZE,
                           MUSIC_TEMPOVOL_MAXVALUE, MUSIC_TEMPOVOL_MINVALUE));

    addChild(KeyMixer, new DuMixer());


    addChild(KeyScale,
             new DuNumeric(MAJOR_LED_MODE, NUMERIC_DEFAULT_SIZE,
                           NUM_LED_MODE - 1, NONE_LED_MODE));

    addChild(KeyTonality,
             new DuNumeric(1, NUMERIC_DEFAULT_SIZE,
                           23, 0));

    addChild(KeyTimeSignature,
             new DuNumeric(TIME_4_4, NUMERIC_DEFAULT_SIZE,
                           NUM_TIMESIGNATURE - 1, TIME_OFF));

    addChild(KeyReverbPreset,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0x7F, 0x00));

    addChild(KeyLeds,
             new DuBinaryData(NUM_LED_VALUE));


    addChild(KeySwing,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0xFF, 0x00));

    addChild(KeyQuantification,
             new DuNumeric(0x00, NUMERIC_DEFAULT_SIZE,
                           0xFF, 0x00));
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

    verif = songInfo->setSongId(du_song.s_id & 0x7FFFFFFF) ? verif : false;
    verif = songInfo->setSongName(QString(QByteArray((char *)du_song.s_name, MUSIC_SONG_NAME_SIZE))) ? verif : false;
    verif = songInfo->setSongVersion(du_song.s_version_song & 0x7FFFFFFF) ? verif : false;

    verif = songInfo->setReferenceTrack(du_song.s_reftrack) ? verif : false;
    verif = songInfo->setReferenceLoopDuration(du_song.s_looptimer) ? verif : false;
    verif = songInfo->setCurrentTrack(du_song.s_currenttrack) ? verif : false;

    verif = songInfo->setVolume(du_song.s_volume) ? verif : false;
    verif = songInfo->setTempo(du_song.s_tempo) ? verif : false;
    verif = songInfo->setOffset(du_song.s_decaltempo) ? verif : false;
    verif = songInfo->setClickVolume(du_song.s_voltempo) ? verif : false;

    verif = songInfo->setScale(du_song.s_displaynote) ? verif : false;
    verif = songInfo->setTonality(du_song.s_scaletonality) ? verif : false;
    verif = songInfo->setTimeSignature(du_song.s_timesignature) ? verif : false;

    verif = songInfo->setLeds(QByteArray((char *)du_song.s_leds, NUM_LED_VALUE)) ? verif : false;

    verif = songInfo->setReverbPreset(du_song.s_reverb_preset) ? verif : false;

    verif = songInfo->setSwing(du_song.s_swing) ? verif : false;
    verif = songInfo->setQuantification(du_song.s_quantification) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuSongInfo::fromDuMusicBinary():\n"
                                     << "an attribute was not properly set";
    }
    
    return songInfo;
}


DuSongInfoPtr DuSongInfo::fromJson(const QJsonObject &jsonSongInfo)
{
    QJsonValue jsonSongId           = jsonSongInfo[KeySongId];
    QJsonValue jsonSongName         = jsonSongInfo[KeySongName];
    QJsonValue jsonSongVersion      = jsonSongInfo[KeySongVersion];
    QJsonValue jsonRefTrack         = jsonSongInfo[KeyReferenceTrack];
    QJsonValue jsonRefDuration      = jsonSongInfo[KeyReferenceLoopDuration];
    QJsonValue jsonCurrentTrack     = jsonSongInfo[KeyCurrentTrack];
    QJsonValue jsonVolume           = jsonSongInfo[KeyVolume];
    QJsonValue jsonTempo            = jsonSongInfo[KeyTempo];
    QJsonValue jsonClickVolume      = jsonSongInfo[KeyClickVolume];
    QJsonValue jsonOffset           = jsonSongInfo[KeyOffset];
    QJsonValue jsonMixer            = jsonSongInfo[KeyMixer];
    QJsonValue jsonScale            = jsonSongInfo[KeyScale];
    QJsonValue jsonTonality         = jsonSongInfo[KeyTonality];
    QJsonValue jsonTimeSignature    = jsonSongInfo[KeyTimeSignature];
    QJsonValue jsonReverbPreset     = jsonSongInfo[KeyReverbPreset];
    QJsonValue jsonLeds             = jsonSongInfo[KeyLeds];
    QJsonValue jsonQuantif          = jsonSongInfo[KeyQuantification];
    QJsonValue jsonSwing            = jsonSongInfo[KeySwing];

    if (        !jsonSongId.isDouble()      ||  !jsonSongName.isString()
            ||  !jsonSongVersion.isDouble()
            ||  !jsonRefTrack.isDouble()    ||  !jsonRefDuration.isDouble()
            ||  !jsonCurrentTrack.isDouble()
            ||  !jsonVolume.isDouble()      ||  !jsonTempo.isDouble()
            ||  !jsonClickVolume.isDouble() ||  !jsonOffset.isDouble()
            ||  !jsonMixer.isObject()       ||  !jsonScale.isDouble()
            ||  !jsonTonality.isDouble()    ||  !jsonTimeSignature.isDouble()
            ||  !jsonLeds.isString()        ||  !jsonReverbPreset.isDouble()
            ||  !jsonQuantif.isDouble()     ||  !jsonSwing.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuSongInfo::fromJson():\n"
                << "failed to generate DuSongInfo\n"
                << "a json key did not contain the proper type";

        return DuSongInfoPtr();
    }


    DuSongInfoPtr songInfo(new DuSongInfo);
    bool verif = true;

    verif = songInfo->setSongId(jsonSongId.toInt()) ? verif : false;
    verif = songInfo->setSongName(jsonSongName.toString()) ? verif : false;
    verif = songInfo->setSongVersion(jsonSongVersion.toInt()) ? verif : false;

    verif = songInfo->setReferenceTrack(jsonRefTrack.toInt()) ? verif : false;
    verif = songInfo->setReferenceLoopDuration(jsonRefDuration.toInt()) ? verif : false;
    verif = songInfo->setCurrentTrack(jsonCurrentTrack.toInt()) ? verif : false;

    verif = songInfo->setVolume(jsonVolume.toInt()) ? verif : false;
    verif = songInfo->setTempo(jsonTempo.toInt()) ? verif : false;
    verif = songInfo->setClickVolume(jsonClickVolume.toInt()) ? verif : false;
    verif = songInfo->setOffset(jsonOffset.toInt()) ? verif : false;

    verif = songInfo->setScale(jsonScale.toInt()) ? verif : false;
    verif = songInfo->setTonality(jsonTonality.toInt()) ? verif : false;
    verif = songInfo->setTimeSignature(jsonTimeSignature.toInt()) ? verif : false;

    verif = songInfo->setLeds(jsonLeds.toString().toUtf8()) ? verif : false;

    verif = songInfo->setReverbPreset(jsonReverbPreset.toInt()) ? verif : false;

    verif = songInfo->setSwing(jsonSwing.toInt()) ? verif : false;
    verif = songInfo->setQuantification(jsonQuantif.toInt()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT)
                << "DuSongInfo::fromJson():\n"
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
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuMusic::fromMidi():\n"
                << "failed to generate DuMusic\n"
                << "invalid conversion helper";

        return DuSongInfoPtr();
    }

    DuSongInfoPtr songInfo(new DuSongInfo);
    bool verif = true;

    verif = songInfo->setSongName(helper.getTitle()) ? verif : false;

    verif = songInfo->setReferenceTrack(helper.getIndexes(0).first) ? verif : false;
    verif = songInfo->setReferenceLoopDuration(helper.getDuration()) ? verif : false;
    verif = songInfo->setCurrentTrack(helper.getIndexes(0).first) ? verif : false;

//    verif = songInfo->setVolume() ? verif : false;
    verif = songInfo->setTempo(helper.getTempo()) ? verif : false;
//    verif = songInfo->setClickVolume() ? verif : false;
//    verif = songInfo->setOffset(jsonOffset.toInt()) ? verif : false;

    verif = songInfo->setScale(helper.getDutouchScale()) ? verif : false;
    verif = songInfo->setTonality(helper.getTonality()) ? verif : false;
    verif = songInfo->setTimeSignature(helper.getTimeSig()) ? verif : false;

//    verif = songInfo->setLeds() ? verif : false;

    verif = songInfo->setSwing(0) ? verif : false;
    verif = songInfo->setQuantification(0) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuSongInfo::fromMidi():\n"
                                     << "an attribute was not properly set";
    }

    return songInfo;
}


QByteArray DuSongInfo::toDuMusicBinary() const
{
    //NOTE: optimization possible if music_song matched du-objects
    music_song du_songinfo;

    QByteArray tmpArray;
    QString tmpStr;
    int tmpNum = 0;

    std::memset((char*)&du_songinfo, 0, MUSIC_SONG_SIZE);


    const DuMixerConstPtr &mixer = getMixer();
    if (mixer == NULL)
        return QByteArray();
    const QByteArray &mixerArray = mixer->toDuMusicBinary();
    if (mixerArray.isNull())
        return QByteArray();

    std::memcpy(&(du_songinfo.s_mix), mixerArray.data(), FX_MIX_SIZE);


    QByteArray tmpClear(MUSIC_SONG_NAME_SIZE, char(0x00));

    tmpArray = tmpClear;
    tmpStr = getSongName();
    if (tmpStr.isNull())
        return QByteArray();
    tmpArray.prepend(tmpStr.toUtf8());

    std::memcpy(du_songinfo.s_name, tmpArray.data(), MUSIC_SONG_NAME_SIZE);


    tmpNum = getSongId();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_id = tmpNum;

    tmpNum = getSongVersion();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_version_song = tmpNum;
    

    tmpNum = getReferenceTrack();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_reftrack = tmpNum;

    tmpNum = getReferenceLoopDuration();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_looptimer = tmpNum;

    tmpNum = getCurrentTrack();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_currenttrack = tmpNum;


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

    tmpNum = getReverbPreset();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_reverb_preset = tmpNum;


    tmpNum = getSwing();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_swing = tmpNum;

    tmpNum = getQuantification();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_quantification = tmpNum;


    du_songinfo.s_totalsample = 0x00;


    tmpClear = QByteArray(NUM_LED_VALUE, char(0x00));

    tmpArray = getLeds();
    if (tmpArray.isNull())
        return QByteArray();

    std::memcpy(du_songinfo.s_leds, tmpArray.data(), NUM_LED_VALUE);


    return QByteArray((char *)&(du_songinfo) + SONGINFO_OFFSET, size());
}


int DuSongInfo::size() const
{
    return SONGINFO_SIZE;
}

DU_KEY_ACCESSORS_IMPL(DuSongInfo, SongId,                Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSongInfo, SongName,              String, QString, QString())
DU_KEY_ACCESSORS_IMPL(DuSongInfo, SongVersion,           Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSongInfo, ReferenceTrack,        Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSongInfo, ReferenceLoopDuration, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSongInfo, CurrentTrack,          Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSongInfo, Volume,                Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSongInfo, Tempo,                 Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSongInfo, Offset,                Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSongInfo, ClickVolume,           Numeric, int, -1)

DU_KEY_ACCESSORS_OBJECT_IMPL(DuSongInfo, Mixer, DuMixer)

DU_KEY_ACCESSORS_IMPL(DuSongInfo, TimeSignature,         Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSongInfo, Scale,                 Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSongInfo, Tonality,              Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSongInfo, ReverbPreset,          Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuSongInfo, Leds,                  BinaryData, QByteArray, QByteArray())

DU_KEY_ACCESSORS_IMPL(DuSongInfo, Swing,                 Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSongInfo, Quantification,        Numeric, int, -1)
