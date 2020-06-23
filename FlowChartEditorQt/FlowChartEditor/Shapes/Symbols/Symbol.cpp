/******************************************************************
* ���� �̸� : Symbol.cpp
* ��� : ��ȣ�� �߻�ȭ Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 30��
*******************************************************************/

#include "Symbol.h"

Symbol::Symbol(float x, float y, float width, float height, QColor backGroundColor,
	Qt::PenStyle borderLine, QColor borderColor, String contents)
	: NShape(x, y, width, height, backGroundColor, borderLine, borderColor, contents) {

}

Symbol::~Symbol() {

}

Symbol::Symbol(const Symbol& source)
	:NShape(source) {

}

Symbol& Symbol::operator =(const Symbol& source) {
	NShape::operator=(source);

	return *this;
}