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

    addChild(KeyFirstStarEvent, new DuNumeric(UINT16_MAX, 2, UINT16_MAX, 0));
    addChild(KeySecondStarEvent, new DuNumeric(UINT16_MAX, 2, UINT16_MAX, 0));
    addChild(KeyThirdStarEvent, new DuNumeric(UINT16_MAX, 2, UINT16_MAX, 0));

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
    // migrate
    QByteArray migratedData(data);
    if (version <= 2)
    {
        typedef struct
        {
            uint32_t dg_grade;
            uint32_t dg_numevent;

            uint32_t dg_currentevent;
            uint32_t dg_unlocker_event;

            uint32_t dg_version;
            uint32_t dg_id;
            uint32_t dummy[3];
            uint32_t dg_sound[5][2]; // s_dugame_sound dg_sound[MAX_DUGAME_SOUND];
        } s_dugame_v2;

        s_dugame_v2 gameStructV2;
        {
            QDataStream stream(migratedData);
            stream.setByteOrder(QDataStream::LittleEndian);
            stream.readRawData(reinterpret_cast<char*>(&gameStructV2), sizeof(s_dugame_v2));
        }

        s_dugame gameStruct;
        std::memcpy(&gameStruct, &gameStructV2, sizeof(s_dugame_v2));
        gameStruct.dg_currentevent = static_cast<uint16_t>(gameStructV2.dg_currentevent);
        gameStruct.dg_first_star_event = static_cast<uint16_t>(gameStructV2.dg_unlocker_event);
        gameStruct.dg_second_star_event = static_cast<uint16_t>(gameStructV2.dg_unlocker_event);
        gameStruct.dg_third_star_event = static_cast<uint16_t>(gameStructV2.dg_unlocker_event);

        migratedData = QByteArray(reinterpret_cast<const char*>(&gameStruct), DUGAME_HEADER);
    }

    QDataStream stream(migratedData);
    stream.setByteOrder(QDataStream::LittleEndian);

    s_dugame gameStruct;
    stream.readRawData(reinterpret_cast<char*>(&gameStruct), DUGAME_HEADER);

    const int expectedSize = DUGAME_HEADER + (gameStruct.dg_numevent * ARRANGEMENT_EVENT_SIZE);
    if (migratedData.size() != expectedSize)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGameMetadata: data size incorrect -> expected" << expectedSize << ", got" << migratedData.size();
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

    const uint nbEvents = gameStruct.dg_numevent;

    DuArrayPtr<DuGameEvent> eventsArray(new DuArray<DuGameEvent>);
    for (uint i = 0; i < nbEvents; ++i)
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
            event->setNextEvent(i == nbEvents - 1 ? UINT16_MAX : i + 1);
            event->setBackwardEvent(i == 0 ? 0 : i - 1);
            event->setForwardEvent(i == nbEvents - 1 ? UINT16_MAX : i + 1);
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
    verif = game->setFirstStarEvent(gameStruct.dg_first_star_event) ? verif : false;
    verif = game->setSecondStarEvent(gameStruct.dg_second_star_event) ? verif : false;
    verif = game->setThirdStarEvent(gameStruct.dg_third_star_event) ? verif : false;
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

    const int firstStarEvent = getFirstStarEvent();
    if (firstStarEvent == -1)
        return QByteArray();
    game.dg_first_star_event = static_cast<quint16>(firstStarEvent);

    const int secondStarEvent = getSecondStarEvent();
    if (secondStarEvent == -1)
        return QByteArray();
    game.dg_second_star_event = static_cast<quint16>(secondStarEvent);

    const int thirdStarEvent = getThirdStarEvent();
    if (thirdStarEvent == -1)
        return QByteArray();
    game.dg_third_star_event = static_cast<quint16>(thirdStarEvent);

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

DU_KEY_ACCESSORS_IMPL(DuGameMetadata, FirstStarEvent, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuGameMetadata, SecondStarEvent, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuGameMetadata, ThirdStarEvent, Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuGameMetadata, Version, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuGameMetadata, GameId, Numeric, int, -1)
DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuGameMetadata, Sounds, DuArray, DuSystemSoundIdentifier)
DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuGameMetadata, Events, DuArray, DuGameEvent)
