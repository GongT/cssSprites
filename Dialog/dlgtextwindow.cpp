#include "dlgtextwindow.h"

dlgTextWindow::dlgTextWindow(QWidget *parent) :
	QWidget(parent),
	Layout(this),
	text(this)
{
	Layout.addWidget(&text);
}

void dlgTextWindow::setText(const QString& data){
	text.setPlainText(data);
}
