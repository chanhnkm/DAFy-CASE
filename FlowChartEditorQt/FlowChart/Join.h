/******************************************************************
* ���� �̸� : Join.h
* ��� : ���ἱ ��ũ��Ʈ Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 31��
*******************************************************************/

#ifndef _JOIN_H
#define _JOIN_H

#include "Line.h"

class Join : public Line {
public:
	Join(Long x, Long y, Long width, Long height, QColor backGroundColor,
		QPen borderLine, QColor borderColor, String contents = static_cast<String>(""));
	~Join();
	Join(const Join& source);
	Join& operator =(const Join& source);

	virtual bool IsEqual(const Shape& other);
	virtual bool IsNotEqual(const Shape& other);

	virtual bool operator ==(const Shape& other);
	virtual bool operator !=(const Shape& other);

	//virtual void DrawActiveShape(Painter *painter);

	virtual void Accept(FlowChartVisitor *draw);

	virtual Shape* Clone();

	virtual void GetRegion(Painter *painter, QRegion *region);
	virtual void GetRegion(Painter *painter, Long thickness, QRegion *region);
	virtual void GetSelectionMarkerAllRegion(QRegion *region);

	void ReSize(Long width, Long height, Long height2);

	virtual bool IsIncluded(Painter *painter, QPoint point);
	virtual bool IsIncluded(Painter *painter, const QRect& rect);

	Long GetHeight2() const;

	virtual void Copy(Shape *object);
	virtual void GetSelectionMarkerRect(int marker, QRect *rect);
	virtual int GetHitCode(Painter *painter, const QPoint& point, const QRegion& region);

	virtual void GetLine(char(*line));
	virtual bool Identify(SHAPE identify);
	virtual bool IsStyle(Long style);

	virtual Long CenterOfGravityY() const;
	virtual Long CenterOfGravityX() const;

	//virtual void DrawSelectionMarkers(Painter* painter, ScrollController *scrollController);
	void GetAttribute(Attribute *attribute);
private:
	Long height2;
};

inline bool Join::Identify(SHAPE identify) {
	return (identify == LINE || identify == JOIN) ? (true) : (false);
}

inline Long Join::GetHeight2() const {
	return this->height2;
}

inline Long Join::CenterOfGravityY() const {
	return (height >= 0) ? (y + height) : (y);
}

inline Long Join::CenterOfGravityX() const {
	//return ( height >= 0 )?( x ):( x + width);
	return x + width / 2;
}

#endif // _JOIN_H