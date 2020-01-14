#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowTitle("44clicker");

	// wipe _curClicks
	resetClicks();

	// reset _lastClicks so we don't restore garbage
	_lastClicks.plusClicks = 0;
	_lastClicks.minusClicks = 0;
	_lastClicks.majDeductLv1Clicks = 0;
	_lastClicks.majDeductLv2Clicks = 0;
	_lastClicks.majDeductLv3Clicks = 0;

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
	// An attempt at defeating autorepeat. Unfortunately not consistent.
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
 * @brief Adjusts
 * @param level Which level of major deduct
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











