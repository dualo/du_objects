#include "DuGame.h"

#include "DuGameEvent.h"

#pragma pack(push, 4)
#include "../du-touch/dualo_structs/metadata_structs.h"
#pragma pack(pop)

#include "../general/duarray.h"
#include "../general/dunumeric.h"

#include <QDataStream>

DU_OBJECT_IMPL(DuGame)

DuGame::DuGame() : DuContainer()
{
    addChild(KeyGrade, new DuNumeric);
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

    s_arrangement arrangement;
    stream.readRawData(reinterpret_cast<char*>(&arrangement), ARRANGEMENT_HEADER);

    const int expectedSize = ARRANGEMENT_HEADER + (arrangement.as_numevent * ARRANGEMENT_EVENT_SIZE);
    if (data.size() != expectedSize)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't parse DuGame: data size incorrect -> expected" << expectedSize << ", got" << data.size();
        return {};
    }

    DuGamePtr game(new DuGame);

    DuArrayPtr<DuGameEvent> eventsArray(new DuArray<DuGameEvent>);
    for (uint i = 0; i < arrangement.as_numevent; ++i)
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

    verif = game->setGrade(arrangement.as_grade) ? verif : false;

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
    s_arrangement arrangement;

    int grade = getGrade();
    if (grade == -1)
        return QByteArray();
    arrangement.as_grade = grade;

    arrangement.as_currentevent = 0;

    const DuArrayConstPtr<DuGameEvent>& events = getEvents();
    if (events == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't get events";
        return QByteArray();
    }

    arrangement.as_numevent = events->count();

    QByteArray data(reinterpret_cast<const char*>(&arrangement), ARRANGEMENT_HEADER);

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
    if (events == NULL)
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "Can't get events";
        return -1;
    }

    return ARRANGEMENT_HEADER + events->size();
}

DU_KEY_ACCESSORS_IMPL(DuGame, Grade, Numeric, int, -1)
DU_KEY_ACCESSORS_OBJECT_TEMPLATE_IMPL(DuGame, Events, DuArray, DuGameEvent)
