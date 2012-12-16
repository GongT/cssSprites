#include "abstractwindow.h"
#include <QString>
#include <QVariant>
#include <QMouseEvent>
#include <QDebug>
#include <QTextStream>
#include <QPainter>
#include <QIcon>
#include <QMetaObject>
#include <QMetaMethod>
#include<QHBoxLayout>


AbstractWindow::AbstractWindow(QWidget *screen, QWidget *content) :
	QWidget(screen),
	mFocus(false),
	mScreen(screen),
	mContent(NULL),
	isMouseDown(false),
	Tracking(None)
{
	setContent(content);
	this->setMouseTracking(true);
	this->setFocusPolicy(Qt::StrongFocus);
	
	setBorderTop(25);
	setBorderLeft(5);
	setBorderRight(5);
	setBorderBottom(15);
	
	emit onOpen(this);
}

QWidget* AbstractWindow::Content() const{
	return mContent;
}

QWidget* AbstractWindow::setContent(QWidget*lpWindow){
	if(mContent){
		mContent->setFocusProxy(NULL);
		mContent->setParent(NULL);
		mContent->setCursor(Qt::CustomCursor);
	}
	QWidget* Old = mContent;
	mContent = lpWindow;
	mContent->setParent(this);
	
	mContent->setFocusProxy(this);
	
	return Old;
}

void AbstractWindow::focusInEvent(QFocusEvent *e){
	setFocus(true);
	e->accept();
}

void AbstractWindow::focusOutEvent(QFocusEvent *e){
	setFocus(false);
	e->accept();
}

void AbstractWindow::mousePressEvent(QMouseEvent *e){
	if(!mFocus) return; //如果没有焦点
	if(e->button() == Qt::LeftButton){
		TestPosition(e->x(),e->y());
		isMouseDown = true;
		TrackingPoint = e->globalPos();
		e->accept();
	}else if(e->button() == Qt::RightButton && COLLISION(e->pos(),mTitleButton) ){
		MenuRequested();
	}
}

void AbstractWindow::mouseReleaseEvent(QMouseEvent *e){
	if(mFocus){
		isMouseDown = false;
		Tracking = None;
		
		emit onRectChange(this);
		e->accept();
	}
}

void AbstractWindow::mouseMoveEvent(QMouseEvent *e){
	if(!mFocus) return;
	if(isMouseDown){
		MoveDispatch(e->globalPos());
		e->accept();
	}else{
		int x = e->x();
		int y = e->y();
		TestPosition(x,y);
		e->accept();
	}
}

void AbstractWindow::resizeEvent(QResizeEvent *e){
	mContent->resize(this->size() - getBorderSize());
	e->accept();
}

void AbstractWindow::closeEvent(QCloseEvent *e){
	emit onClose(this);
	e->accept();
}

void AbstractWindow::paintEvent(QPaintEvent *e){
	QPainter pr;
	pr.begin(this);
	if( mFocus ){
		QPen pen;
		pen.setColor(Qt::black);
		QBrush brush;
		
		//边框
		pen.setWidth(3);
		pr.setPen(pen);
		brush.setColor(Qt::cyan);
		brush.setStyle(Qt::SolidPattern);
		pr.setOpacity(0.3);
		pr.setBrush(brush);
		pr.drawRect(QRect(QPoint(0,0),this->size()));
		pr.setOpacity(1);
		
		//“内容”区
		pen.setWidth(1);
		pr.setPen(Qt::NoPen);
		brush.setColor(Qt::white);
		brush.setStyle(Qt::Dense2Pattern);
		pr.setBrush(brush);
		pr.drawRect( QRect(getBorderPos(),mContent->size()) );
		
		//标题区 按钮
		mTitleButton = QRect(this->width()-mBorderTop-mBorderRight+4,3,mBorderTop-5,mBorderTop-5);
		brush.setStyle(Qt::SolidPattern);
		pen.setWidth(2);
		pen.setColor(Qt::gray);
		pr.setPen(pen);
		pr.setBrush(brush);
		pr.drawRect(mTitleButton);
		//标题区 横线
		pen.setWidth(3);
		pen.setColor(Qt::white);
		pr.drawLine(QPointF(mTitleButton.left()+3,mTitleButton.height()/2+3),QPointF(mTitleButton.right()-3,mTitleButton.height()/2+3));
		//标题区 标题
		pen.setColor(Qt::black);
		pr.setPen(pen);
		pr.drawText(QRect(mBorderLeft,2, 800 ,mBorderTop),this->getTitle());
	}else{
		QPen pen;
		pen.setColor(Qt::black);
		//边框
		pen.setWidth(1);
		pen.setStyle(Qt::DashLine);
		pr.setPen(pen);
		pr.drawRect(mContent->geometry());
	}
	//Draw(this);
	pr.end();
	
	e->accept();
}

void AbstractWindow::moveEvent(QMoveEvent *){
}

void AbstractWindow::Draw(QPaintDevice * dev){
	if(dev==this){
		mContent->render(dev, getPos() ,QRegion() , NULL );
	}else{
		mContent->render(dev, getPos() ,QRegion() , NULL );
	}
}

void AbstractWindow::ChangeInfo(const QString*,const QString*,const QString*,const QRect*){
	
}

QRect AbstractWindow::getContentRect()const{
	return QRect( QPoint(x(),y())+getBorderPos() , size()-getBorderSize() );
}

void AbstractWindow::MoveRelative(const QPoint& point){ //相对移动
	this->move(this->geometry().topLeft() + point);
}

void AbstractWindow::MoveRelative(const int &x, const int &y){ //相对移动
	this->move(this->x()+x,this->y()+y);
}

void AbstractWindow::MoveAbsolute(const QPoint& point){ //绝对移动
	this->move(point - getBorderPos());
}

void AbstractWindow::ResizeRelative(const QSize&size){ //相对大小
	this->resize(this->geometry().size() + size);
}

void AbstractWindow::ResizeRelative(const int &w, const int &h){ //相对大小
	this->resize(this->width()+w,this->height()+h);
}

void AbstractWindow::ResizeAbsolute(const QSize&size){ //绝对大小
	this->resize(size + getBorderSize());
}

void AbstractWindow::setFocus(bool focus){
	if(focus==mFocus) return;
	mFocus = focus;
	if(focus){
		mContent->setCursor(Qt::SizeAllCursor);
		this->setCursor(Qt::SizeAllCursor);
		QWidget::setFocus(Qt::MouseFocusReason);
		onFocus(this);
	}else{
		mContent->setCursor(Qt::ArrowCursor);
		this->setCursor(Qt::ArrowCursor);
		QWidget::clearFocus();
	}
	this->repaint();
}

void AbstractWindow::MenuRequested(){
	qDebug()<<"MenuRequested";
}
