#include "DuWorldUserData.h"

#include "../general/duarray.h"
#include "../general/DuTypedValue.h"

#include <QJsonDocument>
#include <QJsonObject>

DU_OBJECT_IMPL(DuWorldUserData)

DuWorldUserData::DuWorldUserData() : DuContainer()
{
    addChild(KeyFinishedGames, new DuTypedValue<DuWorldUserData::FinishedGamesMap>);
    addChild(KeyUnlockedGames, new DuNumericArray);
}

void DuWorldUserData::mergeWith(const DuWorldUserDataConstPtr &other, bool &outNewInThis, bool &outNewInOther)
{
    if (other == Q_NULLPTR)
        return;

    QMap<int, FinishedGameData> mergedFinishedGames;
    const QMap<int, FinishedGameData>& thisFinishedGames = getFinishedGames();
    const QMap<int, FinishedGameData>& otherFinishedGames = other->getFinishedGames();

    QMap<int, FinishedGameData> finishedGamesOtherDiff = otherFinishedGames;
    QMapIterator<int, FinishedGameData> it(thisFinishedGames);
    while (it.hasNext())
    {
        it.next();
        const int id = it.key();
        const FinishedGameData& thisData = it.value();

        if (otherFinishedGames.contains(id))
        {
            const FinishedGameData& otherData = otherFinishedGames.value(id);

            if (otherData != thisData)
            {
                const FinishedGameData& mergedData = FinishedGameData::getBestOfTwo(thisData, otherData);

                mergedFinishedGames.insert(id, mergedData);

                outNewInThis = outNewInThis || thisData.hasSomethingBetterThan(otherData);
                outNewInOther = outNewInOther || otherData.hasSomethingBetterThan(thisData);
            }
            else
            {
                mergedFinishedGames.insert(id, thisData);
            }
        }
        else
        {
            mergedFinishedGames.insert(id, thisData);
            outNewInThis = true;
        }

        finishedGamesOtherDiff.remove(id);
    }

    outNewInOther = outNewInOther || !finishedGamesOtherDiff.isEmpty();

    mergedFinishedGames.unite(finishedGamesOtherDiff);


    const QSet<int>& thisUnlockedGamesSet = QSet<int>::fromList(getUnlockedGames());
    const QSet<int>& otherUnlockedGamesSet = QSet<int>::fromList(other->getUnlockedGames());

    const QSet<int>& unlockedGamesThisDiff = thisUnlockedGamesSet - otherUnlockedGamesSet;
    outNewInThis = outNewInThis || !unlockedGamesThisDiff.isEmpty();

    const QSet<int>& unlockedGamesOtherDiff = otherUnlockedGamesSet - thisUnlockedGamesSet;
    outNewInOther = outNewInOther || !unlockedGamesOtherDiff.isEmpty();


    setFinishedGames(mergedFinishedGames);
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

        QMap<int, FinishedGameData> finishedGames;
        for (const QJsonValue &item : finishedGamesArray)
        {
            if (item.isDouble())
            {
                finishedGames.insert(item.toInt(), FinishedGameData{1, QTime()});
            }
            else if (item.isObject())
            {
                const QJsonObject& obj = item.toObject();
                finishedGames.insert(obj.value("id").toInt(), FinishedGameData{obj.value("nbStars").toInt(), QTime::fromString(obj.value("bestTime").toString())});
            }
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
    const QMap<int, FinishedGameData>& finishedGames = getFinishedGames();
    QMapIterator<int, FinishedGameData> it(finishedGames);
    while (it.hasNext())
    {
        it.next();
        QJsonObject obj;
        obj.insert("id", it.key());
        obj.insert("nbStars", it.value().nbStars);
        obj.insert("bestTime", it.value().bestTime.toString());
        finishedGamesJsonArray << obj;
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

DU_KEY_ACCESSORS_TEMPLATE_IMPL(DuWorldUserData, FinishedGames, TypedValue, FinishedGamesMap, DuWorldUserData::FinishedGamesMap, FinishedGamesMap())
DU_KEY_ACCESSORS_IMPL(DuWorldUserData, UnlockedGames, NumericArray, QList<int>, QList<int>())
