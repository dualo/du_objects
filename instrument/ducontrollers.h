#ifndef DUCONTROLLERS_H
#define DUCONTROLLERS_H

#include "../general/ducontainer.h"


#define CONTROLLERS_SIZE                    15
#define CONTROLLERS_SONG_OFFSET             4 + 8 * 16 + 10 + 3


#define DIRECTIONGYRO_MINVALUE          -1
#define DIRECTIONGYRO_MAXVALUE          1

#define CONTROLS_NUMERIC_SIZE           2

#define DuControllers_Children \
    X(DirectionGyroP,   Numeric, int, 0) \
    X(DirectionGyroR,   Numeric, int, 0) \
    X(DirectionGyroY,   Numeric, int, 0) \
     \
    X(ActiveAftertouch, Numeric, int, -1) \
    X(ActiveSliderL,    Numeric, int, -1) \
    X(ActiveSliderR,    Numeric, int, -1) \
     \
    X(ActiveGyroP,      Numeric, int, -1) \
    X(ActiveGyroR,      Numeric, int, -1) \
    X(ActiveGyroY,      Numeric, int, -1)


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

#define X(key, dutype, type, defaultReturn) DU_KEY_ACCESSORS(key, type)
    DuControllers_Children
#undef X
};

#endif // DUCONTROLLERS_H
