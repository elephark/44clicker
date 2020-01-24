#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>

/// Default refresh rate in ms while the fs timer is running.
#define DEFAULT_DISPLAY_REFRESH 71
/// Default length of a freestyle, in seconds
#define DEFAULT_FREESTYLE_LENGTH 60

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _fsTimer(new QTimer(this))
    , _dispRefreshTimer(new QTimer(this))
    , _totalTimeSetting(DEFAULT_FREESTYLE_LENGTH * 1000)
    , _timerDisplayRefresh(DEFAULT_DISPLAY_REFRESH)
{
	ui->setupUi(this);
	setWindowTitle("44clicker");

	// Wipe _curClicks.
	resetClicks();

	// Reset _lastClicks so we don't restore garbage.
	_lastClicks.plusClicks = 0;
	_lastClicks.minusClicks = 0;
	_lastClicks.majDeductLv1Clicks = 0;
	_lastClicks.majDeductLv2Clicks = 0;
	_lastClicks.majDeductLv3Clicks = 0;

	// Strive for millisecond accuracy.
	_fsTimer->setTimerType(Qt::PreciseTimer);
	_fsTimer->setSingleShot(true);
	connect(_fsTimer, SIGNAL(QTimer::timeout), this, SLOT(timerFinished()));

	// We're less concerned about _dispRefreshTimer's accuracy. It doesn't really matter.
	// Let's hook it up.
	connect(_dispRefreshTimer, SIGNAL(QTimer::timeout), this, SLOT(timerRedraw()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

/**
 * @brief Handles key presses.
 * @param event A keypress event.
 */
void MainWindow::keyPressEvent(QKeyEvent *event) {
	// An attempt at defeating autorepeat. Kinda works, but unfortunately not consistently.
	if (event->isAutoRepeat()) {return;}

	switch(event->key()) {
	case Qt::Key_R:
		// Reset the score
		resetClicks();
		break;
	case Qt::Key_U:
		// Undo the last reset
		undoReset();
		break;
	case Qt::Key_Slash:
		// One plus click
		plusClick();
		break;
	case Qt::Key_Period:
		// Three plus clicks
		plusClick(3);
		break;
	case Qt::Key_Z:
		// One minus click
		minusClick();
		break;
	case Qt::Key_X:
		// Three minus clicks
		minusClick(-3);
		break;
	case Qt::Key_A:
		// Major deduct level 1 (1 by default)
		majorDeductClick(1);
		break;
	case Qt::Key_S:
		// Major deduct level 2 (3 by default)
		majorDeductClick(2);
		break;
	case Qt::Key_D:
		// Major deduct level 3 (5 by default)
		majorDeductClick(3);
		break;
	case Qt::Key_Q:
		// Start/pause the timer
		timerStartPause();
		break;
	case Qt::Key_W:
		// Reset the timer
		timerReset();
		break;
	default:
//		QFrame::keyPressEvent(event);
		break;
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
	// todo?
	(void)event;
}

/**
 * @brief Adjusts the plus click counter, then updates the total clicks.
 * @param qty How many clicks to add (normally this will be 1).
 */
void MainWindow::plusClick(int qty) {
	_curClicks.plusClicks += qty;
	ui->plusClicksLcd->display(_curClicks.plusClicks);
	updateTotalClicks();
}

/**
 * @brief Adjusts the minus click counter, then updates the total clicks.
 * @param qty How many clicks to add (normally this will be -1).
 */
void MainWindow::minusClick(int qty) {
	_curClicks.minusClicks += qty;
	ui->minusClicksLcd->display(_curClicks.minusClicks);
	updateTotalClicks();
}

/**
 * @brief Adjusts counters for major deducts, then updates the total clicks.
 * @param level Which level of major deduct.
 */
void MainWindow::majorDeductClick (int level) {
	switch(level) {
	case 1:
		_curClicks.majDeductLv1Clicks++;
		ui->majDeductLv1Lcd->display(_curClicks.majDeductLv1Clicks);
		break;
	case 2:
		_curClicks.majDeductLv2Clicks += 3; // todo: magic numberzzzzz
		ui->majDeductLv2Lcd->display(_curClicks.majDeductLv2Clicks);
		break;
	case 3:
		_curClicks.majDeductLv3Clicks += 5;
		ui->majDeductLv3Lcd->display(_curClicks.majDeductLv3Clicks);
		break;
	default:
		break;
	}
	updateTotalClicks();
}

/**
 * @brief Updates the total clicks and total major deducts LCDs.
 */
void MainWindow::updateTotalClicks() {
	ui->totalClicksLcd->display(_curClicks.plusClicks + _curClicks.minusClicks);
	ui->majDeductTotalLcd->display(_curClicks.majDeductLv1Clicks + _curClicks.majDeductLv2Clicks + _curClicks.majDeductLv3Clicks);
}

/**
 * @brief Resets all clicks to zero so we can start over.
 */
void MainWindow::resetClicks() {
	// Back up first
	_lastClicks.plusClicks = _curClicks.plusClicks;
	_lastClicks.minusClicks = _curClicks.minusClicks;
	_lastClicks.majDeductLv1Clicks = _curClicks.majDeductLv1Clicks;
	_lastClicks.majDeductLv2Clicks = _curClicks.majDeductLv2Clicks;
	_lastClicks.majDeductLv3Clicks = _curClicks.majDeductLv3Clicks;

	// Do the reset part
	_curClicks.plusClicks = 0;
	_curClicks.minusClicks = 0;
	ui->plusClicksLcd->display(_curClicks.plusClicks);
	ui->minusClicksLcd->display(_curClicks.minusClicks);

	_curClicks.majDeductLv1Clicks = 0;
	_curClicks.majDeductLv2Clicks = 0;
	_curClicks.majDeductLv3Clicks = 0;
	ui->majDeductLv1Lcd->display(_curClicks.majDeductLv1Clicks);
	ui->majDeductLv2Lcd->display(_curClicks.majDeductLv2Clicks);
	ui->majDeductLv3Lcd->display(_curClicks.majDeductLv3Clicks);

	updateTotalClicks();
}

/**
 * @brief Performs a (single-level) undo operation in case we reset accidentally.
 */
void MainWindow::undoReset() {
	// todo: this, more easily, with memcpy? oops, too late!
	_curClicks.plusClicks = _lastClicks.plusClicks;
	_curClicks.minusClicks = _lastClicks.minusClicks;
	_curClicks.majDeductLv1Clicks = _lastClicks.majDeductLv1Clicks;
	_curClicks.majDeductLv2Clicks = _lastClicks.majDeductLv2Clicks;
	_curClicks.majDeductLv3Clicks = _lastClicks.majDeductLv3Clicks;

	ui->plusClicksLcd->display(_curClicks.plusClicks);
	ui->minusClicksLcd->display(_curClicks.minusClicks);
	ui->majDeductLv1Lcd->display(_curClicks.majDeductLv1Clicks);
	ui->majDeductLv2Lcd->display(_curClicks.majDeductLv2Clicks);
	ui->majDeductLv3Lcd->display(_curClicks.majDeductLv3Clicks);

	updateTotalClicks();
}

/**
 * @brief MainWindow::timerStartPause
 *
 * @param forceStop Set to true to guarantee a stop.
 *
 * @return True if we just started the timer, false if we just paused.
 */
bool MainWindow::timerStartPause(bool forceStop) {


	// If the timer is already running, pause it.
	if (_fsTimer->isActive() || forceStop) { // todo: double check to make sure this isn't a bug
		// Cache the remaining time and stop the fs timer.
		_timeRemaining = _fsTimer->remainingTime();
		_fsTimer->stop();

		// Stop the refresh timer.
		_dispRefreshTimer->stop();

		// One last redraw of the timer values.
		timerRedraw();
		return false;
	}
	// If the timer isn't running, start it.
	else {
		// Load the countdown timer with the total remaining time and kick it off.
		_fsTimer->start(_timeRemaining);

		// Kick off the refresh timer.
		_dispRefreshTimer->start(_timerDisplayRefresh);

		// redraw probably not necessary tbh fam
		return true;
	}
}

/**
 * @brief MainWindow::timerReset
 */
void MainWindow::timerReset() {
	// Stop everything.
	_fsTimer->stop();
	_dispRefreshTimer->stop();

	// Reload things afresh.
	_timeRemaining = _totalTimeSetting;

	// Redraw timer values.
	timerRedraw();
}


void MainWindow::timerRedraw() {
	// todo
}









