#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

void alert(const char* const msg, const char *const title = 0);
void message(const QString& msg ,const QString& title = QString());

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
	public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
	void OpenFile(const QString& file);
	void SaveFile(const QString& file);
	void ExportCss(const QString& file);
	
	public slots:
	void onMenuClicked(QAction*);
	
	protected:
	void changeEvent(QEvent *e);
	void closeEvent(QCloseEvent *e);
	
	private:
	Ui::MainWindow *ui;
	class WindowMananger* wm;
	QString CurrentFilePath;
//	class QStringList CurrentSettings;
	void ChangeInput(const class AbstractWindow *const);
	
	private slots:
	void setStatusTip(const QString&);
	void WndCreate(int);
	void WndChange(int);
	void WndClose(int);
	void WndSelect(int);
	
	void on_btnOk_pressed();
	void on_btnClose_pressed();
	
	void onScreenHeightChange(int h);
	void onScreenWidthChange(int w);
	
	
	protected:
	class DlgProperty* CurrentSetting;
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);
};

#endif // MAINWINDOW_H
