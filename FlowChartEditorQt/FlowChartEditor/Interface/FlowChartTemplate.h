/******************************************************************
* ���� �̸� : FlowCahrtTemplate.h
* ��� : ������ �ۼ��� ��Ʈ��
* �ۼ��� : ����â
* �ۼ����� : 2015�� 4�� 10��
*******************************************************************/

#ifndef _FLOWCHARTTEMPLATE_H
#define _FLOWCHARTTEMPLATE_H

#include <QtWidgets/QFrame>
#include "../Shapes/Shape.h"
class GObject;

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
	NShape *shapeSelected;
	GObject *painter;
	int mode;
	NShape *flowChartTemplate;
	NShape *oldShapeSelected;
	
	NShape *windowTitle;
	QColor windowBorderColor;
	NShape *windowPin;
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event); //tracking ������ ����Ʈ�� ���콺 �ƹ��ų� ������ ���� ȣ���.
	void mouseReleaseEvent(QMouseEvent *event);
	void focusOutEvent(QFocusEvent *event);
	void focusInEvent(QFocusEvent *event);
	void leaveEvent(QEvent *event);
	void enterEvent(QEvent *event);
	void keyPressEvent(QKeyEvent *event); //DrawingMode, one, two, three, etc.
};

#endif // _FLOWCHARTTEMPLATE_H
