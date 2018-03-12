#include "DuWorldUserData.h"

#include "../general/duarray.h"

#include <QJsonDocument>
#include <QJsonObject>

DU_OBJECT_IMPL(DuWorldUserData)

DuWorldUserData::DuWorldUserData() : DuContainer()
{
    addChild(KeyFinishedGames, new DuNumericArray);
    addChild(KeyUnlockedGames, new DuNumericArray);
}

void DuWorldUserData::mergeWith(const DuWorldUserDataConstPtr &other, bool &outNewInThis, bool &outNewInOther)
{
    if (other == Q_NULLPTR)
        return;

    const QSet<int>& thisFinishedGamesSet = QSet<int>::fromList(getFinishedGames());
    const QSet<int>& otherFinishedGamesSet = QSet<int>::fromList(other->getFinishedGames());

    const QSet<int>& thisUnlockedGamesSet = QSet<int>::fromList(getUnlockedGames());
    const QSet<int>& otherUnlockedGamesSet = QSet<int>::fromList(other->getUnlockedGames());


    const QSet<int>& finishedGamesThisDiff = thisFinishedGamesSet - otherFinishedGamesSet;
    const QSet<int>& unlockedGamesThisDiff = thisUnlockedGamesSet - otherUnlockedGamesSet;
    outNewInThis = !finishedGamesThisDiff.isEmpty() || !unlockedGamesThisDiff.isEmpty();

    const QSet<int>& finishedGamesOtherDiff = otherFinishedGamesSet - thisFinishedGamesSet;
    const QSet<int>& unlockedGamesOtherDiff = otherUnlockedGamesSet - thisUnlockedGamesSet;
    outNewInOther = !finishedGamesOtherDiff.isEmpty() || !unlockedGamesOtherDiff.isEmpty();


    setFinishedGames(QSet<int>(thisFinishedGamesSet + otherFinishedGamesSet).toList());
    setUnlockedGames(QSet<int>(thisUnlockedGamesSet + otherUnlockedGamesSet).toList());
}

DuWorldUserDataPtr DuWorldUserData::fromJsonApi(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        return {};

    DuWorldUserDataPtr outData(new DuWorldUserData);

    const QJsonObject& wordDataObject = jsonValue.toObject();

    // FINISHED GAMES
    if (wordDataObject.contains("finished_games"))
    {
        const QJsonValue& finishedGamesValue = wordDataObject.value("finished_games");
        if (!finishedGamesValue.isArray())
            return {};

        const QJsonArray& finishedGamesArray = finishedGamesValue.toArray();

        QList<int> finishedGames;
        for (const QJsonValue &item : finishedGamesArray)
        {
            finishedGames << item.toInt();
        }

        outData->setFinishedGames(finishedGames);
    }

    // UNLOCKED GAMES
    if (wordDataObject.contains("unlocked_games"))
    {
        const QJsonValue& unlockedGamesValue = wordDataObject.value("unlocked_games");
        if (!unlockedGamesValue.isArray())
            return {};

        const QJsonArray& unlockedGamesArray = unlockedGamesValue.toArray();

        QList<int> unlockedGames;
        for (const QJsonValue &item : unlockedGamesArray)
        {
            unlockedGames << item.toInt();
        }

        outData->setUnlockedGames(unlockedGames);
    }

    return outData;
}

DuObjectPtr DuWorldUserData::clone() const
{
    return DuWorldUserDataPtr(new DuWorldUserData(*this));
}

QHttpMultiPart *DuWorldUserData::toHttpMultiPart(const QByteArray &boundary) const
{
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setBoundary(boundary);

    QJsonArray finishedGamesJsonArray;
    const QList<int>& finishedGames = getFinishedGames();
    for (const int finishedGame : finishedGames)
    {
        finishedGamesJsonArray << finishedGame;
    }

    QJsonArray unlockedGamesJsonArray;
    const QList<int>& unlockedGames = getUnlockedGames();
    for (const int unlockedGame : unlockedGames)
    {
        unlockedGamesJsonArray << unlockedGame;
    }

    QHttpPart finishedGamesPart;
    finishedGamesPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"finished_games\"");
    finishedGamesPart.setBody(QJsonDocument(finishedGamesJsonArray).toJson());

    QHttpPart unlockedGamesPart;
    unlockedGamesPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"unlocked_games\"");
    unlockedGamesPart.setBody(QJsonDocument(unlockedGamesJsonArray).toJson());

    multiPart->append(finishedGamesPart);
    multiPart->append(unlockedGamesPart);

    return multiPart;
}

DU_KEY_ACCESSORS_IMPL(DuWorldUserData, FinishedGames, NumericArray, QList<int>, QList<int>())
DU_KEY_ACCESSORS_IMPL(DuWorldUserData, UnlockedGames, NumericArray, QList<int>, QList<int>())
