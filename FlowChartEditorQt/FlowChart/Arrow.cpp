/******************************************************************
* ���� �̸� : Arrow.cpp
* ��� : ���ἱ ��ũ��Ʈ Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 31��
*******************************************************************/

#include "Arrow.h"
#include "Painter.h"
#pragma warning (disable : 4996)
#include "FlowChartVisitor.h"

#include "ScrollController.h"
#include "Scrolls.h"

Arrow::Arrow(Long x, Long y, Long width, Long height, QColor backGroundColor,
	QPen borderLine, QColor borderColor, String contents)
	:Line(x, y, width, height, backGroundColor, borderLine, borderColor, contents) {

}

Arrow::~Arrow() {

}

Arrow::Arrow(const Arrow& source) : Line(source) {

}

Arrow& Arrow::operator =(const Arrow& source) {
	Line::operator=(source);
	
	return *this;
}

bool Arrow::IsEqual(const Shape& other) {
	bool retVo = false;
	if (dynamic_cast<Arrow *>(const_cast<Shape *>(&other))) {
		retVo = Shape::IsEqual(other);
	}
	return retVo;
}

bool Arrow::IsNotEqual(const Shape& other) {
	bool retVo = false;
	if (!dynamic_cast<Arrow *>(const_cast<Shape*>(&other))) {
		retVo = Shape::IsNotEqual(other);
	}
	return retVo;
}

bool Arrow::operator ==(const Shape& other) {
	bool retVo = false;
	if (dynamic_cast<Arrow *>(const_cast<Shape *>(&other))) {
		retVo = Shape::IsEqual(other);
	}
	return retVo;
}

bool Arrow::operator !=(const Shape& other) {
	bool retVo = false;
	if (!dynamic_cast<Arrow *>(const_cast<Shape*>(&other))) {
		retVo = Shape::IsNotEqual(other);
	}
	return retVo;
}

void Arrow::Accept(FlowChartVisitor *draw) {
	draw->Visit(this);
}
/*
void Arrow::DrawActiveShape(Painter *painter) {
	painter->DrawLine(this->x, this->y, this->x + this->width, this->y + this->height);
}
*/

Shape* Arrow::Clone() {
	return new Arrow(*this);
}

void Arrow::GetRegion(QRegion *region) {
	QRect rect(this->x - LINETHICKNESS, this->y, this->x + LINETHICKNESS, this->y + this->height);
	*region += QRegion(rect); //empty region���� �ִ� ���̸� �����ڰ� �°���? �׷��� �ƴϸ�?
}

void Arrow::GetRegion(Long thickness, QRegion *region) {
	QRect rect(this->x - thickness, this->y, this->x + thickness, this->y + this->height);
	*region += QRegion(rect);
}

bool Arrow::IsIncluded(QPoint point) {
	QRect rect(this->x - LINETHICKNESS, this->y, this->x + LINETHICKNESS, this->y + this->height);
	QRegion region(rect);
	bool ret;
	ret = region.contains(point);
	return ret;
}

bool Arrow::IsIncluded(const QRect& rect) {
	QRect regionRect(x - LINETHICKNESS, y, x + LINETHICKNESS, y + height);
	QRegion region(regionRect);
	bool ret;
	ret = region.contains(rect);
	return ret;
}

int Arrow::GetHitCode(const QPoint& point, const QRegion& region) {
	int result = HIT_NONE;
	if (region.contains(point)) {
		result = HIT_BODY;
	}
	QRect rectSelect;
	this->GetSelectionMarkerRect(HIT_IN, &rectSelect);
	if (rectSelect.contains(point)) {
		result = HIT_IN;
	}
	this->GetSelectionMarkerRect(HIT_OUT, &rectSelect);
	if (rectSelect.contains(point)) {
		result = HIT_OUT;
	}
	return result;
}

void Arrow::GetSelectionMarkerRect(int marker, QRect *rect) {
	int x;
	int y;

	switch (marker) {
	case HIT_IN: // ��ũ�� LEFTTOP
		x = this->x;
		y = this->y;
		break;

	case HIT_OUT:
		x = this->x;
		y = this->y + this->height;
		break;
	}
	rect->setCoords(x - 4, y - 4, x + 5, y + 5);
}

void Arrow::GetSelectionMarkerAllRegion(QRegion *region) {
	QRect rect;
	QRegion addRegion;
	
	Long x = this->x;
	Long y = this->y;
	rect.setCoords(x - 6, y - 6, x + 7, y + 7);
	addRegion = QRegion(rect);
	*region += addRegion;

	x = this->x;
	y = this->y + this->height;
	rect.setCoords(x - 6, y - 6, x + 7, y + 7);
	addRegion = QRegion(rect);
	*region += addRegion;
}

/*
void Arrow::DrawSelectionMarkers(Painter* painter, ScrollController *scrollController) {
	CRect rectSelect;

	painter->ChangePlaneProperty(BS_SOLID, RGB(0, 0, 255));

	GetSelectionMarkerRect(HIT_IN, &rectSelect);
	Long positionX = scrollController->GetScroll(1)->GetPosition();
	Long positionY = scrollController->GetScroll(0)->GetPosition();
	rectSelect.left -= positionX;
	rectSelect.top -= positionY;
	rectSelect.right -= positionX;
	rectSelect.bottom -= positionY;
	painter->DrawRectangle(rectSelect.left, rectSelect.top, rectSelect.right, rectSelect.bottom);

	GetSelectionMarkerRect(HIT_OUT, &rectSelect);
	rectSelect.left -= positionX;
	rectSelect.top -= positionY;
	rectSelect.right -= positionX;
	rectSelect.bottom -= positionY;
	painter->DrawRectangle(rectSelect.left, rectSelect.top, rectSelect.right, rectSelect.bottom);
}
*/

void Arrow::GetAttribute(Attribute *attribute) {
	attribute->vertexIn = 'Y';
	attribute->vertexOut = 'Y';

	attribute->pointIn.setX(this->x);
	attribute->pointIn.setY(this->y);

	attribute->pointOut.setX(this->x);
	attribute->pointOut.setY(this->y + this->height);
}

void Arrow::GetLine(char(*line)) {
	sprintf(line, "%d\t%d\t%d\t%d\t%d\t\t\t%s\n", ID_ARROW, this->x, this->y, this->width, this->height, this->contents);
}

bool Arrow::IsStyle(Long style) {
	bool ret = false;
	if (Shape::IsStyle(style) || Line::IsStyle(style) || (style >> 9) % 2) {
		ret = true;
	}
	return ret;
}