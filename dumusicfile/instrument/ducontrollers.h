#ifndef DUCONTROLLERS_H
#define DUCONTROLLERS_H

#include "../../general/ducontainer.h"


#define CONTROLLERS_SIZE                    15
#define CONTROLLERS_SONG_OFFSET             4 + 8 * 16 + 10 + 3


#define DIRECTIONGYRO_MINVALUE          -1
#define DIRECTIONGYRO_MAXVALUE          1

#define CONTROLS_NUMERIC_SIZE           2


DU_OBJECT(DuControllers);

class DuControllers : public DuContainer
{
public:
    explicit DuControllers();
    ~DuControllers();

    virtual DuObjectPtr clone() const;

    static DuControllersPtr fromDuMusicBinary(const music_song &du_song);
    static DuControllersPtr fromDuMusicBinary(const preset_instr &du_preset);
    static DuControllersPtr fromJson(const QJsonObject &jsonControllers);

    QByteArray toDuMusicBinary() const;

    int size() const;

    DU_KEY_ACCESSORS(DirectionGyroP,   int)
    DU_KEY_ACCESSORS(DirectionGyroR,   int)
    DU_KEY_ACCESSORS(DirectionGyroY,   int)

    DU_KEY_ACCESSORS(ActiveAftertouch, int)
    DU_KEY_ACCESSORS(ActiveSliderL,    int)
    DU_KEY_ACCESSORS(ActiveSliderR,    int)

    DU_KEY_ACCESSORS(ActiveGyroP,      int)
    DU_KEY_ACCESSORS(ActiveGyroR,      int)
    DU_KEY_ACCESSORS(ActiveGyroY,      int)
};

#endif // DUCONTROLLERS_H
