/******************************************************************
 * ���� �̸� : FlowCahrtTemplate.cpp
 * ��� : ������ �ۼ��� ��Ʈ��
 * �ۼ��� : ����â
 * �ۼ����� : 2015�� 4�� 10��
*******************************************************************/

#include "FlowChartTemplate.h"
#include "Template.h"
#include "Terminal.h"
#include "Shape.h"
//////////////////////////////////////////////////////////////
#include "../FlowChartEditor.h"
#include "DrawingPaper.h"

#include "Array.h"

#include "Symbol.h"
#include "Line.h"

#include "Decision.h"
#include "Preparation.h"
#include "Process.h"
#include "Terminal.h"
#include "InputOutput.h"

#include "Arrow.h"
#include "Join.h"
#include "LeftDown.h"
#include "RepeatFalse.h"
#include "RepeatTrue.h"
#include "RightDown.h"
#include "RightDownJoin.h"

#include "../GObject/Painter.h"
#include "DrawVisitor.h"
#include "../GObject/QtPainter.h"

//#include "StatusBar.h"
//#include "ToolTip.h"
//#include "TutorialForm.h"
//#include "Tutorials.h"
//#include "TutorialController.h"

#include "../GObject/QtGObjectFactory.h"

#include <qpainter.h>
#include <qevent.h>

FlowChartTemplate::FlowChartTemplate(QWidget *parent)
	: QFrame(parent) {
	this->setMouseTracking(true);

	this->shapeSelected = NULL;
	this->mode = DRAWOFF;
	this->oldShapeSelected = NULL;

	QRect rect = this->frameRect();
	Long width = 150;
	Long height = 50;
	Long x = (190 - width) / 2;
	Long y = 50;

	this->flowChartTemplate = new Template;
	NShape *template1 = new Terminal(x, y, width, height, QColor(255, 153, 153),
		Qt::SolidLine, QColor(0, 0, 0), String("START"));
	y += 70;
	NShape *template2 = new Preparation(x, y, width, height, QColor(153, 153, 255),
		Qt::SolidLine, QColor(0, 0, 0), String("Preparation"));
	y += 70;
	NShape *template3 = new InputOutput(x, y, width, height, QColor(255, 255, 153),
		Qt::SolidLine, QColor(0, 0, 0), String("READ "));
	y += 70;
	NShape *template4 = new Process(x, y, width, height, QColor(153, 255, 153),
		Qt::SolidLine, QColor(0, 0, 0), String("Process"));
	y += 70;
	NShape *template5 = new Decision(x, y, width, height, QColor(255, 153, 255),
		Qt::SolidLine, QColor(0, 0, 0), String("Decision"));
	y += 70;
	NShape *template6 = new InputOutput(x, y, width, height, QColor(255, 255, 153),
		Qt::SolidLine, QColor(0, 0, 0), String("PRINT "));
	y += 70;
	NShape *template7 = new Terminal(x, y, width, height, QColor(255, 153, 153),
		Qt::SolidLine, QColor(0, 0, 0), String("STOP"));

	this->flowChartTemplate->Attach(template1);
	this->flowChartTemplate->Attach(template2);
	this->flowChartTemplate->Attach(template3);
	this->flowChartTemplate->Attach(template4);
	this->flowChartTemplate->Attach(template5);
	this->flowChartTemplate->Attach(template6);
	this->flowChartTemplate->Attach(template7);

	this->painter = new QtPainter(rect.width(), rect.height());

	DrawingPaper *canvas = static_cast<DrawingPaper*>(static_cast<FlowChartEditor*>(this->parentWidget())->windows[0]);
	GObject *font = canvas->painter->CurrentObject("Font");
	this->painter->SelectObject(*font->Clone());
	this->painter->Update();
}

FlowChartTemplate::~FlowChartTemplate() {
	if (this->flowChartTemplate != NULL) {
		delete this->flowChartTemplate;
	}
	if (this->painter != NULL) {
		delete this->painter;
	}
}

void FlowChartTemplate::paintEvent(QPaintEvent *event) {
	QPainter dc(this);

	int oldMode = this->painter->GetBackgroundMode();
	this->painter->SetBackgroundMode(Qt::TransparentMode);
	this->painter->SetCompositionMode(QPainter::RasterOp_NotSourceXorDestination);

	QtGObjectFactory factory;

	QRect frameRect = this->frameRect();

	this->painter->Resize(frameRect.width(), frameRect.height()); // canvas size ����

	//FlowChartEditor *editor = (FlowChartEditor*)this->parentWidget();
	//=======â �׵θ�=========
	GObject *pen = factory.MakePen(QBrush(QColor(153, 204, 204)), 5);
	GObject *oldPen = this->painter->SelectObject(*pen);
	this->painter->Update();

	this->painter->DrawRect(QRect(0,0,190,570));
	
	//=======â �׵θ�=========

	//=======â ����========
	QRect titleRect;
	//������ �𸣰����� setCoords()�� ������ width�� height�� -1�� �Ѵ�.
	titleRect.setCoords(2, 2, frameRect.width() - 3, 30 - 3); //5 = �׵θ� �β�
	dynamic_cast<QPen*>(pen)->setWidth(1);
	GObject *brush = factory.MakeBrush(QColor(102,204,204));
	GObject *oldBrush = this->painter->SelectObject(*brush);
	this->painter->Update();

	this->painter->DrawRect(titleRect);

	this->painter->SelectObject(*oldBrush);
	this->painter->SelectObject(*oldPen);
	this->painter->Update();
	if (brush != 0) {
		delete brush;
	}
	if (pen != 0) {
		delete pen;
	}
	
	GObject *oldFont = this->painter->CurrentObject("Font");
	GObject *font = factory.MakeFont(oldFont->GetFamily(), 5, oldFont->GetWeight(), oldFont->GetItalic());
	pen = factory.MakePen(QBrush(QColor(102, 255, 255)), 2);
	oldPen = this->painter->SelectObject(*pen);
	this->painter->Update();

	this->painter->DrawTextQ(titleRect, Qt::AlignLeft | Qt::AlignVCenter, QString::fromLocal8Bit(" ��ȣ ����"));

	this->painter->SelectObject(*oldPen);
	this->painter->SelectObject(*oldFont);
	this->painter->Update();
	if (font != 0) {
		delete font;
	}
	if (pen != 0) {
		delete pen;
	}
	//=======â ����========

	//======================
	pen = factory.MakePen(QBrush(QColor(0, 0, 0)), 2);
	oldPen = this->painter->SelectObject(*pen);
	this->painter->Update();

	FlowChartVisitor *visitor = new DrawVisitor(this->painter);
	this->flowChartTemplate->Accept(visitor);
	this->painter->Render(&dc, 0, 0);

	this->painter->SelectObject(*oldPen);
	this->painter->Update();
	if (pen != 0) {
		delete pen;
	}

	/*
	bool ret;
	if (editor->toolTip != NULL) {
		this->ModifyStyle(0, WS_CLIPSIBLINGS);
		ret = editor->toolTip->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
	*/
}

void FlowChartTemplate::mousePressEvent(QMouseEvent *event) {
	FlowChartEditor* editor;

	editor = static_cast<FlowChartEditor*>(this->parentWidget());

	Long index = -1;

	index = this->flowChartTemplate->Find(event->pos());
	if (index != -1) {

		this->shapeSelected = this->flowChartTemplate->GetAt(index);

		static_cast<DrawingPaper*>(editor->windows[0])->mode = DrawingPaper::DRAWING;
		/*
			editor->statusBar->Modify(1, String("DRAWING"));
			String style;
			switch (this->shapeSelected->GetSymbolID()) {
			case ID_TERMINAL:
				style = "    �ܸ� ��ȣ"; break;
			case ID_PREPARATION:
				style = "    �غ� ��ȣ"; break;
			case ID_INPUTOUTPUT:
				style = "    ����� ��ȣ"; break;
			case ID_PROCESS:
				style = "    ó�� ��ȣ"; break;
			case ID_DECISION:
				style = "    �Ǵ� ��ȣ"; break;
			}
			editor->statusBar->Modify(0, style);
			editor->statusBar->Print();
		*/
	}
	else {
		this->shapeSelected = NULL;
	}
}

void FlowChartTemplate::mouseMoveEvent(QMouseEvent *event) {
	Long index = -1;

	QColor selectedColor(235, 235, 235);
	NShape *shape;
	Long i = 0;
	while (i < this->flowChartTemplate->GetLength()) {
		shape = this->flowChartTemplate->GetAt(i);
		if (shape->GetBackGroundColor() == selectedColor && this->oldShapeSelected != NULL) {
			shape->Paint(this->oldShapeSelected->GetBackGroundColor(), shape->GetBorderLine(), shape->GetBorderColor());
		}
		i++;
	}

	index = this->flowChartTemplate->Find(event->pos());

	if (index != -1) {
		shape = this->flowChartTemplate->GetAt(index);
		this->oldShapeSelected = shape->Clone();
		shape->Paint(selectedColor, shape->GetBorderLine(), shape->GetBorderColor());
	}

	this->repaint();
}