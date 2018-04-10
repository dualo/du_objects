#include "FinishedGameData.h"

#include <QDebug>

QDebug operator<<(QDebug dbg, const FinishedGameData &obj)
{
     dbg.nospace() << "FinishedGameData(nbStars=" << obj.nbStars << ", bestTime=" << obj.bestTime << ")";

     return dbg.space();
}
