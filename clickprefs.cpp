#include "clickprefs.h"
#include <QSettings>

ClickPrefs::ClickPrefs(QObject *parent)
    : QObject(parent)
{
	// Load settings, assigning defaults if none exist.
	QSettings settings("Elephark", "44clicker");
	_majDedWeight[0] = settings.value("majDedWeight0", DEFAULT_MD_LV1_WEIGHT).toInt();
	_majDedWeight[1] = settings.value("majDedWeight1", DEFAULT_MD_LV2_WEIGHT).toInt();
	_majDedWeight[2] = settings.value("majDedWeight2", DEFAULT_MD_LV3_WEIGHT).toInt();

	_multiClickWeight = settings.value("multiClickWeight", DEFAULT_MULTICLICK_WEIGHT).toInt();
	_totalTimeSetting = settings.value("totalTimeSetting", DEFAULT_FREESTYLE_LENGTH).toUInt();
	_timerDisplayRefresh = settings.value("timerDisplayRefresh", DEFAULT_DISPLAY_REFRESH).toInt();
	_timerDigits = settings.value("timerDigits", DEFAULT_TIMER_DIGITS).toInt();
	_timerVisible = settings.value("timerVisible", DEFAULT_TIMER_VISIBILITY).toInt();
}

/**
 * @brief Save settings to disk.
 * @return True.
 */
bool ClickPrefs::writePrefs()
{
	QSettings settings("Elephark", "44clicker");

	settings.setValue("majDedWeight0", _majDedWeight[0]);
	settings.setValue("majDedWeight1", _majDedWeight[1]);
	settings.setValue("majDedWeight2", _majDedWeight[2]);

	settings.setValue("multiClickWeight", _multiClickWeight);
	settings.setValue("totalTimeSetting", _totalTimeSetting);
	settings.setValue("timerDisplayRefresh", _timerDisplayRefresh);
	settings.setValue("timerDigits", _timerDigits);
	settings.setValue("timerVisible", _timerVisible);

	// todo: Maybe give it a meaningful truth value?
	return true;
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

