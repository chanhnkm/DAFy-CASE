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
class GObject;

class FlowChart : public Block {
public:
	FlowChart(Long capacity = 30);
	FlowChart(const FlowChart& source);
	virtual ~FlowChart();
	FlowChart& operator=(const FlowChart& source);

	virtual void DrawActiveShape(GObject *painter);
	virtual void Accept(FlowChartVisitor *draw);

	virtual NShape* Clone();

	virtual QRegion GetRegion() { QRegion region; return region; };
	virtual QRegion GetRegion(Long thickness) { QRegion region; return region; };

	QRect GetRange();
};

//Long CompareShapeAddress(void *one, void *other);
//Long CompareCoordinateForFlowChart(void *one, void *other);

#endif //_FLOWCHART_H
