/******************************************************************
* ���� �̸� : Terminal.cpp
* ��� : �ܸ���ȣ�� ��ũ��Ʈ Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 31��
*******************************************************************/

#include "Terminal.h"
#include "../GObject/Painter.h"
#include "FlowChartVisitor.h"
#pragma warning (disable : 4996)

Terminal::Terminal(Long x, Long y, Long width, Long height,
	QColor backGroundColor, Qt::PenStyle borderLine, QColor borderColor, String contents)
	: Symbol(x, y, width, height, backGroundColor, borderLine, borderColor, contents) {
}

Terminal::~Terminal() {

}

Terminal::Terminal(const Terminal& source) : Symbol(source) {

}

Terminal& Terminal::operator =(const Terminal& source) {
	Symbol::operator=(source);

	return *this;
}

bool Terminal::IsEqual(const NShape& other) {
	bool retVo = false;
	if (dynamic_cast<Terminal *>(const_cast<NShape *>(&other))) {
		retVo = NShape::IsEqual(other);
	}
	return retVo;
}

bool Terminal::IsNotEqual(const NShape& other) {
	bool retVo = false;
	if (!dynamic_cast<Terminal *>(const_cast<NShape*>(&other))) {
		retVo = NShape::IsNotEqual(other);
	}
	return retVo;
}

bool Terminal::operator ==(const NShape& other) {
	bool retVo = false;
	if (dynamic_cast<Terminal *>(const_cast<NShape *>(&other))) {
		retVo = NShape::IsEqual(other);
	}
	return retVo;
}

bool Terminal::operator !=(const NShape& other) {
	bool retVo = false;
	if (!dynamic_cast<Terminal *>(const_cast<NShape*>(&other))) {
		retVo = NShape::IsNotEqual(other);
	}
	return retVo;
}

void Terminal::Accept(FlowChartVisitor *draw) {
	draw->Visit(this);
}

NShape* Terminal::Clone() {
	return new Terminal(*this);
}

QRegion Terminal::GetRegion() {
	Long halfHeight = (Long)this->height / 2;

	QRect ellipse;

	ellipse = QRect(this->x, this->y, this->height, this->height);
	QRegion region(ellipse, QRegion::Ellipse);
	
	ellipse = QRect(this->x + this->width - this->height, this->y, this->height, this->height);
	region += QRegion(ellipse, QRegion::Ellipse);
	
	QRect rectangle(this->x + halfHeight, this->y, this->width - this->height, this->height);
	region += QRegion(rectangle, QRegion::Rectangle);
	
	return region;
}


QRegion Terminal::GetRegion(Long thickness) {
	Long x = this->x - thickness;
	Long y = this->y - thickness;
	Long width = this->width + thickness * 2;
	Long height = this->height + thickness * 2;
	Long halfHeight = height / 2;

	QRect ellipse;

	ellipse = QRect(x, y, height, height);
	QRegion region(ellipse, QRegion::Ellipse);
	
	ellipse = QRect(x + width - height, y, height, height);
	region += QRegion(ellipse, QRegion::Ellipse);
	
	QRect rectangle(x + halfHeight, y, width - height, height);
	region += QRegion(rectangle, QRegion::Rectangle);
	
	return region;
}

bool Terminal::IsIncluded(QPoint point) {
	bool ret;
	
	Long halfHeight = (Long)this->height / 2;

	QRect ellipse;
	QRegion addRegion;
	QRegion region;

	ellipse = QRect(this->x, this->y, this->height, this->height);
	addRegion = QRegion(ellipse, QRegion::Ellipse);
	region += addRegion;

	ellipse = QRect(this->x + this->width - this->height, this->y, this->height, this->height);
	addRegion = QRegion(ellipse, QRegion::Ellipse);
	region += addRegion;

	QRect rectangle(this->x + halfHeight, this->y, this->width - this->height, this->height);
	addRegion = QRegion(rectangle, QRegion::Rectangle);
	region += addRegion;

	ret = region.contains(point);

	return ret;
}

bool Terminal::IsIncluded(const QRect& rect) {
	bool ret;

	Long halfHeight = (Long)this->height / 2;

	QRect ellipse;
	QRegion addRegion;
	QRegion region;

	ellipse = QRect(this->x, this->y, this->height, this->height);
	addRegion = QRegion(ellipse, QRegion::Ellipse);
	region += addRegion;

	ellipse = QRect(this->x + this->width - this->height, this->y, this->height, this->height);
	addRegion = QRegion(ellipse, QRegion::Ellipse);
	region += addRegion;

	QRect rectangle(this->x + halfHeight, this->y, this->width - this->height, this->height);
	addRegion = QRegion(rectangle, QRegion::Rectangle);
	region += addRegion;

	ret = region.contains(rect);
 
	return ret;
}

void Terminal::GetAttribute(Attribute *attribute) {
	attribute->vertexIn = 'Y';
	attribute->vertexOut = 'Y';

	attribute->pointIn.setX(this->x + this->width / 2);
	attribute->pointIn.setY(this->y);

	attribute->pointOut.setX(this->x + this->width / 2);
	attribute->pointOut.setY(this->y + this->height);
}

void Terminal::GetLine(char(*line)) {
	String saveContents(this->contents);
	saveContents.Replace('\n', '\r');

	sprintf(line, "%d\t%d\t%d\t%d\t%d\t\t\t%s\n", 
		ID_TERMINAL, this->x, this->y, this->width, this->height, saveContents.GetString());
}

bool Terminal::IsStyle(Long style) {
	bool ret = false;
	if (NShape::IsStyle(style) || Symbol::IsStyle(style) || (style >> 2) % 2) {
		ret = true;
	}
	return ret;
}

void Terminal::DrawSelectionMarkers(GObject *painter, ScrollController *scrollController) {
	NShape::DrawSelectionMarkers(painter, scrollController);
}