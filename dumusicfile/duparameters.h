#ifndef DUPARAMETERS_H
#define DUPARAMETERS_H

#include "../general/ducontainer.h"
#include "../general/dunumeric.h"


#define KEY_PARAMS_DIRECTIONGYROP       "DirectionGyroP"
#define KEY_PARAMS_DIRECTIONGYROR       "DirectionGyroR"
#define KEY_PARAMS_DIRECTIONGYROY       "DirectionGyroY"

#define KEY_PARAMS_ACTIVEAFTERTOUCH     "ActiveAftertouch"
#define KEY_PARAMS_ACTIVESLIDERL        "ActiveSliderLeft"
#define KEY_PARAMS_ACTIVESLIDERR        "ActiveSliderRight"

#define KEY_PARAMS_ACTIVEGYROP          "ActiveGyroP"
#define KEY_PARAMS_ACTIVEGYROR          "ActiveGyroR"
#define KEY_PARAMS_ACTIVEGYROY          "ActiveGyroY"


#define PARAMETERS_SIZE                 15
#define PARAMETERS_OFFSET               4 + 8 * 16 + 10 + 3


#define DIRECTIONGYRO_MINVALUE          -1
#define DIRECTIONGYRO_MAXVALUE          1

#define PARAMS_NUMERIC_SIZE             2


DU_OBJECT(DuParameters)

class DuParameters : public DuContainer
{
public:
    explicit DuParameters();
    ~DuParameters();

    virtual DuObjectPtr clone() const;

    static DuParametersPtr fromDuMusicBinary(const music_song &du_song);
    static DuParametersPtr fromJson(const QJsonObject &jsonParameters);

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

#endif // DUPARAMETERS_H
