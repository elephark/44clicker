#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clickprefs.h"

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QPushButton;
class QLabel;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef struct {
	int plusClicks;
	int minusClicks;
	int majDeductLv1Clicks;
	int majDeductLv2Clicks;
	int majDeductLv3Clicks;
} ClickScore;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	/// This is so the prefs dialog has something to work with.
	ClickPrefs *prefs() const { return _prefs; }

private slots:
	void timerFinished() { timerStartPause(true); }
	void timerRedraw();
	void undoReset();
	void showPrefDialog();

private:
	Ui::MainWindow *ui;

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

	void plusClick(int qty = 1);
	void minusClick(int qty = -1);
	void majorDeductClick (int level);
	void displayClicks();
	void resetClicks();

	bool timerStartPause(bool forceStop = false);
	void timerReset();

	ClickScore _curClicks;     ///< Here we keep track of the current number of clicks
	ClickScore _lastClicks;    ///< Here we back up the clicks when resetting so we can undo

	QTimer *_fsTimer;          ///< Timer for keeping track of the fs run time
	QTimer *_dispRefreshTimer; ///< Timer for deciding when to refresh the display during the fs
	int _timeRemaining;        ///< How much time is left in the freestyle?

	ClickPrefs *_prefs;
};
#endif // MAINWINDOW_H
