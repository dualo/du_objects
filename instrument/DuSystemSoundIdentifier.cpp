#include "DuSystemSoundIdentifier.h"

#include "../general/dunumeric.h"

DU_OBJECT_IMPL(DuSystemSoundIdentifier)

DuSystemSoundIdentifier::DuSystemSoundIdentifier() :
    DuContainer()
{
    addChild(KeyID, new DuNumeric);
    addChild(KeyUserID, new DuNumeric);
}

DuSystemSoundIdentifierPtr DuSystemSoundIdentifier::fromBinary(const s_dugame_sound &data)
{
    DuSystemSoundIdentifierPtr identifier(new DuSystemSoundIdentifier);

    bool verif = true;

    verif = identifier->setID(data.dg_sound_id) ? verif : false;
    verif = identifier->setUserID(data.dg_sound_user_id) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "An attribute was not properly set";
    }

    return identifier;
}

DuObjectPtr DuSystemSoundIdentifier::clone() const
{
    return DuSystemSoundIdentifierPtr(new DuSystemSoundIdentifier(*this));
}

QByteArray DuSystemSoundIdentifier::toDuMusicBinary() const
{
    s_dugame_sound identifier;

    int tmp = 0;

    tmp = getID();
    if (tmp == -1)
        return QByteArray();
    identifier.dg_sound_id = static_cast<quint32>(tmp);

    tmp = getUserID();
    if (tmp == -1)
        return QByteArray();
    identifier.dg_sound_user_id = static_cast<quint32>(tmp);

    return QByteArray(reinterpret_cast<const char*>(&identifier), DUGAME_SOUND_STRUCT_SIZE);
}

int DuSystemSoundIdentifier::size() const
{
    return DUGAME_SOUND_STRUCT_SIZE;
}

DU_KEY_ACCESSORS_IMPL(DuSystemSoundIdentifier, ID,     Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuSystemSoundIdentifier, UserID, Numeric, int, -1)
