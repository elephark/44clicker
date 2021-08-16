#ifndef PREFDIALOG_H
#define PREFDIALOG_H

#include <QDialog>
#include "clickprefs.h"

namespace Ui {
class PrefDialog;
}

class PrefDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PrefDialog(QWidget *parent = nullptr);
	~PrefDialog();

private:
	Ui::PrefDialog *ui;

	ClickPrefs *_prefs = nullptr;    ///< Local pointer to the program's main prefs instance.

private slots:
	void savePrefs();
	void restoreDefaults();
};

#endif // PREFDIALOG_H
