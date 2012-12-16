#include "imageframe.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>
#include <QPainter>
#include <QMetaProperty>

ImageFrame::ImageFrame(QWidget *parent) :
	QWidget(parent),
	mType("Image")
{
	img.load(":/new-image.png","PNG");
	icon.addPixmap(QPixmap::fromImage(img));
	this->setBackgroundRole(QPalette::Base);
}

ImageFrame::~ImageFrame()
{
}

void ImageFrame::paintEvent(QPaintEvent *e){
	QPainter p;
	p.begin(this);
	//p.setOpacity(0.8);
	//p.fillRect(QRect(QPoint(0,0),this->size()),Qt::red);
	//p.setOpacity(1);
	p.drawImage(QRect(QPoint(0,0),this->size()),img);
	p.end();
	e->accept();
}
