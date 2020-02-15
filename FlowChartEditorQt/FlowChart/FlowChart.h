/******************************************************************
* ���� �̸� : FlowChart.h
* ��� : ������ ��Ʈ�� Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 31��
*******************************************************************/

#ifndef _FLOWCHART_H
#define _FLOWCHART_H

#include "Shape.h"
#include "Array.h"

typedef signed long int Long;

class DrawVisitor;

class FlowChart : public Shape {
public:
	FlowChart(Long capacity = 100);
	~FlowChart();

	FlowChart(const FlowChart& source);

	Long Attach(Shape *shape);

	Long Insert(Long index, Shape *shape);
	Long InsertRear(Long index, Shape *shape);

	Long Find(Shape *shape); // Make �Լ����� ���
	//Long Find(CDC *dc, CPoint point);
	//Long Find(CDC *dc, int x, int y);
	Long Find(Painter *painter, QPoint point);
	Long Find(Painter *painter, int x, int y);

	// 20160623 SequnceMake ���鶧 �ʹ� ã�°� ���Ƽ� ����
	Long Find(Painter *painter, QRect& rect);
	Long Find(SHAPE identify);
	Long Find(Long fromIndex, SHAPE identify);
	void Find(Long styles, Long *(*indexes), Long *count);

	Long Erase(Long index);
	Long Erase(SHAPE identify);
	bool EraseSelectedAll();

	// �Լ��̸� Change ���� �ٲ� ��!! 20160617
	void Swap(Long toIndex, Long fromIndex);
	void Swap(Long toIndex, Shape *shape);

	void Clear();

	FlowChart& operator =(const FlowChart& source);

	Shape& operator[](Long index);
	Shape* GetAt(Long index);
	Long GetCapacity() const;
	Long GetLength() const;
	Long GetCurrent() const;

	//virtual void DrawActiveShape(CDC *dc);
	virtual void DrawActiveShape(Painter *painter);
	virtual void Accept(FlowChartVisitor *draw);

	virtual Shape* Clone();

	///////////////////////////////////////////////////////////
	// ���� ����
	void SelectAll();
	void UnSelectAll();
	void GetSelecteds(Long *(*indexes), Long *count);
	Long CountSelecteds();

	virtual void GetRegion(Painter *painter, QRegion *region) {};
	virtual void GetRegion(Painter *painter, Long thickness, QRegion *region) {};

	// ���� ����
	void AscendingSort();
	void DescendingSort();

	void GetRange(QRect *rect);
private:
	Array<Shape *> shapes;
	Long capacity;
	Long length;
	Long current;
};

Long CompareShapeAddress(void *one, void *other);
Long CompareCoordinateForFlowChart(void *one, void *other);

inline Long FlowChart::GetCapacity() const {
	return this->capacity;
}

inline Long FlowChart::GetLength() const {
	return this->length;
}

inline Long FlowChart::GetCurrent() const {
	return this->current;
}

#endif //_FLOWCHART_H
