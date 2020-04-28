/******************************************************************
* ���� �̸� : Arrow.h
* ��� : ���ἱ ��ũ��Ʈ Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 31��
*******************************************************************/

#ifndef _ARROW_H
#define _ARROW_H

#include "Line.h"

class Arrow : public Line {
public:
	Arrow(float x, float y, float width, float height,
		QColor backGroundColor = QColor(255, 255, 255), Qt::PenStyle borderLine = Qt::SolidLine,
		QColor borderColor = QColor(0, 0, 0), String contents = static_cast<String>(""));
	~Arrow();
	Arrow(const Arrow& source);
	Arrow& operator =(const Arrow& source);

	virtual bool IsEqual(const NShape& other);
	virtual bool IsNotEqual(const NShape& other);

	virtual bool operator ==(const NShape& other);
	virtual bool operator !=(const NShape& other);

	virtual void DrawActiveShape(GObject *painter);

	virtual void Accept(FlowChartVisitor *draw);

	virtual NShape* Clone();

	virtual QRegion GetRegion();
	virtual QRegion GetRegion(Long thickness);
	virtual QRegion GetSelectionMarkerAllRegion();

	virtual bool IsIncluded(QPointF point);
	virtual bool IsIncluded(const QRectF& rect);

	virtual int GetHitCode(const QPointF& point, const QRegion& region);

	virtual void GetLine(char(*line));

	virtual bool Identify(SHAPE identify);

	virtual void DrawSelectionMarkers(GObject *painter, ScrollController *scrollController);
	virtual void GetSelectionMarkerRect(int marker, QRectF *rect);

	void GetAttribute(Attribute *attribute);
	virtual bool IsStyle(Long style);
};

inline bool Arrow::Identify(SHAPE identify) {
	return (identify == LINE || identify == ARROW) ? (true) : (false);
}

#endif // _ARROW_H