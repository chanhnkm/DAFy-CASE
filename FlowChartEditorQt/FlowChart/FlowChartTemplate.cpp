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
#include "WindowTitle.h"
#include "WindowPin.h"

#include "../GObject/Painter.h"
#include "DrawVisitor.h"
#include "../GObject/QtPainter.h"
#include "../GObject/QtGObjectFactory.h"

#include <qpainter.h>
#include <qevent.h>
#include <qmenubar.h>

FlowChartTemplate::FlowChartTemplate(QWidget *parent)
	: QFrame(parent) {
	this->setMouseTracking(true);
	this->setFocusPolicy(Qt::StrongFocus);

	this->shapeSelected = NULL;
	this->mode = DRAWOFF;
	this->oldShapeSelected = NULL;
	this->windowBorderColor = QColor(153, 204, 204);

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

	this->windowTitle = new WindowTitle(2, 2, 186, 30, QColor(102, 204, 204),
		Qt::SolidLine, QColor(153, 204, 204), String(" ��ȣ ����")); //x, y�� â �׵θ� �β� 5�� Ÿ��Ʋ �β� 1�� ��ð�? �ذ�
	Long windowPinX = this->windowTitle->GetX() + this->windowTitle->GetWidth() - 26 - 3; //24=�簢������,3=��������
	Long windowPinY = this->windowTitle->GetY() + 4;
	this->windowPin = new WindowPin(windowPinX, windowPinY, 26, 23, QColor(102, 204, 204),
		Qt::SolidLine, QColor(255, 255, 255));

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
	if (this->windowTitle != NULL) {
		delete this->windowTitle;
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

	//=======â �׵θ�=========
	GObject *pen = factory.MakePen(QBrush(this->windowBorderColor), 5);
	GObject *oldPen = this->painter->SelectObject(*pen);
	this->painter->Update();

	this->painter->DrawRect(QRect(0, 0, 190, frameRect.height()));

	this->painter->SelectObject(*oldPen);
	this->painter->Update();
	if (pen != 0) {
		delete pen;
	}
	//=======â �׵θ�=========

	FlowChartVisitor *visitor = new DrawVisitor(this->painter);
	this->flowChartTemplate->Accept(visitor);

	//â ���� ����
	this->windowTitle->Accept(visitor);
	//���� ��
	this->windowPin->Accept(visitor);

	//======================
	pen = factory.MakePen(QBrush(QColor(0, 0, 0)), 2);
	oldPen = this->painter->SelectObject(*pen);
	this->painter->Update();


	this->painter->Render(&dc, 0, 0);

	this->painter->SelectObject(*oldPen);
	this->painter->Update();
	if (pen != 0) {
		delete pen;
	}

	if (visitor != 0) {
		delete visitor;
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

	QRect pinRect(this->windowPin->GetX(), this->windowPin->GetY(), this->windowPin->GetWidth(), this->windowPin->GetHeight());
	bool isContain = pinRect.contains(event->pos());
	bool isPinned = dynamic_cast<WindowPin*>(this->windowPin)->GetIsPinned();
	if (isContain == true) {
		if (isPinned == true) {
			dynamic_cast<WindowPin*>(this->windowPin)->SetIsPinned(false);
		}
		else {
			dynamic_cast<WindowPin*>(this->windowPin)->SetIsPinned(true);
		}
	}
	this->repaint();
}

void FlowChartTemplate::mouseMoveEvent(QMouseEvent *event) {
	//��ȣ��
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

	//������ ��
	QRect pinRect(this->windowPin->GetX(), this->windowPin->GetY(), this->windowPin->GetWidth(), this->windowPin->GetHeight());
	bool isContain = pinRect.contains(event->pos());
	if (isContain == true && this->hasFocus()) {
		this->windowPin->Paint(QColor(102, 255, 255), Qt::SolidLine, this->windowPin->GetBorderColor());
	}
	else if (isContain == false && this->hasFocus()) {
		this->windowPin->Paint(QColor(102, 204, 204), Qt::SolidLine, this->windowPin->GetBorderColor());
	}
	else if (isContain == true && !this->hasFocus()) {
		this->windowPin->Paint(QColor(204, 204, 204), Qt::SolidLine, this->windowPin->GetBorderColor());
	}
	else { //isContain == false && !this->hasFocus()
		this->windowPin->Paint(QColor(235, 235, 235), Qt::SolidLine, this->windowPin->GetBorderColor());
	}

	this->repaint();
}

void FlowChartTemplate::focusOutEvent(QFocusEvent *event) {
	bool isPinned = dynamic_cast<WindowPin*>(this->windowPin)->GetIsPinned();
	if (isPinned == true) {
		this->windowTitle->Paint(QColor(235, 235, 235), this->windowTitle->GetBorderLine(), QColor(255, 255, 255));
		this->windowBorderColor = this->windowTitle->GetBorderColor();
		this->windowPin->Paint(QColor(235, 235, 235), Qt::SolidLine, this->windowPin->GetBorderColor());
	}
	else { //���� ������ ���¿��� ��Ŀ�� �ƿ���.
		this->windowTitle->ReSize(this->windowTitle->GetHeight(), this->windowTitle->GetWidth() / 2);
		this->resize(this->windowTitle->GetWidth(), this->windowTitle->GetHeight());

		dynamic_cast<WindowTitle*>(this->windowTitle)->SetIsFocusedAndPinned(false);

		//DrawingPaper
		FlowChartEditor *editor = (FlowChartEditor*)this->parentWidget();
		DrawingPaper *canvas = (DrawingPaper*)editor->windows[0];
		canvas->move(this->x() * 2 + this->windowTitle->GetWidth(), canvas->y());
		canvas->resize(canvas->width() + canvas->x() - (this->x() * 2 + this->width()), canvas->height());
	}
	this->repaint();
}

void FlowChartTemplate::focusInEvent(QFocusEvent *event) {
	dynamic_cast<WindowTitle*>(this->windowTitle)->SetIsFocusedAndPinned(true);
	bool isPinned = dynamic_cast<WindowPin*>(this->windowPin)->GetIsPinned();
	if (isPinned == true) {
		this->windowTitle->Paint(QColor(102, 204, 204), this->windowTitle->GetBorderLine(), QColor(153, 204, 204));
		this->windowBorderColor = this->windowTitle->GetBorderColor();
		this->windowPin->Paint(QColor(102, 204, 204), Qt::SolidLine, this->windowPin->GetBorderColor());
	}
	else {
		FlowChartEditor *editor = (FlowChartEditor*)this->parentWidget();
		this->resize(190, editor->frameRect().height() - editor->menuBar->height() - 20);
		this->windowTitle->ReSize(this->windowTitle->GetHeight() * 2, this->windowTitle->GetWidth());

		//DrawingPaper
		DrawingPaper *canvas = (DrawingPaper*)editor->windows[0];
		canvas->move(canvas->x() + this->width(), canvas->y());
		canvas->resize(canvas->width() - canvas->x() - (this->x() * 2 + this->width()), canvas->height());
	}
	this->repaint();
}