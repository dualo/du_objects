#ifndef FINISHEDGAMEDATA_H
#define FINISHEDGAMEDATA_H

#include <QMetaType>
#include <QTime>

struct FinishedGameData
{
    int nbStars;
    QTime bestTime;

    bool hasSomethingBetterThan(const FinishedGameData& other) const
    {
        return nbStars > other.nbStars || (bestTime.isValid() && !other.bestTime.isValid()) || bestTime < other.bestTime;
    }

    static FinishedGameData getBestOfTwo(const FinishedGameData& lhs, const FinishedGameData& rhs)
    {
        FinishedGameData result;
        result.nbStars = qMax(lhs.nbStars, rhs.nbStars);
        if (!lhs.bestTime.isValid())
            result.bestTime = rhs.bestTime;
        else if (!rhs.bestTime.isValid())
            result.bestTime = lhs.bestTime;
        else
            result.bestTime = qMin(lhs.bestTime, rhs.bestTime);

        return result;
    }
};

inline bool operator==(const FinishedGameData& lhs, const FinishedGameData& rhs)
{
    return lhs.nbStars == rhs.nbStars && lhs.bestTime == rhs.bestTime;
}

inline bool operator!=(const FinishedGameData& lhs, const FinishedGameData& rhs)
{
    return !(lhs == rhs);
}

QDebug operator<<(QDebug dbg, const FinishedGameData& obj);

Q_DECLARE_METATYPE(FinishedGameData)

#endif // FINISHEDGAMEDATA_H
