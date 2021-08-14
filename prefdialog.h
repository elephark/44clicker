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

	ClickPrefs *p = nullptr;    ///< todo: comment, and also rename
	ClickPrefs *_tmpPrefs;      ///< Where we'll store things while we decide how to run things.

private slots:
	void savePrefs();
};

#endif // PREFDIALOG_H
