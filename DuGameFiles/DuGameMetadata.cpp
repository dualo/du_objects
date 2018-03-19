#include "DuGameMetadata.h"

#include "DuGameEvent.h"

#pragma pack(push, 4)
#include "../du-touch/dualo_structs/metadata_structs.h"
#include "../du-touch/dualo_structs/dugame_structs.h"
#pragma pack(pop)

#include "../general/duarray.h"
#include "../general/dunumeric.h"

#include <cstring>

#include <QDataStream>

#include "../instrument/DuSystemSoundIdentifier.h"

DU_OBJECT_IMPL(DuGameMetadata)

DuGameMetadata::DuGameMetadata() : DuContainer()
{
    addChild(KeyGrade, new DuNumeric);
    addChild(KeyUnlockerEvent, new DuNumeric);
    addChild(KeyVersion, new DuNumeric);
    addChild(KeyGameId, new DuNumeric);

    DuArrayPtr<DuSystemSoundIdentifier> soundsArray(new DuArray<DuSystemSoundIdentifier>(MAX_DUGAME_SOUND));
    for (int i = 0; i < MAX_DUGAME_SOUND; ++i)
        soundsArray->append(new DuSystemSoundIdentifier);
    addChild(KeySounds, soundsArray);

    addChild(KeyEvents, new DuArray<DuGameEvent>);
}

DuGameMetadataPtr DuGameMetadata::fromBinary(const QByteArray &data, quint32 version)
{
    if (version < MUSICMETADATA_GAME_CURRENT_VERSION)
    {
        // migrate
    }

    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    s_dugame gameStruct;
    stream.readRawData(reinterpret_cast<char*>(&gameStruct), DUGAME_HEADER);

    const int expectedSize = DUGAME_HEADER + (gameStruct.dg_numevent * ARRANGEMENT_EVENT_SIZE);
    if (data.size() != expectedSize)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameMetadata: data size incorrect -> expected" << expectedSize << ", got" << data.size();
        return {};
    }

    DuGameMetadataPtr game(new DuGameMetadata);

    DuArrayPtr<DuSystemSoundIdentifier> soundsArray(new DuArray<DuSystemSoundIdentifier>(MAX_DUGAME_SOUND));
    for (uint i = 0; i < MAX_DUGAME_SOUND; ++i)
    {
        const s_dugame_sound& soundStruct = gameStruct.dg_sound[i];

        const DuSystemSoundIdentifierPtr& sound = DuSystemSoundIdentifier::fromBinary(soundStruct);
        if (sound == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameMetadata: sound" << i << "corrupted";
            return {};
        }

        if (!soundsArray->append(sound))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameMetadata: the sound" << i << "could not be appended";
            return {};
        }
    }
    game->setSounds(soundsArray);

    DuArrayPtr<DuGameEvent> eventsArray(new DuArray<DuGameEvent>);
    for (uint i = 0; i < gameStruct.dg_numevent; ++i)
    {
        s_arrangement_event eventStruct;
        stream.readRawData(reinterpret_cast<char*>(&eventStruct), ARRANGEMENT_EVENT_SIZE);

        const DuGameEventPtr& event = DuGameEvent::fromStruct(eventStruct);
        if (event == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameMetadata: event" << i << "corrupted";
            return {};
        }

        // MIGRATE EVENTS FOR DU-GAME VERSION 1
        if (version <= 1)
        {
            event->setWaitForLoopStart(0xFF);
        }

        // MIGRATE EVENTS FOR DU-GAME VERSION 2
        if (version <= 2)
        {
            event->setNextEvent(i==(gameStruct.dg_numevent-1) ? 0xFF : i+1);
            event->setBackwardEvent(i==0 ? 0 : i-1);
            event->setForwardEvent(i==(gameStruct.dg_numevent-1) ? 0xFF : i+1);
        }

        if (!eventsArray->append(event))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameMetadata: the event" << i << "could not be appended";
            return {};
        }
    }
    game->setEvents(eventsArray);

    bool verif = true;

    verif = game->setGrade(gameStruct.dg_grade) ? verif : false;
    verif = game->setUnlockerEvent(gameStruct.dg_unlocker_event) ? verif : false;
    verif = game->setVersion(gameStruct.dg_version) ? verif : false;
    verif = game->setGameId(gameStruct.dg_id) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }

    return game;
}

DuObjectPtr DuGameMetadata::clone() const
{
    return DuGameMetadataPtr(new DuGameMetadata(*this));
}

QByteArray DuGameMetadata::toDuMusicBinary() const
{
    s_dugame game;
    std::memset(&game, 0x00, DUGAME_HEADER);

    int grade = getGrade();
    if (grade == -1)
        return QByteArray();
    game.dg_grade = grade;

    game.dg_currentevent = 0;

    int unlockerEvent = getUnlockerEvent();
    if (unlockerEvent == -1)
        return QByteArray();
    game.dg_unlocker_event = unlockerEvent;

    int version = getVersion();
    if (version == -1)
        return QByteArray();
    game.dg_version = version;

    int gameId = getGameId();
    if (gameId == -1)
        return QByteArray();
    game.dg_id = gameId;


    const DuArrayConstPtr<DuSystemSoundIdentifier> &sounds = getSounds();
    if (sounds == Q_NULLPTR)
        return QByteArray();

    int soundsSize = sounds->size();
    if (soundsSize == -1)
        return QByteArray();

    const QByteArray &soundsArray = sounds->toDuMusicBinary();
    if (soundsArray.isNull())
        return QByteArray();

    std::memcpy(&(game.dg_sound), soundsArray.data(), static_cast<size_t>(soundsSize));


    const DuArrayConstPtr<DuGameEvent>& events = getEvents();
    if (events == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't get events";
        return QByteArray();
    }

    game.dg_numevent = events->count();


    QByteArray data(reinterpret_cast<const char*>(&game), DUGAME_HEADER);

    for (const DuGameEventConstPtr& event : *(events))
    {
        const QByteArray& eventData = event->toDuMusicBinary();
        if (eventData.isEmpty())
            return QByteArray();

        data += eventData;
    }

    return data;
}

int DuGameMetadata::size() const
{
    const DuArrayConstPtr<DuGameEvent>& events = getEvents();
    if (events == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't get events";
        return -1;
    }

    return DUGAME_HEADER + events->size();
}

DU_KEY_ACCESSORS_IMPL(DuGameMetadata, Grade, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuGameMetadata, UnlockerEvent, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuGameMetadata, Version, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuGameMetadata, GameId, Numeric, int, -1)
DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuGameMetadata, Sounds, DuArray, DuSystemSoundIdentifier)
DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuGameMetadata, Events, DuArray, DuGameEvent)
