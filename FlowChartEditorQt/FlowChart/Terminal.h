/******************************************************************
* ���� �̸� : Terminal.h
* ��� : �ܸ���ȣ�� ��ũ��Ʈ Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 31��
*******************************************************************/

#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "Symbol.h"

class Terminal : public Symbol {
public:
	Terminal(float x, float y, float width, float height,
		QColor backGroundColor = QColor(255, 255, 255), Qt::PenStyle borderLine = Qt::SolidLine,
		QColor borderColor = QColor(0, 0, 0), String contents = static_cast<String>(""));
	~Terminal();

	Terminal(const Terminal& source);
	Terminal& operator =(const Terminal& source);

	virtual bool IsEqual(const NShape& other);
	virtual bool IsNotEqual(const NShape& other);

	virtual bool operator ==(const NShape& other);
	virtual bool operator !=(const NShape& other);

	virtual void Accept(FlowChartVisitor *draw);

	virtual NShape* Clone();

	virtual QRegion GetRegion();
	virtual QRegion GetRegion(Long thickness);

	virtual bool IsIncluded(QPointF point);
	virtual bool IsIncluded(const QRectF& rect);

	void GetAttribute(Attribute *attribute);

	virtual void GetLine(char(*line));
	virtual bool Identify(SHAPE identify);
	virtual bool IsStyle(Long style);

	virtual void DrawSelectionMarkers(GObject *painter, ScrollController *scrollController);
};

inline bool Terminal::Identify(SHAPE identify) {
	return (identify == SYMBOL || identify == TERMINAL) ? (true) : (false);
}

#endif //_THERMINAL_H