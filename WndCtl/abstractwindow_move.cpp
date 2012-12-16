#include "abstractwindow.h"

void AbstractWindow::TestPosition(int x,int y){
	if(x<mBorderLeft){ //左边缘
		if(y<mBorderBottom){ //左上角
			if(Tracking == LeftTop) return;
			this->setCursor(Qt::SizeFDiagCursor);
			Tracking = LeftTop;
		}else if(y>this->height()-mBorderBottom){//左下角
			if(Tracking == LeftBottom) return;
			this->setCursor(Qt::SizeBDiagCursor);
			Tracking = LeftBottom;
		}else{//左边缘
			if(Tracking == Left) return;
			this->setCursor(Qt::SizeHorCursor);
			Tracking = Left;
		}
	}else if(x>this->width()-mBorderRight){
		if(y<mBorderBottom){ //右上角
			if(Tracking == RightTop) return;
			this->setCursor(Qt::SizeBDiagCursor);
			Tracking = RightTop;
		}else if(y>this->height()-mBorderBottom){//右下角
			if(Tracking == RightBottom) return;
			this->setCursor(Qt::SizeFDiagCursor);
			Tracking = RightBottom;
		}else{//右边缘
			if(Tracking == Right) return;
			this->setCursor(Qt::SizeHorCursor);
			Tracking = Right;
		}
	}else{
		if(y<mBorderBottom){ //上边缘
			if(Tracking == Top) return;
			this->setCursor(Qt::SizeVerCursor);
			Tracking = Top;
		}else if(y>this->height()-mBorderBottom){//下边缘
			if(Tracking == Bottom) return;
			this->setCursor(Qt::SizeVerCursor);
			Tracking = Bottom;
		}else{//中间
			if(Tracking == Center) return;
			this->setCursor(Qt::SizeAllCursor);
			Tracking = Center;
		}
	}
}

/*bool AbstractWindow::TestNoFocusClick(const QPoint& point)const{
	const QRect rect = mContent->geometry();
	return !( point.x() < rect.left() || point.x() > rect.right() || point.y() < rect.top() || point.y() > rect.bottom() );
}*/

void AbstractWindow::MoveDispatch(const QPoint &p){
	switch(Tracking){
	case LeftTop:
		this->MoveRelative(p.x()-TrackingPoint.x(),p.y()-TrackingPoint.y());
		this->ResizeRelative(TrackingPoint.x()-p.x(),TrackingPoint.y()-p.y());
		break;
	case Top:
		this->MoveRelative(0,p.y()-TrackingPoint.y());
		this->ResizeRelative(0,TrackingPoint.y()-p.y());
		break;
	case RightTop:
		this->MoveRelative(0,p.y()-TrackingPoint.y());
		this->ResizeRelative(p.x()-TrackingPoint.x(),TrackingPoint.y()-p.y());
		break;
	case Left:
		this->MoveRelative(p.x()-TrackingPoint.x(),0);
		this->ResizeRelative(TrackingPoint.x()-p.x(),0);
		break;
	case Center:
		this->MoveRelative(p.x()-TrackingPoint.x(),p.y()-TrackingPoint.y());
		break;
	case Right:
		this->ResizeRelative(p.x()-TrackingPoint.x(),0);
		break;
	case LeftBottom:
		this->MoveRelative(p.x()-TrackingPoint.x(),0);
		this->ResizeRelative(TrackingPoint.x()-p.x(),p.y()-TrackingPoint.y());
		break;
	case Bottom:
		this->ResizeRelative(0,p.y()-TrackingPoint.y());
		break;
	case RightBottom:
		this->ResizeRelative(p.x()-TrackingPoint.x(),p.y()-TrackingPoint.y());
		break;
	default:
		break;
	}
	TrackingPoint = p;
}
