#ifndef CLICKPREFS_H
#define CLICKPREFS_H

#include <QObject>

class ClickPrefs : public QObject
{
	Q_OBJECT
public:
	explicit ClickPrefs(QObject *parent = nullptr);

signals:

};

#endif // CLICKPREFS_H
