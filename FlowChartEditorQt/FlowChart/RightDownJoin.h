/******************************************************************
* ���� �̸� : RightDownJoin.h
* ��� : ���ἱ ��ũ��Ʈ Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 31��
*******************************************************************/

#ifndef _RIGHTDOWNJOIN_H
#define _RIGHTDOWNJOIN_H

#include "Line.h"

class RightDownJoin : public Line {
public:
	RightDownJoin(Long x, Long y, Long width, Long height, Long width2 = RIGHTDOWNJOIN_WIDTH, Long height2 = RIGHTDOWNJOIN_HEIGHT,
		QColor backGroundColor = QColor(255, 255, 255), Qt::PenStyle borderLine = Qt::SolidLine,
		QColor borderColor = QColor(0, 0, 0), String contents = static_cast<String>(""));
	~RightDownJoin();
	RightDownJoin(const RightDownJoin& source);
	RightDownJoin& operator =(const RightDownJoin& source);

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

	void ReSize(Long width, Long height, Long width2, Long height2);

	virtual bool IsIncluded(QPoint point);
	virtual bool IsIncluded(const QRect& rect);

	virtual int GetHitCode(const QPoint& point, const QRegion& region);

	virtual void Copy(NShape *object);

	Long GetWidth2() const;
	Long GetHeight2() const;

	virtual void GetLine(char(*line));
	virtual bool Identify(SHAPE identify);
	virtual bool IsStyle(Long style);

	virtual Long CenterOfGravityY() const;
	virtual Long CenterOfGravityX() const;

	virtual void DrawSelectionMarkers(GObject *painter, ScrollController *scrollController);

	virtual void GetSelectionMarkerRect(int marker, QRect *rect);
	void GetAttribute(Attribute *attribute);

private:
	Long width2;
	Long height2;
};

inline bool RightDownJoin::Identify(SHAPE identify) {
	return (identify == LINE || identify == RIGHTDOWNJOIN) ? (true) : (false);
}

inline Long RightDownJoin::GetWidth2() const {
	return this->width2;
}

inline Long RightDownJoin::GetHeight2() const {
	return this->height2;
}

inline Long RightDownJoin::CenterOfGravityY() const {
	return this->y + this->height;
}

inline Long RightDownJoin::CenterOfGravityX() const {
	return this->x + this->width;
}
#endif// _RIGHTDOWNJOIN_H