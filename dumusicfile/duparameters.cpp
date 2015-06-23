#include "duparameters.h"

#include <QJsonObject>


DU_OBJECT_IMPL(DuParameters)

DuParameters::DuParameters() :
    DuContainer()
{
    addChild(KEY_PARAMS_DIRECTIONGYROP,
             new DuNumeric(DIRECTIONGYRO_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           DIRECTIONGYRO_MAXVALUE, DIRECTIONGYRO_MINVALUE));

    addChild(KEY_PARAMS_DIRECTIONGYROR,
             new DuNumeric(DIRECTIONGYRO_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           DIRECTIONGYRO_MAXVALUE, DIRECTIONGYRO_MINVALUE));

    addChild(KEY_PARAMS_DIRECTIONGYROY,
             new DuNumeric(DIRECTIONGYRO_MINVALUE, NUMERIC_DEFAULT_SIZE,
                           DIRECTIONGYRO_MAXVALUE, DIRECTIONGYRO_MINVALUE));

    addChild(KEY_PARAMS_ACTIVEAFTERTOUCH,
             new DuNumeric(0x00, PARAMS_NUMERIC_SIZE,
                           0x7F, 0x00));

    addChild(KEY_PARAMS_ACTIVESLIDERL,
             new DuNumeric(0x00, PARAMS_NUMERIC_SIZE,
                           0x7F, 0x00));

    addChild(KEY_PARAMS_ACTIVESLIDERR,
             new DuNumeric(0x00, PARAMS_NUMERIC_SIZE,
                           0x7F, 0x00));

    addChild(KEY_PARAMS_ACTIVEGYROP,
             new DuNumeric(0x00, PARAMS_NUMERIC_SIZE,
                           0x7F, 0x00));

    addChild(KEY_PARAMS_ACTIVEGYROR,
             new DuNumeric(0x00, PARAMS_NUMERIC_SIZE,
                           0x7F, 0x00));

    addChild(KEY_PARAMS_ACTIVEGYROY,
             new DuNumeric(0x00, PARAMS_NUMERIC_SIZE,
                           0x7F, 0x00));
}

DuParameters::~DuParameters()
{
}


DuObjectPtr DuParameters::clone() const
{
    return DuParametersPtr(new DuParameters(*this));
}


DuParametersPtr DuParameters::fromDuMusicBinary(const music_song &du_song)
{
    DuParametersPtr parameters(new DuParameters);
    bool verif = true;

    verif = verif && parameters->setDirectionGyroP(du_song.s_direction_gyro_P);
    verif = verif && parameters->setDirectionGyroR(du_song.s_direction_gyro_R);
    verif = verif && parameters->setDirectionGyroY(du_song.s_direction_gyro_Y);

    verif = verif && parameters->setActiveAftertouch(du_song.s_activ_aftertouch);
    verif = verif && parameters->setActiveSliderL(du_song.s_activ_slider_L);
    verif = verif && parameters->setActiveSliderR(du_song.s_activ_slider_R);

    verif = verif && parameters->setActiveGyroP(du_song.s_activ_gyro_P);
    verif = verif && parameters->setActiveGyroR(du_song.s_activ_gyro_R);
    verif = verif && parameters->setActiveGyroY(du_song.s_activ_gyro_Y);

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuParameters::fromDuMusicBinary():\n"
                   << "an attribute was not properly set";
    }

    return parameters;
}


DuParametersPtr DuParameters::fromJson(const QJsonObject &jsonParameters)
{
    QJsonValue jsonDirectionGyroP       = jsonParameters[KEY_PARAMS_DIRECTIONGYROP];
    QJsonValue jsonDirectionGyroR       = jsonParameters[KEY_PARAMS_DIRECTIONGYROR];
    QJsonValue jsonDirectionGyroY       = jsonParameters[KEY_PARAMS_DIRECTIONGYROY];
    QJsonValue jsonActiveAftertouch     = jsonParameters[KEY_PARAMS_ACTIVEAFTERTOUCH];
    QJsonValue jsonActiveSliderL        = jsonParameters[KEY_PARAMS_ACTIVESLIDERL];
    QJsonValue jsonActiveSliderR        = jsonParameters[KEY_PARAMS_ACTIVESLIDERR];
    QJsonValue jsonActiveGyroP          = jsonParameters[KEY_PARAMS_ACTIVEGYROP];
    QJsonValue jsonActiveGyroR          = jsonParameters[KEY_PARAMS_ACTIVEGYROR];
    QJsonValue jsonActiveGyroY          = jsonParameters[KEY_PARAMS_ACTIVEGYROY];

    if (        !jsonDirectionGyroP.isDouble()  ||  !jsonDirectionGyroR.isDouble()
            ||  !jsonDirectionGyroY.isDouble()  ||  !jsonActiveAftertouch.isDouble()
            ||  !jsonActiveSliderL.isDouble()   ||  !jsonActiveSliderR.isDouble()
            ||  !jsonActiveGyroP.isDouble()     ||  !jsonActiveGyroR.isDouble()
            ||  !jsonActiveGyroY.isDouble())
    {
        qCCritical(LOG_CAT_DU_OBJECT) << "DuParameters::fromJson():\n"
                    << "failed to generate DuParameters\n"
                    << "a json key did not contain the proper type";

        return DuParametersPtr();
    }


    DuParametersPtr parameters(new DuParameters);
    bool verif = true;

    verif = verif && parameters->setDirectionGyroP(jsonDirectionGyroP.toInt());
    verif = verif && parameters->setDirectionGyroR(jsonDirectionGyroR.toInt());
    verif = verif && parameters->setDirectionGyroY(jsonDirectionGyroY.toInt());

    verif = verif && parameters->setActiveAftertouch(jsonActiveAftertouch.toInt());
    verif = verif && parameters->setActiveSliderL(jsonActiveSliderL.toInt());
    verif = verif && parameters->setActiveSliderR(jsonActiveSliderR.toInt());

    verif = verif && parameters->setActiveGyroP(jsonActiveGyroP.toInt());
    verif = verif && parameters->setActiveGyroR(jsonActiveGyroR.toInt());
    verif = verif && parameters->setActiveGyroY(jsonActiveGyroY.toInt());

    if (!verif)
    {
        qCWarning(LOG_CAT_DU_OBJECT) << "DuParameters::fromJson():\n"
                   << "an attribute was not properly set";
    }

    return parameters;
}


QByteArray DuParameters::toDuMusicBinary() const
{
    //TODO: restructure music_song to match du-objects
    music_song du_parameters;

    int tmpNum = 0;

    QByteArray tmpClear(MUSIC_SONG_SIZE, (char)0x00);
    std::memcpy((char *)&(du_parameters), tmpClear.data(), MUSIC_SONG_SIZE);


    tmpNum = getDirectionGyroP();
    if (tmpNum == 0)
        return QByteArray();
    du_parameters.s_direction_gyro_P = tmpNum;

    tmpNum = getDirectionGyroR();
    if (tmpNum == 0)
        return QByteArray();
    du_parameters.s_direction_gyro_R = tmpNum;

    tmpNum = getDirectionGyroY();
    if (tmpNum == 0)
        return QByteArray();
    du_parameters.s_direction_gyro_Y = tmpNum;


    tmpNum = getActiveAftertouch();
    if (tmpNum == -1)
        return QByteArray();
    du_parameters.s_activ_aftertouch = tmpNum;

    tmpNum = getActiveSliderL();
    if (tmpNum == -1)
        return QByteArray();
    du_parameters.s_activ_slider_L = tmpNum;

    tmpNum = getActiveSliderR();
    if (tmpNum == -1)
        return QByteArray();
    du_parameters.s_activ_slider_R = tmpNum;


    tmpNum = getActiveGyroP();
    if (tmpNum == -1)
        return QByteArray();
    du_parameters.s_activ_gyro_P = tmpNum;

    tmpNum = getActiveGyroR();
    if (tmpNum == -1)
        return QByteArray();
    du_parameters.s_activ_gyro_R = tmpNum;

    tmpNum = getActiveGyroY();
    if (tmpNum == -1)
        return QByteArray();
    du_parameters.s_activ_gyro_Y = tmpNum;


    return QByteArray((char *)&(du_parameters) + PARAMETERS_OFFSET, size());
}


int DuParameters::size() const
{
    return PARAMETERS_SIZE;
}


//Warning: the direction gyro parameters should be either 1 or -1 so the error
//return value can't be -1.

int DuParameters::getDirectionGyroP() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_DIRECTIONGYROP);

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

bool DuParameters::setDirectionGyroP(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_DIRECTIONGYROP);

    if (tmp == NULL)
        return false;

    if (value == 0)
    {
        tmp->setNumeric(DIRECTIONGYRO_MINVALUE);
        return false;
    }

    return tmp->setNumeric(value);
}


int DuParameters::getDirectionGyroR() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_DIRECTIONGYROR);

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

bool DuParameters::setDirectionGyroR(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_DIRECTIONGYROR);

    if (tmp == NULL)
        return false;

    if (value == 0)
    {
        tmp->setNumeric(DIRECTIONGYRO_MINVALUE);
        return false;
    }

    return tmp->setNumeric(value);
}


int DuParameters::getDirectionGyroY() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_DIRECTIONGYROY);

    if (tmp == NULL)
        return 0;

    return tmp->getNumeric();
}

bool DuParameters::setDirectionGyroY(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_DIRECTIONGYROY);

    if (tmp == NULL)
        return false;

    if (value == 0)
    {
        tmp->setNumeric(DIRECTIONGYRO_MINVALUE);
        return false;
    }

    return tmp->setNumeric(value);
}


int DuParameters::getActiveAftertouch() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_ACTIVEAFTERTOUCH);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuParameters::setActiveAftertouch(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_ACTIVEAFTERTOUCH);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuParameters::getActiveSliderL() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_ACTIVESLIDERL);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuParameters::setActiveSliderL(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_ACTIVESLIDERL);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuParameters::getActiveSliderR() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_ACTIVESLIDERR);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuParameters::setActiveSliderR(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_ACTIVESLIDERR);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuParameters::getActiveGyroP() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_ACTIVEGYROP);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuParameters::setActiveGyroP(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_ACTIVEGYROP);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuParameters::getActiveGyroR() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_ACTIVEGYROR);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuParameters::setActiveGyroR(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_ACTIVEGYROR);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}


int DuParameters::getActiveGyroY() const
{
    const DuNumericConstPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_ACTIVEGYROY);

    if (tmp == NULL)
        return -1;

    return tmp->getNumeric();
}

bool DuParameters::setActiveGyroY(int value)
{
    const DuNumericPtr &tmp = getChildAs<DuNumeric>(KEY_PARAMS_ACTIVEGYROY);

    if (tmp == NULL)
        return false;

    return tmp->setNumeric(value);
}
