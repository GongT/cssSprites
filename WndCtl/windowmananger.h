#ifndef WindowMananger_H
#define WindowMananger_H

#include <QObject>
#include <QList>
#include "main.h"

class WindowMananger : public QObject
{
	Q_OBJECT
	public:
	explicit WindowMananger(QObject *parent = 0, QWidget *screen = 0);
	
	class AbstractWindow* CurrentWindow(){return current;}
	int CurrentIndex(){return windows.indexOf(current);}
	void setCurrentWindow(class AbstractWindow*);
	
	class AbstractWindow* getWindow(int index = -1){return windows.value(index);}
	
	class WindowInfo getWindowInfo(int = -1);
	
	class QImage Screenshot();
	
	const QList<class AbstractWindow*>& AllTask()const;
	QString SessionSave();
	void SessionResume(QString& data);
	void restart();
	
	inline void setScreen(QWidget* sc){mScreen = sc;}
	inline QWidget* screen(){return mScreen;}
	private:
	QWidget* mScreen;
	
	signals:
	void WindowCreate(int);
	void WindowClose(int);
	void WindowChange(int);
	void WindowSelect(int);
	
	public slots:
	void CreateWindow(QWidget *wnd , const QString*Name = NULL, const QString*Id = NULL , const QString*Data = NULL , const QRect*Rect = NULL);
	void CloseWindow(int index = -1);
	void setCurrentWindow(int);
	
	private slots:
	void onWindowClicked(class AbstractWindow*);
	void onWindowChanged(class AbstractWindow*);
	void onWindowSelected(class AbstractWindow*);
	void onWindowClosed(class AbstractWindow*);
	
	private:
	QList<class AbstractWindow*> windows;
	class AbstractWindow* current;
		
};

#endif // WindowMananger_H
