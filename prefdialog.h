#ifndef PREFDIALOG_H
#define PREFDIALOG_H

#include <QDialog>

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
};

#endif // PREFDIALOG_H
