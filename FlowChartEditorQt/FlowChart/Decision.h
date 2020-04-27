/******************************************************************
* ���� �̸� : Decision.h
* ��� : �Ǵܱ�ȣ�� ��ũ��Ʈ Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 30��
*******************************************************************/

#ifndef _DECISION_H
#define _DECISION_H

#include "Symbol.h"

class Decision : public Symbol {
public:
	Decision(Long x, Long y, Long width, Long height,
		QColor backGroundColor = QColor(255, 255, 255), Qt::PenStyle borderLine = Qt::SolidLine,
		QColor borderColor = QColor(0, 0, 0), String contents = static_cast<String>(""));
	~Decision();
	Decision(const Decision& source);
	Decision& operator =(const Decision& source);

	virtual bool IsEqual(const NShape& other);
	virtual bool IsNotEqual(const NShape& other);

	virtual bool operator ==(const NShape& other);
	virtual bool operator !=(const NShape& other);

	virtual void Accept(FlowChartVisitor *draw);

	virtual NShape* Clone();

	virtual QRegion GetRegion();
	virtual QRegion GetRegion(Long thickness);

	virtual bool IsIncluded(QPoint point);
	virtual bool IsIncluded(const QRect& rect);

	void GetAttribute(Attribute *attribute);

	virtual void GetLine(char(*line));

	virtual bool Identify(SHAPE identify);
	virtual bool IsStyle(Long style);

	virtual void DrawSelectionMarkers(GObject *painter, ScrollController *scrollController);
};

inline bool Decision::Identify(SHAPE identify) {
	return (identify == DECISION || identify == SYMBOL) ? (true) : (false);
}

#endif// _DECISION_H