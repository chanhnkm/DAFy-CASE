/******************************************************************
* ���� �̸� : Line.cpp
* ��� : ���� �߻�ȭ Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 30��
*******************************************************************/

#include "Line.h"

Line::Line(float x, float y, float width, float height, QColor backGroundColor,
	Qt::PenStyle borderLine, QColor borderColor, String contents)
	: NShape(x, y, width, height, backGroundColor, borderLine, borderColor, contents) {
	this->arrowSize = 20;
}

Line::~Line() {
}

Line::Line(const Line& source) :NShape(source) {
	this->arrowSize = source.arrowSize;
}

Line& Line::operator =(const Line& source) {
	NShape::operator=(source);
	this->arrowSize = source.arrowSize;
	return *this;
}

void Line::ChangeArrowSize(float arrowSize) {
	this->arrowSize = arrowSize;
}