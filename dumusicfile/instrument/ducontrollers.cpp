#include "ducontrollers.h"

#include <cstring>

#include <QJsonObject>

#include "../../general/dunumeric.h"


DU_OBJECT_IMPL(DuControllers)

DuControllers::DuControllers() :
    DuContainer()
{
    addChild(KEY_CONTROLS_DIRECTIONGYROP,
             new DuNumeric(DIRECTIONGYRO_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           DIRECTIONGYRO_MAXVALUE, DIRECTIONGYRO_MINVALUE));

    addChild(KEY_CONTROLS_DIRECTIONGYROR,
             new DuNumeric(DIRECTIONGYRO_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           DIRECTIONGYRO_MAXVALUE, DIRECTIONGYRO_MINVALUE));

    addChild(KEY_CONTROLS_DIRECTIONGYROY,
             new DuNumeric(DIRECTIONGYRO_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           DIRECTIONGYRO_MAXVALUE, DIRECTIONGYRO_MINVALUE));

    addChild(KEY_CONTROLS_ACTIVEAFTERTOUCH,
             new DuNumeric(0x00, CONTROLS_NUMERIC_SIZE,
                           0xFFFF, 0x0000));

    addChild(KEY_CONTROLS_ACTIVESLIDERL,
             new DuNumeric(0x00, CONTROLS_NUMERIC_SIZE,
                           0xFFFF, 0x0000));

    addChild(KEY_CONTROLS_ACTIVESLIDERR,
             new DuNumeric(0x00, CONTROLS_NUMERIC_SIZE,
                           0xFFFF, 0x0000));

    addChild(KEY_CONTROLS_ACTIVEGYROP,
             new DuNumeric(0x00, CONTROLS_NUMERIC_SIZE,
                           0xFFFF, 0x0000));

    addChild(KEY_CONTROLS_ACTIVEGYROR,
             new DuNumeric(0x00, CONTROLS_NUMERIC_SIZE,
                           0xFFFF, 0x0000));

    addChild(KEY_CONTROLS_ACTIVEGYROY,
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
    QJsonValue jsonDirectionGyroP       = jsonControllers[KEY_CONTROLS_DIRECTIONGYROP];
    QJsonValue jsonDirectionGyroR       = jsonControllers[KEY_CONTROLS_DIRECTIONGYROR];
    QJsonValue jsonDirectionGyroY       = jsonControllers[KEY_CONTROLS_DIRECTIONGYROY];
    QJsonValue jsonActiveAftertouch     = jsonControllers[KEY_CONTROLS_ACTIVEAFTERTOUCH];
    QJsonValue jsonActiveSliderL        = jsonControllers[KEY_CONTROLS_ACTIVESLIDERL];
    QJsonValue jsonActiveSliderR        = jsonControllers[KEY_CONTROLS_ACTIVESLIDERR];
    QJsonValue jsonActiveGyroP          = jsonControllers[KEY_CONTROLS_ACTIVEGYROP];
    QJsonValue jsonActiveGyroR          = jsonControllers[KEY_CONTROLS_ACTIVEGYROR];
    QJsonValue jsonActiveGyroY          = jsonControllers[KEY_CONTROLS_ACTIVEGYROY];

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
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_DIRECTIONGYROP);

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

bool DuControllers::setDirectionGyroP(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_DIRECTIONGYROP);

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


int DuControllers::getDirectionGyroR() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_DIRECTIONGYROR);

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

bool DuControllers::setDirectionGyroR(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_DIRECTIONGYROR);

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


int DuControllers::getDirectionGyroY() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_DIRECTIONGYROY);

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

bool DuControllers::setDirectionGyroY(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_DIRECTIONGYROY);

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


int DuControllers::getActiveAftertouch() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_ACTIVEAFTERTOUCH);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuControllers::setActiveAftertouch(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_ACTIVEAFTERTOUCH);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuControllers::getActiveSliderL() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_ACTIVESLIDERL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuControllers::setActiveSliderL(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_ACTIVESLIDERL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuControllers::getActiveSliderR() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_ACTIVESLIDERR);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuControllers::setActiveSliderR(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_ACTIVESLIDERR);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuControllers::getActiveGyroP() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_ACTIVEGYROP);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuControllers::setActiveGyroP(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_ACTIVEGYROP);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuControllers::getActiveGyroR() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_ACTIVEGYROR);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuControllers::setActiveGyroR(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_ACTIVEGYROR);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuControllers::getActiveGyroY() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_ACTIVEGYROY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuControllers::setActiveGyroY(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_CONTROLS_ACTIVEGYROY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
