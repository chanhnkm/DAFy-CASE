/******************************************************************
* ���� �̸� : Symbol.h
* ��� : ��ȣ�� �߻�ȭ Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 30�� 
*******************************************************************/

#ifndef _SYMBOL_H
#define _SYMBOL_H

#include "Shape.h"

using namespace FlowChartShape;

class Symbol : public Shape {
	public:		
		Symbol(Long x, Long y, Long width, Long height, 
			QColor backGroundColor = QColor(255, 255, 255), Qt::PenStyle borderLine = Qt::SolidLine,
			QColor borderColor = QColor(0, 0, 0), String contents = static_cast<String>(""));

		virtual ~Symbol() = 0;

		Symbol(const Symbol& source);
		Symbol& operator =(const Symbol& source);

		virtual bool IsStyle(Long style);
};

inline bool Symbol::IsStyle(Long style) {
	return (style % 2) ? (true) : (false);
}


#endif// _SYMBOL_H