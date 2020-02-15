/******************************************************************
* ���� �̸� : Process.cpp
* ��� : ó����ȣ�� ��ũ��Ʈ Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 30��
*******************************************************************/

#include "Process.h"
#include "Painter.h"
#include "FlowChartVisitor.h"
#pragma warning (disable : 4996)

Process::Process(Long x, Long y, Long width, Long height, QColor backGroundColor,
	QPen borderLine, QColor borderColor, String contents)
	: Symbol(x, y, width, height, backGroundColor, borderLine, borderColor, contents) {

}

Process::~Process() {

}

Process::Process(const Process& source) :Symbol(source) {

}

Process& Process::operator =(const Process& source) {
	Symbol::operator=(source);

	return *this;
}

bool Process::IsEqual(const Shape& other) {
	bool retVo = false;
	if (dynamic_cast<Process *>(const_cast<Shape *>(&other))) {
		retVo = Shape::IsEqual(other);
	}
	return retVo;
}

bool Process::IsNotEqual(const Shape& other) {
	bool retVo = false;
	if (!dynamic_cast<Process *>(const_cast<Shape*>(&other))) {
		retVo = Shape::IsNotEqual(other);
	}
	return retVo;
}

bool Process::operator ==(const Shape& other) {
	bool retVo = false;
	if (dynamic_cast<Process *>(const_cast<Shape *>(&other))) {
		retVo = Shape::IsEqual(other);
	}
	return retVo;
}

bool Process::operator !=(const Shape& other) {
	bool retVo = false;
	if (!dynamic_cast<Process *>(const_cast<Shape*>(&other))) {
		retVo = Shape::IsNotEqual(other);
	}
	return retVo;
}

void Process::Accept(FlowChartVisitor *draw) {
	draw->Visit(this);
}

Shape* Process::Clone() {
	return new Process(*this);
}

void Process::GetRegion(QRegion *region) {
	QRect rect;
	rect.setCoords(this->x, this->y, this->x + this->width, this->y + this->height);
	QRegion addRegion(rect);
	*region += addRegion;
}

void Process::GetRegion(Long thickness, QRegion *region) {
	Long x = this->x - thickness;
	Long y = this->y - thickness;
	Long width = this->width + thickness * 2;
	Long height = this->height + thickness * 2;

	QRect rect;
	rect.setCoords(this->x, this->y, this->x + this->width, this->y + this->height);
	QRegion addRegion(rect);
	*region += addRegion;
}

bool Process::IsIncluded(QPoint point) {
	bool ret;

	QRect rect;
	rect.setCoords(this->x, this->y, this->x + this->width, this->y + this->height);
	QRegion region(rect);
	ret = region.contains(point);
	
	return ret;
}

bool Process::IsIncluded(const QRect& rect) {
	bool ret;

	QRect regionRect;
	regionRect.setCoords(this->x, this->y, this->x + this->width, this->y + this->height);
	QRegion region(regionRect);
	ret = region.contains(rect);

	return ret;
}

void Process::GetAttribute(Attribute *attribute) {
	attribute->vertexIn = 'Y';
	attribute->vertexOut = 'Y';

	attribute->pointIn.setX(this->x + this->width / 2);
	attribute->pointIn.setY(this->y);

	attribute->pointOut.setX(this->x + this->width / 2);
	attribute->pointOut.setY(this->y + this->height);
}

void Process::GetLine(char(*line)) {
	String saveContents(this->contents);
	saveContents.Replace('\n', '\r');

	sprintf(line, "%d\t%d\t%d\t%d\t%d\t\t\t%s\n", 
		ID_PROCESS, this->x, this->y, this->width, this->height, saveContents);
}

bool Process::IsStyle(Long style) {
	bool ret = false;
	if (Shape::IsStyle(style) || Symbol::IsStyle(style) || (style >> 5) % 2) {
		ret = true;
	}
	return ret;
}
/*
void Process::DrawSelectionMarkers(Painter* painter, ScrollController *scrollController) {
	Shape::DrawSelectionMarkers(painter, scrollController);
}
*/