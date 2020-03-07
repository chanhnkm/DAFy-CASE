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
	LONG mode;
	FlowChartShape::Shape *flowChartTemplate;
	FlowChartShape::Shape *oldShapeSelected;
protected:
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnDestroy();
	//afx_msg void OnPaint();
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point); // ���콺 ���� ��ư �ٿ�
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//DECLARE_MESSAGE_MAP()          //��32���� ON_COMMAND ON_NOTIFY �������� ����
};

#endif // _FLOWCHARTTEMPLATE_H
