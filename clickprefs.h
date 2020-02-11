#ifndef CLICKPREFS_H
#define CLICKPREFS_H

#include <QObject>

/// Default number of clicks to give for each press of the multiclick button.
#define DEFAULT_MULTICLICK_WEIGHT 3
/// Default number of points for major deduction level 1 (restart).
#define DEFAULT_MD_LV1_WEIGHT 1
/// Default number of points for major deduction level 2 (switch).
#define DEFAULT_MD_LV2_WEIGHT 3
/// Default number of points for major deduction level 3 (string break).
#define DEFAULT_MD_LV3_WEIGHT 5
/// Default length of a freestyle, in ms.
#define DEFAULT_FREESTYLE_LENGTH 120000
/// Default refresh rate in ms while the fs timer is running. Should be prime.
#define DEFAULT_DISPLAY_REFRESH 47
/// Default number of digits to show for partial seconds.
#define DEFAULT_TIMER_DIGITS 2

class ClickPrefs : public QObject
{
	Q_OBJECT
public:
	explicit ClickPrefs(QObject *parent = nullptr);

	/// Getters with obvious functionality.
	int multiClickWeight() const { return _multiClickWeight; }
	int MD1Weight() const { return _majDedLv1Weight; }
	int MD2Weight() const { return _majDedLv2Weight; }
	int MD3Weight() const { return _majDedLv3Weight; }
	int totalTimeSetting() const { return _totalTimeSetting; }
	int timerDisplayRefresh() const { return _timerDisplayRefresh; }

	/// Setters with obvious functionality.
	void setMultiClickWeight(int weight) { _multiClickWeight = weight; }
	void setMD1Weight(int weight) { _majDedLv1Weight = weight; }
	void setMD2Weight(int weight) { _majDedLv2Weight = weight; }
	void setMD3Weight(int weight) { _majDedLv3Weight = weight; }
	void setTotalTimeSetting(int totalTime) { _totalTimeSetting = totalTime; }
	void setTimerDisplayRefresh(int timeout) { _timerDisplayRefresh = timeout; }

	bool setMDWeight(int level, int weight); // todo: move all the MD functionality to this style

private:
	int _multiClickWeight;      ///< Weight of each multiclick button. Plus and minus are the same.

	/// Major deduct levels
	int _majDedLv1Weight;
	int _majDedLv2Weight;
	int _majDedLv3Weight;

	int _totalTimeSetting;     ///< Freestyle length, in milliseconds.
	int _timerDisplayRefresh;  ///< Time in ms between display updates while _fsTimer is running
	// todo: flash timer when it finishes?
//	bool

	// todo: appearance settings
	int _timerDigits;          ///< How many digits we'll show for partial seconds

	// todo: key settings

};

#endif // CLICKPREFS_H
