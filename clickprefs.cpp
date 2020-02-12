#include "clickprefs.h"

ClickPrefs::ClickPrefs(QObject *parent)
    : QObject(parent)
    , _multiClickWeight(DEFAULT_MULTICLICK_WEIGHT)
    , _totalTimeSetting(DEFAULT_FREESTYLE_LENGTH)
    , _timerDisplayRefresh(DEFAULT_DISPLAY_REFRESH)
    , _timerDigits(DEFAULT_TIMER_DIGITS)
    , _timerVisible(DEFAULT_TIMER_VISIBILITY)
{
	_majDedWeight[0] = DEFAULT_MD_LV1_WEIGHT;
	_majDedWeight[1] = DEFAULT_MD_LV2_WEIGHT;
	_majDedWeight[2] = DEFAULT_MD_LV3_WEIGHT;

	// todo: save/load settings
}

/**
 * @brief Getter for major deduct weights.
 * @param level Which (zero-indexed) MD level we want to know about.
 * @return The weight assigned to the specified MD level.
 */
int ClickPrefs::MDWeight(int level) const {
	if (level >= 0 && level < MD_LEVEL_COUNT) { return _majDedWeight[level]; }
	else { return 0; }
}

/**
 * @brief Sets the weight, in points, of a major deduct level.
 * @param level Which (zero-indexed) MD level will be set.
 * @param weight The weight to be assigned to the specified MD level.
 * @return True if weight was set. False if you, oh, I dunno, tried to do something dumb.
 */
bool ClickPrefs::setMDWeight(int level, int weight) {
	if (level >= 0 && level < MD_LEVEL_COUNT) {
		_majDedWeight[level] = weight;
		return true;
	}
	else { return false; }
}

