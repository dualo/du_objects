#ifndef DUCONTROLLERS_H
#define DUCONTROLLERS_H

#include "../../general/ducontainer.h"
#include "../../general/dunumeric.h"


#define KEY_CONTROLS_DIRECTIONGYROP         "DirectionGyroP"
#define KEY_CONTROLS_DIRECTIONGYROR         "DirectionGyroR"
#define KEY_CONTROLS_DIRECTIONGYROY         "DirectionGyroY"

#define KEY_CONTROLS_ACTIVEAFTERTOUCH       "ActiveAftertouch"
#define KEY_CONTROLS_ACTIVESLIDERL          "ActiveSliderLeft"
#define KEY_CONTROLS_ACTIVESLIDERR          "ActiveSliderRight"

#define KEY_CONTROLS_ACTIVEGYROP            "ActiveGyroP"
#define KEY_CONTROLS_ACTIVEGYROR            "ActiveGyroR"
#define KEY_CONTROLS_ACTIVEGYROY            "ActiveGyroY"


#define CONTROLLERS_SIZE                    15
#define CONTROLLERS_SONG_OFFSET             4 + 8 * 16 + 10 + 3


#define DIRECTIONGYRO_MINVALUE          -1
#define DIRECTIONGYRO_MAXVALUE          1

#define CONTROLS_NUMERIC_SIZE           2


DU_OBJECT(DuControllers)

class DuControllers : public DuContainer
{
public:
    explicit DuControllers();
    ~DuControllers();

    virtual DuObjectPtr clone() const;

    static DuControllersPtr fromDuMusicBinary(const music_song &du_song);
    static DuControllersPtr fromJson(const QJsonObject &jsonControllers);

    QByteArray toDuMusicBinary() const;

    int size() const;

    int getDirectionGyroP() const;
    bool setDirectionGyroP(int value);

    int getDirectionGyroR() const;
    bool setDirectionGyroR(int value);

    int getDirectionGyroY() const;
    bool setDirectionGyroY(int value);

    int getActiveAftertouch() const;
    bool setActiveAftertouch(int value);

    int getActiveSliderL() const;
    bool setActiveSliderL(int value);

    int getActiveSliderR() const;
    bool setActiveSliderR(int value);

    int getActiveGyroP() const;
    bool setActiveGyroP(int value);

    int getActiveGyroR() const;
    bool setActiveGyroR(int value);

    int getActiveGyroY() const;
    bool setActiveGyroY(int value);
};

#endif // DUCONTROLLERS_H
