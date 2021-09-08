#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include "prefdialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _fsTimer(new QTimer(this))
    , _dispRefreshTimer(new QTimer(this))
    , _prefs(new ClickPrefs(this))
{
	ui->setupUi(this);
	setWindowTitle("44clicker");

	// Exit strategy.
	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	// Set up the undo signal.
	connect(ui->actionUndoReset, SIGNAL(triggered()), this, SLOT(undoReset()));

	// We probably want to be able to get to the pref dialog...
	connect(ui->actionPrefs, SIGNAL(triggered()), this, SLOT(showPrefDialog()));

	// Initialize the click structs to nice happy known values.
	_curClicks.plusClicks = 0;
	_curClicks.minusClicks = 0;
	_curClicks.majDeductLv1Clicks = 0;
	_curClicks.majDeductLv2Clicks = 0;
	_curClicks.majDeductLv3Clicks = 0;

	_lastClicks.plusClicks = 0;
	_lastClicks.minusClicks = 0;
	_lastClicks.majDeductLv1Clicks = 0;
	_lastClicks.majDeductLv2Clicks = 0;
	_lastClicks.majDeductLv3Clicks = 0;

	displayClicks();

	// Strive for millisecond accuracy.
	_fsTimer->setTimerType(Qt::PreciseTimer);
	_fsTimer->setSingleShot(true);
	_fsTimer->setInterval(_prefs->totalTimeSetting());
	connect(_fsTimer, SIGNAL(timeout()), this, SLOT(timerFinished()));
	_timeRemaining = _prefs->totalTimeSetting();

	// We're less concerned about _dispRefreshTimer's accuracy. It doesn't really matter.
	// Let's hook it up.
	connect(_dispRefreshTimer, SIGNAL(timeout()), this, SLOT(timerRedraw()));

	timerRedraw();
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
	case Qt::Key_Slash:
		// One plus click
		plusClick();
		break;
	case Qt::Key_Period:
		// Three plus clicks
		plusClick(_prefs->multiClickWeight());
		break;
	case Qt::Key_Z:
		// One minus click
		minusClick();
		break;
	case Qt::Key_X:
		// Three minus clicks
		minusClick(_prefs->multiClickWeight() * -1);
		break;
	case Qt::Key_A:
		// Major deduct level 1 (restart)
		majorDeductClick(1);
		break;
	case Qt::Key_S:
		// Major deduct level 2 (switch)
		majorDeductClick(2);
		break;
	case Qt::Key_D:
		// Major deduct level 3 (detach)
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
 * @brief Adjusts the plus click counter, then updates the clicks.
 * @param qty How many clicks to add (normally this will be 1).
 */
void MainWindow::plusClick(int qty) {
	_curClicks.plusClicks += qty;
	displayClicks();
}

/**
 * @brief Adjusts the minus click counter, then updates the clicks.
 * @param qty How many clicks to add (normally this will be -1).
 */
void MainWindow::minusClick(int qty) {
	_curClicks.minusClicks += qty;
	displayClicks();
}

/**
 * @brief Adjusts counters for major deducts, then updates the clicks.
 * @param level Which level of major deduct.
 */
void MainWindow::majorDeductClick (int level) {
	switch(level) {
	case 1:
		_curClicks.majDeductLv1Clicks += _prefs->MDWeight(0); // todo: zero vs one indexinggggg
		break;
	case 2:
		_curClicks.majDeductLv2Clicks += _prefs->MDWeight(1);
		break;
	case 3:
		_curClicks.majDeductLv3Clicks += _prefs->MDWeight(2);
		break;
	default:
		break;
	}
	displayClicks();
}

/**
 * @brief Updates the LCDs showing clicks and major deducts.
 */
void MainWindow::displayClicks() {
	ui->plusClicksLcd->display(_curClicks.plusClicks);
	ui->minusClicksLcd->display(_curClicks.minusClicks);
	ui->totalClicksLcd->display(_curClicks.plusClicks + _curClicks.minusClicks);

	ui->majDeductLv1Lcd->display(_curClicks.majDeductLv1Clicks);
	ui->majDeductLv2Lcd->display(_curClicks.majDeductLv2Clicks);
	ui->majDeductLv3Lcd->display(_curClicks.majDeductLv3Clicks);
	ui->majDeductTotalLcd->display(_curClicks.majDeductLv1Clicks + _curClicks.majDeductLv2Clicks + _curClicks.majDeductLv3Clicks);
}

/**
 * @brief Resets all clicks to zero so we can start over.
 */
void MainWindow::resetClicks() {
	// Make it so we can undo.
	// Note that this doesn't check if anything was actually changed. If they start up, immediately
	// reset, click some, and then undo, everything will "undo" back to zero, which is kinda weird.
	// Further note that I don't much care what happens if someone really wants to do that.
	ui->actionUndoReset->setEnabled(true);

	// Back up first
	_lastClicks.plusClicks = _curClicks.plusClicks;
	_lastClicks.minusClicks = _curClicks.minusClicks;
	_lastClicks.majDeductLv1Clicks = _curClicks.majDeductLv1Clicks;
	_lastClicks.majDeductLv2Clicks = _curClicks.majDeductLv2Clicks;
	_lastClicks.majDeductLv3Clicks = _curClicks.majDeductLv3Clicks;

	// Do the reset part
	_curClicks.plusClicks = 0;
	_curClicks.minusClicks = 0;

	_curClicks.majDeductLv1Clicks = 0;
	_curClicks.majDeductLv2Clicks = 0;
	_curClicks.majDeductLv3Clicks = 0;

	displayClicks();
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

	displayClicks();
}

/**
 * @brief Starts or stops (whichever makes sense) the freestyle timer.
 * Also restarts it if the time was up.
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

		// Hold bounds.
		if (_timeRemaining < 0) { _timeRemaining = 0; }

		// Stop the refresh timer.
		_dispRefreshTimer->stop();

		// One last redraw of the timer values.
		timerRedraw();
		return false;
	}
	// If the timer isn't running, start it.
	else {
		// Automatically restart if the timer has expired.
		if (!_timeRemaining) { _timeRemaining = _prefs->totalTimeSetting(); }

		// Load the countdown timer with the total remaining time and kick it off.
		_fsTimer->start(_timeRemaining);

		// Kick off the refresh timer.
		_dispRefreshTimer->start(_prefs->timerDisplayRefresh());

		// redraw probably not necessary tbh fam
		return true;
	}
}

/**
 * @brief Resets the timer. Also stops it if it was running.
 */
void MainWindow::timerReset() {
	// Stop everything.
	_fsTimer->stop();
	_dispRefreshTimer->stop();

	// Reload things afresh.
	_timeRemaining = _prefs->totalTimeSetting();

	// Redraw timer values.
	timerRedraw();
}

/**
 * @brief Redraws the LCD elements comprising the freestyle timer.
 *
 * Note that this function assumes _prefs->timerDigits is in range [0, 2].
 */
void MainWindow::timerRedraw() {
	// Cache the remaining time.
	if (_fsTimer->isActive()) {
		_timeRemaining = _fsTimer->remainingTime();
	}

	// Center the digits, in the way a QLCDNumber understands.
	ui->timeElapsedLcd->setDigitCount(5 + (_prefs->timerDigits() ? _prefs->timerDigits() + 1 : 0));
	ui->timeRemainingLcd->setDigitCount(5 + (_prefs->timerDigits() ? _prefs->timerDigits() + 1 : 0));

	// Figure out how much time is left.
	int timeElapsed = _prefs->totalTimeSetting() - _timeRemaining;
	// Make things add up on screen.
	if      (_prefs->timerDigits() == 1) { timeElapsed += 99; }
	else if (_prefs->timerDigits() == 2) { timeElapsed += 9; }

	if (timeElapsed < 0) { timeElapsed = 0; } // PARANOiA.mp3

	// Divide time remaining down to minutes, seconds, smaller units, and push it out to the screen.
	int mm = _timeRemaining / (60 * 1000);
	// That ternary makes bare seconds behave more like you'd expect them to. Silly integer math.
	int ss = ((_timeRemaining % (60 * 1000)) + (_prefs->timerDigits() ? 0 : 999)) / 1000;
	// Oh, we also don't want 2:00 to display as 1:60 when the timer is running.
	// There's probably a much more elegant way to handle all this. I'm sorry.
	if (ss == 60) {
		ss = 0;
		mm++;
	}
	int cc = (_timeRemaining % 1000) / 10;
	if (_prefs->timerDigits() == 1) { cc /= 10; }
	QString dispText;

	if (_prefs->timerDigits()) {
		dispText = QString("%1:%2.%3")
		        .arg(mm, 2, 10, QChar('0'))
		        .arg(ss, 2, 10, QChar('0'))
		        .arg(cc, _prefs->timerDigits(), 10, QChar('0'));
	} else { // timerDigits == 0
		dispText = QString("%1:%2")
		        .arg(mm, 2, 10, QChar('0'))
		        .arg(ss, 2, 10, QChar('0'));
	}
	ui->timeRemainingLcd->display(dispText);

	// Do it again for time elapsed.
	mm = timeElapsed / (60 * 1000);
	ss = (timeElapsed % (60 * 1000)) / 1000;
	cc = (timeElapsed % 1000) / 10;
	if (_prefs->timerDigits() == 1) { cc /= 10; }

	if (_prefs->timerDigits()) {
		dispText = QString("%1:%2.%3")
		        .arg(mm, 2, 10, QChar('0'))
		        .arg(ss, 2, 10, QChar('0'))
		        .arg(cc, _prefs->timerDigits(), 10, QChar('0'));
	} else { // timerDigits == 0
		dispText = QString("%1:%2")
		        .arg(mm, 2, 10, QChar('0'))
		        .arg(ss, 2, 10, QChar('0'));
	}
	ui->timeElapsedLcd->display(dispText);
}

/**
 * @brief Slot for bringing up a preferences dialog.
 */
void MainWindow::showPrefDialog() {
	PrefDialog d(this);
	if (d.exec() == QDialog::Accepted) {
		// They clicked OK, so refresh anything the new prefs would affect.
		timerRedraw();
	}
}
