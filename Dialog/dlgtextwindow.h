#ifndef DLGTEXTWINDOW_H
#define DLGTEXTWINDOW_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QHBoxLayout>

class dlgTextWindow : public QWidget
{
	Q_OBJECT
	public:
	explicit dlgTextWindow(QWidget *parent = 0);
	void setText(const QString&);
	
	signals:
	
	public slots:
	
	
	private:
	QHBoxLayout Layout;
	QPlainTextEdit text;
};

#endif // DLGTEXTWINDOW_H
