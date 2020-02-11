#include "clickprefs.h"

ClickPrefs::ClickPrefs(QObject *parent)
    : QObject(parent)
    , _multiClickWeight(DEFAULT_MULTICLICK_WEIGHT)
    , _majDedLv1Weight(DEFAULT_MD_LV1_WEIGHT)
    , _majDedLv2Weight(DEFAULT_MD_LV2_WEIGHT)
    , _majDedLv3Weight(DEFAULT_MD_LV3_WEIGHT)
    , _totalTimeSetting(DEFAULT_FREESTYLE_LENGTH)
    , _timerDisplayRefresh(DEFAULT_DISPLAY_REFRESH)
    , _timerDigits(DEFAULT_TIMER_DIGITS)
{

}

/**
 * @brief Sets the weight, in points, of a major deduct level.
 * @param level Which MD level will be set.
 * @param weight The weight to be assigned to the specified MD level.
 * @return True if weight was set. False if you, oh, I dunno, tried to do something dumb.
 */
bool ClickPrefs::setMDWeight(int level, int weight) {
	switch(level) {
	case 0:
		_majDedLv1Weight = weight;
		break;
	case 1:
		_majDedLv2Weight = weight;
		break;
	case 2:
		_majDedLv3Weight = weight;
		break;
	default:
		return false;
	}

	return true;
}
