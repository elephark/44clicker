#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
	Ui::MainWindow *ui;

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

	void plusClick(int qty = 1);
	void minusClick(int qty = -1);
	void majorDeductClick (int level);
	void updateTotalClicks();
	void resetClicks();
	void undoReset();

	void timerStartPause();
	void timerReset();

	ClickScore _curClicks;
	ClickScore _lastClicks;

	QTimer *_fsTimer;

//	QLCDNumber *totalClicksLcd;
//	QLCDNumber *plusClicksLcd;
//	QLCDNumber *minusClicksLcd;

};
#endif // MAINWINDOW_H
