#include "DuGame.h"

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

DU_OBJECT_IMPL(DuGame)

DuGame::DuGame() : DuContainer()
{
    addChild(KeyGrade, new DuNumeric);

    DuArrayPtr<DuSystemSoundIdentifier> soundsArray(new DuArray<DuSystemSoundIdentifier>(MAX_DUGAME_SOUND));
    for (int i = 0; i < MAX_DUGAME_SOUND; ++i)
        soundsArray->append(new DuSystemSoundIdentifier);
    addChild(KeySounds, soundsArray);

    addChild(KeyEvents, new DuArray<DuGameEvent>);
}

DuGamePtr DuGame::fromBinary(const QByteArray &data, quint32 version)
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
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGame: data size incorrect -> expected" << expectedSize << ", got" << data.size();
        return {};
    }

    DuGamePtr game(new DuGame);

    DuArrayPtr<DuSystemSoundIdentifier> soundsArray(new DuArray<DuSystemSoundIdentifier>(MAX_DUGAME_SOUND));
    for (uint i = 0; i < MAX_DUGAME_SOUND; ++i)
    {
        const s_dugame_sound& soundStruct = gameStruct.dg_sound[i];

        const DuSystemSoundIdentifierPtr& sound = DuSystemSoundIdentifier::fromBinary(soundStruct);
        if (sound == Q_NULLPTR)
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGame: sound" << i << "corrupted";
            return {};
        }

        if (!soundsArray->append(sound))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGame: the sound" << i << "could not be appended";
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
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGame: event" << i << "corrupted";
            return {};
        }

        if (!eventsArray->append(event))
        {
            qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGame: the event" << i << "could not be appended";
            return {};
        }
    }
    game->setEvents(eventsArray);


    bool verif = true;

    verif = game->setGrade(gameStruct.dg_grade) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }

    return game;
}

DuObjectPtr DuGame::clone() const
{
    return DuGamePtr(new DuGame(*this));
}

QByteArray DuGame::toDuMusicBinary() const
{
    s_dugame game;
    std::memset(&game, 0x00, DUGAME_HEADER);

    int grade = getGrade();
    if (grade == -1)
        return QByteArray();
    game.dg_grade = grade;

    game.dg_currentevent = 0;


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

int DuGame::size() const
{
    const DuArrayConstPtr<DuGameEvent>& events = getEvents();
    if (events == Q_NULLPTR)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't get events";
        return -1;
    }

    return DUGAME_HEADER + events->size();
}

DU_KEY_ACCESSORS_IMPL(DuGame, Grade, Numeric, int, -1)
DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuGame, Sounds, DuArray, DuSystemSoundIdentifier)
DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuGame, Events, DuArray, DuGameEvent)
