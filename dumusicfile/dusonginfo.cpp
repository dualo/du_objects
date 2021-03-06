#include "dusonginfo.h"

#include <cstring>

#include <QJsonObject>

#include "../instrument/effects/dumixer.h"

#include "../general/dubinarydata.h"
#include "../general/dunumeric.h"
#include "../general/dustring.h"

#ifndef NO_MIDI_IMPORT
#include "../miditodumusic/midiconversionhelper.h"
#endif


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


    addChild(KeyQuantification,
             new DuNumeric(0x00));
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
    if (mixer == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT)
                << "DuSongInfo::fromDuMusicBinary():\n"
                << "the DuMixer was not properly generated";

        return DuSongInfoPtr();
    }
    songInfo->setMixer(mixer);

    verif = songInfo->setSongId(du_song.s_id & 0x7FFFFFFF) ? verif : false;
    verif = songInfo->setSongName(DuString::fromStruct(du_song.s_name, MUSIC_SONG_NAME_SIZE)) ? verif : false;
    verif = songInfo->setSongVersion(du_song.s_version_song & 0x7FFFFFFF) ? verif : false;

    verif = songInfo->setReferenceTrack(du_song.s_reftrack) ? verif : false;
    verif = songInfo->setReferenceLoopDuration(static_cast<const int>(du_song.s_looptimer)) ? verif : false;
    verif = songInfo->setCurrentTrack(du_song.s_currenttrack) ? verif : false;

    verif = songInfo->setVolume(du_song.s_volume) ? verif : false;
    verif = songInfo->setTempo(du_song.s_tempo) ? verif : false;
    verif = songInfo->setOffset(du_song.s_decaltempo) ? verif : false;
    verif = songInfo->setClickVolume(du_song.s_voltempo) ? verif : false;

    verif = songInfo->setScale(du_song.s_displaynote) ? verif : false;
    verif = songInfo->setTonality(du_song.s_scaletonality) ? verif : false;
    verif = songInfo->setTimeSignature(du_song.s_timesignature) ? verif : false;

    verif = songInfo->setLeds(QByteArray(reinterpret_cast<const char*>(du_song.s_leds), NUM_LED_VALUE)) ? verif : false;

    verif = songInfo->setReverbPreset(du_song.s_reverb_preset) ? verif : false;

    verif = songInfo->setQuantification(du_song.s_quantification) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuSongInfo::fromDuMusicBinary():\n"
                                     << "an attribute was not properly set";
    }
    
    return songInfo;
}


#ifndef NO_MIDI_IMPORT
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

    verif = songInfo->setQuantification(0) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuSongInfo::fromMidi():\n"
                                     << "an attribute was not properly set";
    }

    return songInfo;
}
#endif


QByteArray DuSongInfo::toDuMusicBinary() const
{
    //NOTE: optimization possible if music_song matched du-objects
    music_song du_songinfo;

    QByteArray tmpArray;
    QString tmpStr;
    int tmpNum = 0;

    std::memset(&du_songinfo, 0, MUSIC_SONG_SIZE);


    const DuMixerConstPtr &mixer = getMixer();
    if (mixer == Q_NULLPTR)
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
    tmpArray.prepend(tmpStr.toLatin1());

    std::memcpy(du_songinfo.s_name, tmpArray.data(), MUSIC_SONG_NAME_SIZE);


    tmpNum = getSongId();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_id = static_cast<quint32>(tmpNum);

    tmpNum = getSongVersion();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_version_song = static_cast<quint32>(tmpNum);
    

    tmpNum = getReferenceTrack();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_reftrack = static_cast<quint8>(tmpNum);

    tmpNum = getReferenceLoopDuration();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_looptimer = static_cast<quint32>(tmpNum);

    tmpNum = getCurrentTrack();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_currenttrack = static_cast<quint8>(tmpNum);


    tmpNum = getVolume();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_volume = static_cast<quint8>(tmpNum);

    tmpNum = getTempo();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_tempo = static_cast<quint8>(tmpNum);

    tmpNum = getClickVolume();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_voltempo = static_cast<quint8>(tmpNum);

    tmpNum = getOffset();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_decaltempo = static_cast<quint8>(tmpNum);

    tmpNum = getScale();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_displaynote = static_cast<quint8>(tmpNum);

    tmpNum = getTonality();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_scaletonality = static_cast<quint8>(tmpNum);

    tmpNum = getTimeSignature();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_timesignature = static_cast<quint8>(tmpNum);

    tmpNum = getReverbPreset();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_reverb_preset = static_cast<quint8>(tmpNum);


    tmpNum = getQuantification();
    if (tmpNum == -1)
        return QByteArray();
    du_songinfo.s_quantification = static_cast<quint8>(tmpNum);


    du_songinfo.s_totalsample = 0x00;


    tmpClear = QByteArray(NUM_LED_VALUE, char(0x00));

    tmpArray = getLeds();
    if (tmpArray.isNull())
        return QByteArray();

    std::memcpy(du_songinfo.s_leds, tmpArray.data(), NUM_LED_VALUE);


    return QByteArray(reinterpret_cast<char*>(&du_songinfo) + SONGINFO_OFFSET, size());
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

DU_KEY_ACCESSORS_IMPL(DuSongInfo, Quantification,        Numeric, int, -1)
