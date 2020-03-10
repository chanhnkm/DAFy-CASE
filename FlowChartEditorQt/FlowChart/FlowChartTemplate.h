/******************************************************************
* ���� �̸� : FlowCahrtTemplate.h
* ��� : ������ �ۼ��� ��Ʈ��
* �ۼ��� : ����â
* �ۼ����� : 2015�� 4�� 10��
*******************************************************************/

#ifndef _FLOWCHARTTEMPLATE_H
#define _FLOWCHARTTEMPLATE_H

#include <QtWidgets/QFrame>
#include "Shape.h"
class Painter;

typedef signed long int Long;

class FlowChartTemplate : public QFrame
{
	Q_OBJECT

public:
	enum { DRAWON, DRAWOFF };
public:
	FlowChartTemplate(QWidget *parent = Q_NULLPTR);
	~FlowChartTemplate();
public:
	FlowChartShape::Shape *shapeSelected;
	Painter *painter;
	int mode;
	FlowChartShape::Shape *flowChartTemplate;
	FlowChartShape::Shape *oldShapeSelected;
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event); //tracking ������ ����Ʈ�� ���콺 �ƹ��ų� ������ ���� ȣ���.
};

#endif // _FLOWCHARTTEMPLATE_H
