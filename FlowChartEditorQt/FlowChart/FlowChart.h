/******************************************************************
* ���� �̸� : FlowChart.h
* ��� : ������ ��Ʈ�� Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 31��
*******************************************************************/

#ifndef _FLOWCHART_H
#define _FLOWCHART_H

#include "Block.h"

typedef signed long int Long;

class FlowChartVisitor;

class FlowChart : public Block {
public:
	FlowChart(Long capacity = 30);
	FlowChart(const FlowChart& source);
	virtual ~FlowChart();
	FlowChart& operator=(const FlowChart& source);


	virtual void DrawActiveShape(Painter *painter);
	virtual void Accept(FlowChartVisitor *draw);

	virtual Shape* Clone();

	virtual void GetRegion(QRegion *region) {};
	virtual void GetRegion(Long thickness, QRegion *region) {};

	QRect GetRange();
};

//Long CompareShapeAddress(void *one, void *other);
//Long CompareCoordinateForFlowChart(void *one, void *other);

#endif //_FLOWCHART_H
