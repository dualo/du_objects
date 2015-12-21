#include "ducontrollers.h"

#include <cstring>

#include <QJsonObject>

#include "../general/dunumeric.h"


DU_OBJECT_IMPL(DuControllers)

DuControllers::DuControllers() :
    DuContainer()
{
    addChild(KeyDirectionGyroP,
             new DuNumeric(DIRECTIONGYRO_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           DIRECTIONGYRO_MAXVALUE, DIRECTIONGYRO_MINVALUE));

    addChild(KeyDirectionGyroR,
             new DuNumeric(DIRECTIONGYRO_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           DIRECTIONGYRO_MAXVALUE, DIRECTIONGYRO_MINVALUE));

    addChild(KeyDirectionGyroY,
             new DuNumeric(DIRECTIONGYRO_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           DIRECTIONGYRO_MAXVALUE, DIRECTIONGYRO_MINVALUE));

    addChild(KeyActiveAftertouch,
             new DuNumeric(0x00, CONTROLS_NUMERIC_SIZE,
                           0xFFFF, 0x0000));

    addChild(KeyActiveSliderL,
             new DuNumeric(0x00, CONTROLS_NUMERIC_SIZE,
                           0xFFFF, 0x0000));

    addChild(KeyActiveSliderR,
             new DuNumeric(0x00, CONTROLS_NUMERIC_SIZE,
                           0xFFFF, 0x0000));

    addChild(KeyActiveGyroP,
             new DuNumeric(0x00, CONTROLS_NUMERIC_SIZE,
                           0xFFFF, 0x0000));

    addChild(KeyActiveGyroR,
             new DuNumeric(0x00, CONTROLS_NUMERIC_SIZE,
                           0xFFFF, 0x0000));

    addChild(KeyActiveGyroY,
             new DuNumeric(0x00, CONTROLS_NUMERIC_SIZE,
                           0xFFFF, 0x0000));
}

DuControllers::~DuControllers()
{
}


DuObjectPtr DuControllers::clone() const
{
    return DuControllersPtr(new DuControllers(*this));
}


DuControllersPtr DuControllers::fromDuMusicBinary(const music_song &du_song)
{
    DuControllersPtr controllers(new DuControllers);
    bool verif = true;

    verif = controllers->setDirectionGyroP(du_song.s_direction_gyro_P) ? verif : false;
    verif = controllers->setDirectionGyroR(du_song.s_direction_gyro_R) ? verif : false;
    verif = controllers->setDirectionGyroY(du_song.s_direction_gyro_Y) ? verif : false;

    verif = controllers->setActiveAftertouch(du_song.s_activ_aftertouch) ? verif : false;
    verif = controllers->setActiveSliderL(du_song.s_activ_slider_L) ? verif : false;
    verif = controllers->setActiveSliderR(du_song.s_activ_slider_R) ? verif : false;

    verif = controllers->setActiveGyroP(du_song.s_activ_gyro_P) ? verif : false;
    verif = controllers->setActiveGyroR(du_song.s_activ_gyro_R) ? verif : false;
    verif = controllers->setActiveGyroY(du_song.s_activ_gyro_Y) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuControllers::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return controllers;
}

DuControllersPtr DuControllers::fromDuMusicBinary(const preset_instr &du_preset)
{
    DuControllersPtr controllers(new DuControllers);
    bool verif = true;

    verif = controllers->setDirectionGyroP(du_preset.s_direction_gyro_P) ? verif : false;
    verif = controllers->setDirectionGyroR(du_preset.s_direction_gyro_R) ? verif : false;
    verif = controllers->setDirectionGyroY(du_preset.s_direction_gyro_Y) ? verif : false;

    verif = controllers->setActiveAftertouch(du_preset.s_activ_aftertouch) ? verif : false;
    verif = controllers->setActiveSliderL(du_preset.s_activ_slider_L) ? verif : false;
    verif = controllers->setActiveSliderR(du_preset.s_activ_slider_R) ? verif : false;

    verif = controllers->setActiveGyroP(du_preset.s_activ_gyro_P) ? verif : false;
    verif = controllers->setActiveGyroR(du_preset.s_activ_gyro_R) ? verif : false;
    verif = controllers->setActiveGyroY(du_preset.s_activ_gyro_Y) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuControllers::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return controllers;
}


DuControllersPtr DuControllers::fromJson(const QJsonObject &jsonControllers)
{
    QJsonValue jsonDirectionGyroP       = jsonControllers[KeyDirectionGyroP];
    QJsonValue jsonDirectionGyroR       = jsonControllers[KeyDirectionGyroR];
    QJsonValue jsonDirectionGyroY       = jsonControllers[KeyDirectionGyroY];
    QJsonValue jsonActiveAftertouch     = jsonControllers[KeyActiveAftertouch];
    QJsonValue jsonActiveSliderL        = jsonControllers[KeyActiveSliderL];
    QJsonValue jsonActiveSliderR        = jsonControllers[KeyActiveSliderR];
    QJsonValue jsonActiveGyroP          = jsonControllers[KeyActiveGyroP];
    QJsonValue jsonActiveGyroR          = jsonControllers[KeyActiveGyroR];
    QJsonValue jsonActiveGyroY          = jsonControllers[KeyActiveGyroY];

    if (        !jsonDirectionGyroP.isDouble()  ||  !jsonDirectionGyroR.isDouble()
            ||  !jsonDirectionGyroY.isDouble()  ||  !jsonActiveAftertouch.isDouble()
            ||  !jsonActiveSliderL.isDouble()   ||  !jsonActiveSliderR.isDouble()
            ||  !jsonActiveGyroP.isDouble()     ||  !jsonActiveGyroR.isDouble()
            ||  !jsonActiveGyroY.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuControllers::fromJson():\n"
                    << "failed to generate DuControllers\n"
                    << "a json key did not contain the proper type";

        return DuControllersPtr();
    }


    DuControllersPtr controllers(new DuControllers);
    bool verif = true;

    verif = controllers->setDirectionGyroP(jsonDirectionGyroP.toInt()) ? verif : false;
    verif = controllers->setDirectionGyroR(jsonDirectionGyroR.toInt()) ? verif : false;
    verif = controllers->setDirectionGyroY(jsonDirectionGyroY.toInt()) ? verif : false;

    verif = controllers->setActiveAftertouch(jsonActiveAftertouch.toInt()) ? verif : false;
    verif = controllers->setActiveSliderL(jsonActiveSliderL.toInt()) ? verif : false;
    verif = controllers->setActiveSliderR(jsonActiveSliderR.toInt()) ? verif : false;

    verif = controllers->setActiveGyroP(jsonActiveGyroP.toInt()) ? verif : false;
    verif = controllers->setActiveGyroR(jsonActiveGyroR.toInt()) ? verif : false;
    verif = controllers->setActiveGyroY(jsonActiveGyroY.toInt()) ? verif : false;

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuControllers::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return controllers;
}


QByteArray DuControllers::toDuMusicBinary() const
{
    //NOTE: optimization possible if music_song matched du-objects
    music_song du_controllers;

    int tmpNum = 0;

    QByteArray tmpClear(MUSIC_SONG_SIZE, (char)0x00);
    std::memcpy((char *)&(du_controllers), tmpClear.data(), MUSIC_SONG_SIZE);


    tmpNum = getDirectionGyroP();
    if (tmpNum == 0)
        return QByteArray();
    du_controllers.s_direction_gyro_P = tmpNum;

    tmpNum = getDirectionGyroR();
    if (tmpNum == 0)
        return QByteArray();
    du_controllers.s_direction_gyro_R = tmpNum;

    tmpNum = getDirectionGyroY();
    if (tmpNum == 0)
        return QByteArray();
    du_controllers.s_direction_gyro_Y = tmpNum;


    tmpNum = getActiveAftertouch();
    if (tmpNum == -1)
        return QByteArray();
    du_controllers.s_activ_aftertouch = tmpNum;

    tmpNum = getActiveSliderL();
    if (tmpNum == -1)
        return QByteArray();
    du_controllers.s_activ_slider_L = tmpNum;

    tmpNum = getActiveSliderR();
    if (tmpNum == -1)
        return QByteArray();
    du_controllers.s_activ_slider_R = tmpNum;


    tmpNum = getActiveGyroP();
    if (tmpNum == -1)
        return QByteArray();
    du_controllers.s_activ_gyro_P = tmpNum;

    tmpNum = getActiveGyroR();
    if (tmpNum == -1)
        return QByteArray();
    du_controllers.s_activ_gyro_R = tmpNum;

    tmpNum = getActiveGyroY();
    if (tmpNum == -1)
        return QByteArray();
    du_controllers.s_activ_gyro_Y = tmpNum;


    return QByteArray((char *)&(du_controllers) + CONTROLLERS_SONG_OFFSET, size());
}


int DuControllers::size() const
{
    return CONTROLLERS_SIZE;
}


//Warning: the direction gyro parameters should be either 1 or -1 so the error
//return value can't be -1.

int DuControllers::getDirectionGyroP() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KeyDirectionGyroP);

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

bool DuControllers::setDirectionGyroP(const int& value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KeyDirectionGyroP);

    if (tmp == NULL)
        return false;

    if (value == 0)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuControllers::setDirectionGyroP():\n"
                   << "incorrect value found, default value used instead";

        tmp->setNumeric(DIRECTIONGYRO_MINVALUE);
        return false;
    }

    return tmp->setNumeric(value);
}

const QString DuControllers::KeyDirectionGyroP = QStringLiteral("DirectionGyroP");


int DuControllers::getDirectionGyroR() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KeyDirectionGyroR);

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

bool DuControllers::setDirectionGyroR(const int& value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KeyDirectionGyroR);

    if (tmp == NULL)
        return false;

    if (value == 0)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuControllers::setDirectionGyroP():\n"
                   << "incorrect value found, default value used instead";

        tmp->setNumeric(DIRECTIONGYRO_MINVALUE);
        return false;
    }

    return tmp->setNumeric(value);
}

const QString DuControllers::KeyDirectionGyroR = QStringLiteral("DirectionGyroR");


int DuControllers::getDirectionGyroY() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KeyDirectionGyroY);

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

bool DuControllers::setDirectionGyroY(const int& value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KeyDirectionGyroY);

    if (tmp == NULL)
        return false;

    if (value == 0)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuControllers::setDirectionGyroP():\n"
                   << "incorrect value found, default value used instead";

        tmp->setNumeric(DIRECTIONGYRO_MINVALUE);
        return false;
    }

    return tmp->setNumeric(value);
}

const QString DuControllers::KeyDirectionGyroY = QStringLiteral("DirectionGyroY");


DU_KEY_ACCESSORS_IMPL(DuControllers, ActiveAftertouch, Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuControllers, ActiveSliderL,    Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuControllers, ActiveSliderR,    Numeric, int, -1)

DU_KEY_ACCESSORS_IMPL(DuControllers, ActiveGyroP,      Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuControllers, ActiveGyroR,      Numeric, int, -1)
DU_KEY_ACCESSORS_IMPL(DuControllers, ActiveGyroY,      Numeric, int, -1)
