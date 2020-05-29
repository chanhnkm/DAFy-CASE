#include "DrawingPaper.h"
#include "../FlowChartEditor.h"
#include "FlowChart.h"
#include "Clipboard.h"
#include "Zoom.h"
#include "A4Paper.h"
#include "../GObject/QtPainter.h"
#include "DrawingTool.h"
#include "MovingTool.h"
#include "ResizingTool.h"
#include "SelectingTool.h"
#include "DrawVisitor.h"
#include "ZoomVisitor.h"
#include "Label.h"
#include "Preparation.h"
#include "ToolFactory.h"
#include "../Notepad/Note.h"
#include "FlowChartTemplate.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "../GObject/QtGObjectFactory.h"
#include "../GObject/GObject.h"
#include "FlowChartKeyActionFactory.h"
#include "FlowChartKeyActions.h"
#include "CoordinateConverter.h"
#include "Decision.h"
#include "File.h"
#include "WindowTitle.h"
#include "VariableList.h"
#include "Line.h"
#include "HistoryController.h"
#include "Registrar.h"
#include "HistoryBook.h"
#include "SheetManager.h"
#include "ContentsAnalyzer.h"

#include <qscrollbar.h>
#include <qpainter.h>
#include <qevent.h>
#include <qmenu.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qstatusbar.h>

DrawingPaper::DrawingPaper(QWidget *parent)
	: QFrame(parent) {

	this->setFocusPolicy(Qt::WheelFocus);

	this->setContextMenuPolicy(Qt::CustomContextMenu);

	this->windowBorderColor = QColor(102, 204, 204);

	this->templateSelected = NULL;

	this->flowChart = NULL;

	this->label = NULL;
	this->painter = NULL;

	this->startX = 0.0F;
	this->startY = 0.0F;

	this->currentX = 0.0F;
	this->currentY = 0.0F;

	this->tool = NULL;

	this->mode = IDLE;
	this->indexOfSelected = -1;

	this->hitCode = -1;

	this->clipboard = NULL;

	this->scrollController = NULL;

	this->historyController = NULL;

	this->registrar = NULL;

	this->zoom = NULL;

	this->popup = NULL;

	this->drawSelectingAreaFlag = false;

	QRect frameRect = this->frameRect();

	this->flowChart = new FlowChart;

	this->painter = new QtPainter(frameRect.width(), frameRect.height()); //Ʈ���� ���۸� ��������.

	this->clipboard = new Clipboard;

	this->historyController = new HistoryController(this);

	this->registrar = new Registrar;

	this->zoom = new Zoom(100);

	this->a4Paper = new A4Paper(444, 615, 1653, 2338);
	//this->zoom->Set(40);

	this->variableList = new VariableList;

	connect(this, &QWidget::customContextMenuRequested, this, &DrawingPaper::OnContextMenu);

	this->setFocus();
}

DrawingPaper::~DrawingPaper() {
	//flowChart, registrar, variableList ������ ��ü�� Sheet�̴�. ���� �Ҹ��Ű�� �ʴ´�.
	if (this->painter != NULL) {
		delete this->painter;
		this->painter = NULL;
	}

	if (this->label != NULL) {
		this->label->Destroy();
	}

	if (DrawingTool::Instance() != NULL) {
		DrawingTool::Destroy();
	}
	if (MovingTool::Instance() != NULL) {
		MovingTool::Destroy();
	}
	if (ResizingTool::Instance() != NULL) {
		ResizingTool::Destroy();
	}
	if (SelectingTool::Instance() != NULL) {
		SelectingTool::Destroy();
	}

	if (this->clipboard != NULL) {
		delete this->clipboard;
	}

	if (this->templateSelected != NULL) {
		delete this->templateSelected;
	}

	if (this->scrollController != NULL) {
		delete this->scrollController;
	}

	if (this->historyController != NULL) {
		delete this->historyController;
	}

	if (this->zoom != NULL) {
		delete this->zoom;
	}

	if (this->a4Paper != NULL) {
		delete this->a4Paper;
	}
	if (this->popup != NULL) {
		delete this->popup;
		this->popup = NULL;
	}
}

void DrawingPaper::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	QRect rect = this->frameRect();

	FlowChartEditor *editor = (FlowChartEditor*)this->parent();

	this->painter->Resize(rect.width(), rect.height(), QColor(250, 250, 250));

	//��Ʈ
	GObject *font = this->painter->CurrentObject("Font")->Clone();
	int size = font->GetPointSize() * this->zoom->GetRate() / 100;
	dynamic_cast<QFont *>(font)->setPointSize(size);
	GObject *oldFont = this->painter->SelectObject(*font);
	this->painter->Update();

	//Visitor ���� ����	
	FlowChartVisitor *drawVisitor = new DrawVisitor(this->painter, this->scrollController);
	FlowChartVisitor *zoomVisitor = new ZoomVisitor(this->zoom);
	NShape *cloneA4 = this->a4Paper->Clone();
	cloneA4->Accept(zoomVisitor);
	cloneA4->Accept(drawVisitor);

	NShape *cloneFlowChart = this->flowChart->Clone();
	cloneFlowChart->Accept(zoomVisitor);
	cloneFlowChart->Accept(drawVisitor);

	if (this->templateSelected != NULL && this->templateSelected->IsSelected())
	{
		NShape *cloneSelected = this->templateSelected->Clone();
		cloneSelected->Accept(zoomVisitor);
		cloneSelected->Accept(drawVisitor);
		if (cloneSelected != 0) {
			delete cloneSelected;
		}
	}

	//��Ʈ ����
	this->painter->SelectObject(*oldFont);
	this->painter->Update();
	if (font != 0) {
		delete font;
	}

	if (this->drawSelectingAreaFlag == true) {
		this->DrawSelectingArea();
	}

	this->painter->Render(&painter, 0, 0);

	if (cloneFlowChart != NULL) {
		delete cloneFlowChart;
		cloneFlowChart = NULL;
	}

	if (cloneA4 != NULL) {
		delete cloneA4;
	}

	if (drawVisitor != NULL) {
		delete drawVisitor;
	}
	if (zoomVisitor != NULL) {
		delete zoomVisitor;
	}
}

void DrawingPaper::mousePressEvent(QMouseEvent *event) {
	//this->setFocus();

	QPointF point = event->localPos();
	this->tool = ToolFactory::Create(this, point);

	FlowChartEditor *editor = static_cast<FlowChartEditor*>(this->parentWidget());
	QString mode = this->GetCurrentMode();
	editor->modeStatus->setText(mode);
	editor->statusBar->repaint();

	if (this->tool != NULL) {
		this->tool->OnLButtonDown(this, point);
		//this->SetCapture();
	}

	this->setMouseTracking(false);
}

void DrawingPaper::mouseMoveEvent(QMouseEvent *event) {
	FlowChartEditor *editor = static_cast<FlowChartEditor*>(this->parentWidget());
	editor->windowClose->Paint(QColor(102, 204, 204), Qt::SolidLine, editor->windowClose->GetBorderColor());
	editor->repaint();

	QPointF point = event->localPos();
	if (this->hasMouseTracking() == false) { //���콺�� �ϰų� ������ ��
		if (this->tool != NULL) {
			this->tool->OnMouseMove(this, point);
		}
	}
	else { //���콺�� �ϰ͵� �ȴ����� �׳� �ִ� ������ ��
		//OnSetCursor �κ�
		QCursor cursor = this->GetCursor(event->pos());
		this->setCursor(cursor);

		QString x = "X: ";
		QString xPoint = QString::number(point.x());
		x += xPoint;
		editor->xStatus->setText(x);
		QString y = "Y: ";
		QString yPoint = QString::number(point.y());
		y += yPoint;
		editor->yStatus->setText(y);
		editor->statusBar->repaint();

	}
}

void DrawingPaper::mouseReleaseEvent(QMouseEvent *event) {

	QPointF point = event->localPos();


	if (this->tool != NULL && this->label==NULL) {
		FlowChartEditor *editor = static_cast<FlowChartEditor*>(this->parentWidget());
		//��ȣ�� �׸��� ���̸�
		if (this->variableList != NULL && dynamic_cast<DrawingTool*>(this->tool)) {
			//�غ��ȣ�� ã�´�.
			Long index = this->flowChart->Find(SHAPE::PREPARATION);
			
			//�غ��ȣ�� ������ �غ��ȣ�� �׸� �� ���� �غ��ȣ�� ������ �غ��ȣ�� �׸� ���� �ִ�.
			//������ �ܸ� ��ȣ�� �����̴�. �غ� ��ȣ�� �ְų� ���ų� ��� ����.
			//�غ��ȣ�� ã�Ұ� �غ� ��ȣ�� �׸��� ���� �ƴϰų�(�غ� ��ȣ�� �ϳ���)
			//�غ��ȣ�� ��ã�Ұ� �غ��ȣ�� �׸����� ���̰ų�
			//�ܸ� ��ȣ�� �׸����� ���̸�
			if ((index != -1 && this->templateSelected->GetSymbolID() != ID_PREPARATION) ||
				(index == -1 && this->templateSelected->GetSymbolID() == ID_PREPARATION) ||
				this->templateSelected->GetSymbolID() == ID_TERMINAL) {

				this->tool->OnLButtonUp(this, point);
				ReleaseCapture();

				QString mode = this->GetCurrentMode();
				editor->modeStatus->setText(mode);
				editor->statusBar->repaint();
			}
			//�غ� ��ȣ�� ��ã�Ұų� �غ��ȣ�� �׸����� ���̰�
			//�غ��ȣ�� ã�Ұų� �غ��ȣ�� �׸����� ���� �ƴϰ�
			//�ܸ� ��ȣ�� �׸����� ���� �ƴϸ�
			else {
				QString message;
				if (this->templateSelected->GetSymbolID() != ID_PREPARATION) {
					message = QString::fromLocal8Bit("    �غ� ��ȣ�� �׸��ʽÿ�.");
				}
				else {
					message = QString::fromLocal8Bit("    �غ� ��ȣ�� �ϳ��� �׸� �� �ֽ��ϴ�.");
				}
				editor->messageStatus->setText(message);
				editor->statusBar->repaint();
				if (this->templateSelected != NULL) {
					delete this->templateSelected;
					this->templateSelected = NULL;
				}
				this->mode = IDLE;
			}
		}
		else {
			this->tool->OnLButtonUp(this, point);
			ReleaseCapture();

			QString mode = this->GetCurrentMode();
			editor->modeStatus->setText(mode);
			editor->statusBar->repaint();
		}
	}
	this->Notify();

	this->setMouseTracking(true);
}

void DrawingPaper::mouseDoubleClickEvent(QMouseEvent *event) {
	if (this->label == NULL) {

		// ���� ���� : �ؽ�Ʈ ������ Manipulator
		NShape *shape;
		float left, top, right, bottom, halfHeight;

		QRectF rect = this->frameRect();
		QPointF point = event->localPos();
		Long positionX = 0;
		Long positionY = 0;
		if (this->scrollController != NULL) {
			positionX = this->scrollController->GetScroll(1)->value();
			positionY = this->scrollController->GetScroll(0)->value();
		}
		point.setX(point.x() + positionX);
		point.setY(point.y() + positionY);
		NShape *holdA4Paper = this->a4Paper->Clone();
		NShape *holdFlowChart = this->flowChart->Clone();
		FlowChartVisitor *zoomVisitor = new ZoomVisitor(this->zoom);
		holdA4Paper->Accept(zoomVisitor);
		holdFlowChart->Accept(zoomVisitor);

		this->indexOfSelected = holdFlowChart->Find(point);

		shape = holdFlowChart->GetAt(this->indexOfSelected);

		if (this->indexOfSelected != -1 &&
			(shape->GetSymbolID() != ID_TERMINAL && !dynamic_cast<Line*>(shape)) &&
			((point.x() > shape->GetX() + 5 && point.x() < shape->GetX() + shape->GetWidth() - 5) ||
				(point.y() > shape->GetY() + 5 && point.y() < shape->GetY() + shape->GetHeight() - 5))) {

			this->clearFocus();

			QColor color = shape->GetBackGroundColor();
			String contents = shape->GetContents();
			
			ContentsAnalyzer analyzer;
			contents = analyzer.RollBackOperators(contents);
			this->label = Label::Instance(&contents, color, this);

			halfHeight = shape->GetHeight() / 2;
			left = shape->GetX() + halfHeight - positionX;
			top = shape->GetY() + 1 - positionY;
			right = shape->GetX() + shape->GetWidth() - halfHeight + 5 - positionX;
			bottom = shape->GetY() + shape->GetHeight() - 1 - positionY;

			this->label->Open(left, top, right - left, bottom - top);
			this->label->show();

			this->Notify();

			shape = this->flowChart->GetAt(this->indexOfSelected);
			shape->Rewrite(String(""));
			this->label->setFocus();
		}
	}
}

void DrawingPaper::resizeEvent(QResizeEvent *event) {
	//========================ĵ���� Ÿ��Ʋ ��========================
	FlowChartEditor *editor = static_cast<FlowChartEditor*>(this->parentWidget());
	editor->sheetManager->ModifyTitles();
	
	NShape *currentTitle = editor->sheetManager->GetTitle(editor->sheetManager->GetBinderCurrent());
	float windowCloseX = currentTitle->GetX() + currentTitle->GetWidth() - 26 - 3; //24=�簢������,3=��������
	float windowCloseY = currentTitle->GetY() + 4;
	editor->windowClose->Move(windowCloseX, windowCloseY);
	editor->update();
	//========================ĵ���� Ÿ��Ʋ ��========================

	QRect frameRect = this->frameRect();
	if (this->painter != NULL) {
		delete this->painter;
		this->painter = new QtPainter(frameRect.width(), frameRect.height());
	}

	if (this->scrollController == NULL) {
		this->scrollController = new ScrollController(this);

		this->scrollController->GetScroll(0)->setValue(583);

		this->scrollController->GetScroll(1)->setValue(427);

		this->scrollController->Update();
	}
	if (this->flowChart != NULL) {
		this->scrollController->Update();
	}

	this->repaint();
}

void DrawingPaper::wheelEvent(QWheelEvent *event) {
	QPoint delta = event->angleDelta();
	bool isControlPressed = ((::GetKeyState(VK_CONTROL) & 0x8000) != 0);
	if (isControlPressed && this->scrollController->GetScroll(0) != NULL) { //zoom
		Long oldRate = this->zoom->GetRate();
		Long rate;
		QString rateStatus;
		if (delta.y() > 0 && oldRate < 150) {
			rate = oldRate + 10;
			this->zoom->Set(rate);

			rateStatus = QString::number(rate);
			rateStatus += "%";
			dynamic_cast<FlowChartEditor*>(this->parentWidget())->zoomStatus->setText(rateStatus);
		}
		else if (delta.y() < 0 && oldRate > 40) {
			rate = oldRate - 10;
			this->zoom->Set(rate);

			rateStatus = QString::number(rate);
			rateStatus += "%";
			dynamic_cast<FlowChartEditor*>(this->parentWidget())->zoomStatus->setText(rateStatus);
		}
		dynamic_cast<FlowChartEditor*>(this->parentWidget())->statusBar->repaint();
		this->scrollController->Update();
	}
	else {
		Long value = this->scrollController->Rotate(delta);
		this->repaint();
	}
}

void DrawingPaper::keyPressEvent(QKeyEvent *event) {
	//CDC *pDC = CWnd::GetDC();

	FlowChartEditor *editor = (FlowChartEditor*)this->parentWidget();
	FlowChartKeyActionFactory keyActionFactory(editor);
	FlowChartKeyAction *keyAction = keyActionFactory.Make(event->modifiers(), event->key());
	if (keyAction != 0) {
		keyAction->OnKeyDown();
		delete keyAction;

		QString mode = this->GetCurrentMode();
		editor->modeStatus->setText(mode);
		editor->statusBar->repaint();
	}
	editor->windows[1]->repaint(); //��?

	this->repaint();
}

void DrawingPaper::focusOutEvent(QFocusEvent *event) {
	QFrame::focusOutEvent(event);
}

void DrawingPaper::focusInEvent(QFocusEvent *event) {
	QFrame::focusInEvent(event);
}

void DrawingPaper::enterEvent(QEvent *event) {
	this->setFocus();
}

void DrawingPaper::OnContextMenu(const QPoint& pos) {
	//HMENU hPopup;
	this->flowChart->AscendingSort();
	this->popup = new QMenu(this);

	QAction *moveMake = this->popup->addAction(QString::fromLocal8Bit("��ȣ ��ġ ����"), this, &DrawingPaper::OnMoveMakeMenuClick);
	//connect(moveMake, &QAction::triggered, this, &DrawingPaper::OnMoveMakeMenuClick);
	QAction *sizeMake = this->popup->addAction(QString::fromLocal8Bit("��ȣ ũ�� ����"), this, &DrawingPaper::OnSizeMakeMenuClick);
	QAction *intervalMake = this->popup->addAction(QString::fromLocal8Bit("��ȣ ���� ����"), this, &DrawingPaper::OnIntervalMakeMenuClick);
	QAction *sequence = this->popup->addAction(QString::fromLocal8Bit("��������"), this, &DrawingPaper::OnSequenceMenuClick);
	QAction *iteration = this->popup->addAction(QString::fromLocal8Bit("�ݺ�����"), this, &DrawingPaper::OnIterationMenuClick);
	QAction *selection = this->popup->addAction(QString::fromLocal8Bit("���ñ���"), this, &DrawingPaper::OnSelectionMenuClick);

	moveMake->setEnabled(false);
	sizeMake->setEnabled(false);
	intervalMake->setEnabled(false);
	sequence->setEnabled(false);
	iteration->setEnabled(false);
	selection->setEnabled(false);


	Long(*indexes);
	Long count;
	NShape *shape;
	this->flowChart->GetSelecteds(&indexes, &count);

	if (count > 1) {
		moveMake->setEnabled(true);
		sizeMake->setEnabled(true);
		intervalMake->setEnabled(true);
		sequence->setEnabled(true);

		shape = this->flowChart->GetAt(indexes[0]);
		if (dynamic_cast<Decision *>(shape)) {
			iteration->setEnabled(true);
			//19.09.16 �Ǵܱ�ȣ ���ʿ� ó���� ������ ������ ���� ��ȣ �޴� Ȱ��ȭX
			Long leftCount = 0;
			Long i = 1;
			while (i < count) {
				NShape *left = this->flowChart->GetAt(indexes[i]);
				if (left->CenterOfGravityX() < shape->CenterOfGravityX()) {
					leftCount++;
				}
				i++;
			}
			if (leftCount > 0) {
				selection->setEnabled(true);
			}
		}
	}

	if (indexes != 0)
	{
		delete[] indexes;
	}

	this->popup->popup(this->mapToGlobal(pos));
}

void DrawingPaper::DrawSelectingArea() {
	int compositionMode = this->painter->GetCompositionMode();
	this->painter->SetCompositionMode(QPainter::RasterOp_NotSourceXorDestination);

	QtGObjectFactory factory;
	GObject *pen = factory.MakePen(QBrush(QColor(166, 166, 166)), 1, Qt::DotLine);
	GObject *oldPen = this->painter->SelectObject(*pen);
	this->painter->Update();

	float x = this->startX;
	float width = this->currentX - this->startX;
	if (this->startX > this->currentX) {
		x = this->currentX;
		width = this->startX - this->currentX;
	}
	float y = this->startY;
	float height = this->currentY - this->startY;
	if (this->startY > this->currentY) {
		y = this->currentY;
		height = this->startY - this->currentY;
	}

	QRectF drawRect(x, y, width, height);
	this->painter->DrawRect(drawRect);

	this->painter->SelectObject(*oldPen);
	this->painter->Update();
	if (pen != 0) {
		delete pen;
	}
	this->painter->SetCompositionMode(compositionMode);

	this->drawSelectingAreaFlag = false;
}

void DrawingPaper::DrawActiveShape(NShape *entity) {
	//���� dc�����ؼ� �ٷ� �׸�(�Ӽ� Painter ��� ����)
	QPainter dc(this);

	QRect rect = this->frameRect();
	QtPainter painter(rect.width(), rect.height());
	int bgMode = painter.GetBackgroundMode();
	painter.SetBackgroundMode(Qt::TransparentMode);

	painter.SetCompositionMode(QPainter::RasterOp_NotSourceXorDestination);
	QtGObjectFactory factory;
	GObject *pen = factory.MakePen(QBrush(QColor(0, 0, 255)), 1, Qt::DotLine);
	painter.SelectObject(*pen);
	GObject *brush = factory.MakeBrush(QColor(255, 255, 255));
	painter.SelectObject(*brush);
	painter.Update();

	entity->DrawActiveShape(&painter);

	painter.Render(&dc, 0, 0);
}

void DrawingPaper::DrawActiveShape2(NShape *entity) {
	int oldMode = this->painter->GetBackgroundMode();
	this->painter->SetBackgroundMode(Qt::TransparentMode);

	QtGObjectFactory factory;
	GObject *pen = factory.MakePen(QBrush(QColor(0, 0, 255)), 1, Qt::DashLine);
	GObject *oldPen = this->painter->SelectObject(*pen);
	GObject *brush = factory.MakeBrush(QColor(0, 0, 255)); //BS_NULL style
	GObject *oldBrush = this->painter->SelectObject(*brush);
	this->painter->Update();

	entity->DrawActiveShape(this->painter);

	this->painter->SelectObject(*oldPen);
	this->painter->SelectObject(*oldBrush);
	this->painter->Update();
	if (pen != NULL) {
		delete pen;
	}
	if (brush != NULL) {
		delete brush;
	}
	this->painter->SetBackgroundMode(oldMode);
}

void DrawingPaper::DrawSelectionMark(QPoint(*points), Long count) {
	//���� dc �����ؼ� �ٷ� �׸�(�Ӽ� Painter ��� ����)
	QPainter dc(this);
	QRect rect = this->frameRect();

	QtPainter painter(rect.width(), rect.height());

	painter.SetBackgroundMode(Qt::TransparentMode);
	painter.SetCompositionMode(QPainter::RasterOp_NotSourceXorDestination);

	QtGObjectFactory factory;
	GObject *pen = factory.MakePen(QBrush(QColor(0, 0, 255)), 1);
	painter.SelectObject(*pen);
	GObject *brush = factory.MakeBrush(QColor(255, 255, 255)); // NULL_BRUSH
	painter.SelectObject(*brush);

	QPoint selectionMarkPoints[BOXVERTECIES];
	Long i = 0;
	while (i < count) {
		selectionMarkPoints[0].setX(points[i].x() - BOXLENGTH);
		selectionMarkPoints[1].setX(points[i].x() + BOXLENGTH);
		selectionMarkPoints[2].setX(points[i].x() + BOXLENGTH);
		selectionMarkPoints[3].setX(points[i].x() - BOXLENGTH);

		selectionMarkPoints[0].setY(points[i].y() - BOXLENGTH);
		selectionMarkPoints[1].setY(points[i].y() - BOXLENGTH);
		selectionMarkPoints[2].setY(points[i].y() + BOXLENGTH);
		selectionMarkPoints[3].setY(points[i].y() + BOXLENGTH);

		painter.DrawPolygon(selectionMarkPoints, BOXVERTECIES);
		i++;
	}

	painter.Render(&dc, 0, 0);

	/* ������ dc���� QPainter�� ������ �������� �ʾҰ�
	�ӽ÷� ����� QtPainter�� ������ ���̱� ������ ���� ó�� ���ص� ��.
	dc->SelectObject(oldPen);
	dc->SelectObject(oldBrush);

	dc->SetBkMode(oldmode);
	this->ReleaseDC(dc);

	pen.DeleteObject();
	*/
}

void DrawingPaper::DrawSelectionMark2(QPoint(*points), Long count) {
	//�Ӽ� Painter ��� ����
	int oldMode = this->painter->GetBackgroundMode();
	this->painter->SetBackgroundMode(Qt::OpaqueMode);

	QtGObjectFactory factory;
	GObject *pen = factory.MakePen(QBrush(QColor(0, 0, 255)), 1);
	GObject *oldPen = this->painter->SelectObject(*pen);
	GObject *brush = factory.MakeBrush(QColor(0, 0, 255)); //BS_NULL
	GObject *oldBrush = this->painter->SelectObject(*brush);
	this->painter->Update();

	QPoint selectionMarkPoints[BOXVERTECIES];
	Long i = 0;
	while (i < count) {
		selectionMarkPoints[0].setX(points[i].x() - BOXLENGTH);
		selectionMarkPoints[1].setX(points[i].x() + BOXLENGTH);
		selectionMarkPoints[2].setX(points[i].x() + BOXLENGTH);
		selectionMarkPoints[3].setX(points[i].x() - BOXLENGTH);

		selectionMarkPoints[0].setY(points[i].y() - BOXLENGTH);
		selectionMarkPoints[1].setY(points[i].y() - BOXLENGTH);
		selectionMarkPoints[2].setY(points[i].y() + BOXLENGTH);
		selectionMarkPoints[3].setY(points[i].y() + BOXLENGTH);

		this->painter->DrawPolygon(selectionMarkPoints, BOXVERTECIES);
		i++;
	}

	this->painter->SelectObject(*oldPen);
	this->painter->SelectObject(*oldBrush);
	this->painter->Update();
	this->painter->SetBackgroundMode(oldMode);

	if (pen != NULL) {
		delete pen;
	}
	if (brush != NULL) {
		delete brush;
	}
}

void DrawingPaper::FillSelectionMark(QPoint point) {
	//dc�� �ٷ� �׸�
	QPainter dc(this);
	QRect rect = this->frameRect();
	QtPainter painter(rect.width(), rect.height());

	painter.SetBackgroundMode(Qt::OpaqueMode);
	painter.SetCompositionMode(QPainter::RasterOp_NotSourceXorDestination);

	QtGObjectFactory factory;
	GObject *pen = factory.MakePen(QBrush(QColor(0, 0, 255)), 1);
	painter.SelectObject(*pen);
	GObject *brush = factory.MakeBrush(QColor(0, 0, 255));
	painter.SelectObject(*brush);
	painter.Update();

	QRect selectionMark;
	selectionMark.setCoords(point.x() + 1 - MARKLENGTH, point.y() + 1 - MARKLENGTH,
		point.x() + MARKLENGTH, point.y() + MARKLENGTH);
	painter.DrawRect(selectionMark);

	painter.Render(&dc, 0, 0);
}

QCursor DrawingPaper::GetCursor(QPoint point) {
	QCursor result;
	Long i = 0;
	Long end;
	NShape *shape;

	if (this->mode == SELECT) {
		Long positionX = this->scrollController->GetScroll(1)->value();
		Long positionY = this->scrollController->GetScroll(0)->value();
		point.setX(point.x() + positionX);
		point.setY(point.y() + positionY);

		NShape *holdA4Paper = this->a4Paper->Clone();
		FlowChartVisitor *zoomVisitor = new ZoomVisitor(this->zoom);
		holdA4Paper->Accept(zoomVisitor);

		QPointF currentReal(point.x(), point.y());
		QPointF currentVirtual = dynamic_cast<ZoomVisitor*>(zoomVisitor)->converter->ConvertVirtual(currentReal);

		currentVirtual.setX(currentVirtual.x() * 100 / this->zoom->GetRate());
		currentVirtual.setY(currentVirtual.y() * 100 / this->zoom->GetRate());

		currentReal = dynamic_cast<ZoomVisitor*>(zoomVisitor)->converter->ConvertReal(currentVirtual);

		point.setX(currentReal.x());
		point.setY(currentReal.y());

		end = this->flowChart->GetLength();
		while (i < end) {
			shape = this->flowChart->GetAt(i);
			if (shape->IsSelected()) {
				int hitCode = shape->GetHitCode(point);
				if (hitCode != HIT_NONE && hitCode != HIT_BODY) {
					result = shape->GetCursor(hitCode);
				}
			}
			i++;
		}
	}
	else if (this->mode == DRAWING) {
		result.setShape(Qt::CrossCursor);
	}

	return result;
}

void DrawingPaper::OnSequenceMenuClick() {
	this->tool->SequenceMake(this);
	
	this->Notify();

	this->repaint();
}

void DrawingPaper::OnIterationMenuClick() {
	this->tool->IterationMake(this);
	this->Notify();

	this->repaint();
}

void DrawingPaper::OnSelectionMenuClick() {
	this->tool->SelectionMake(this);
	this->Notify();

	this->repaint();
}

void DrawingPaper::OnMoveMakeMenuClick() {
	this->tool->MoveMake(this);
	this->Notify();

	this->repaint();
}

void DrawingPaper::OnSizeMakeMenuClick() {
	this->tool->SizeMake(this);
	this->Notify();

	this->repaint();
}

void DrawingPaper::OnIntervalMakeMenuClick() {
	this->tool->IntervalMake(this);
	this->Notify();

	this->repaint();
}

void DrawingPaper::New() {
	FlowChartEditor *editor = static_cast<FlowChartEditor *>(this->parentWidget());

	NShape *last = editor->sheetManager->GetTitle(editor->sheetManager->GetBinderLength() - 1);
	float lastRight = last->GetX() + last->GetWidth();
	//������ ĵ���� Ÿ��Ʋ�� �������� �������� �����ʺ��� ���� ���� �߰��� �� �� �ִ�.
	float right = this->x() + this->width();
	if (lastRight + 186 < right) { //186�� ĵ���� Ÿ��Ʋ�� �ּ� �ʺ�
		Long current = editor->sheetManager->New();
		editor->sheetManager->Change(current);
		editor->sheetManager->ModifyTitles();

		NShape *newTitle = editor->sheetManager->GetTitle(current);
		float windowCloseX = newTitle->GetX() + newTitle->GetWidth() - 26 - 3; //24=�簢������,3=��������
		float windowCloseY = newTitle->GetY() + 4;
		editor->windowClose->Move(windowCloseX, windowCloseY);

		editor->repaint();
	}
	else {
		//�޽��� �ڽ�
		QMessageBox messageBox(QMessageBox::Warning, QString::fromLocal8Bit("���"),
			QString::fromLocal8Bit("�� �� �� �����ϴ�. ������ ���� �� ���ʽÿ�."),
			QMessageBox::Ok, editor);
		int ret = messageBox.exec();
	}
}

Long DrawingPaper::Save(QString fileName) {
	File file;
	return file.Save(this, fileName);
}

Long DrawingPaper::Load(QString fileName) {
	File file;
	return file.Load(this, fileName);
}

void DrawingPaper::Close() {
	QString fileName;

	FlowChartEditor *editor = static_cast<FlowChartEditor *>(this->parentWidget());

	QMessageBox messageBox(QMessageBox::Question, QString::fromLocal8Bit("���� �����"),
		QString::fromLocal8Bit("���� ������ �����Ͻðڽ��ϱ�?"),
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this);
	int ret = messageBox.exec();
	if (ret == QMessageBox::Yes) { //������ �����Ѵٰ� �ϸ�
		QString fileOpenPath = editor->sheetManager->GetFileOpenPath(editor->sheetManager->GetBinderCurrent());
		if (fileOpenPath.isEmpty()) { //���� �������� ����Ǿ� ���� �ʴٸ� ���� �����Ѵ�.
			fileName = QFileDialog::getSaveFileName((QWidget*)editor,
				QObject::tr("Save File"),
				NULL,
				QObject::tr("Text files (*.txt)"));
			//fileName�� fileOpenPath�� ��ü�� ���ο� ���
			QFile file(fileName);
			bool isOpen = file.open(QIODevice::WriteOnly | QIODevice::Text);
			if (isOpen == true) {
				editor->sheetManager->ModifyFileOpenPath(fileName); //�� ��θ� ���̾�α׿��� �����ͼ� ��ħ.
				this->Save(fileName.toLocal8Bit().data());
			}
		}
		else { //���� �������� ����Ǿ� ������ �״�� �Ȱ��� ��ο� �̸����� �����Ѵ�.
			this->Save(fileOpenPath);
		}
	}

	if (ret != QMessageBox::Cancel) { //������ �ѹ� �� ������� �ݴ� �� ������
		//ĵ���� �����츦 ����.
		//this->flowChart->Clear();
		this->mode = IDLE;
		this->indexOfSelected = -1;

		//���� ĵ���� �����/���ο� ���� �������ְ�/�ݱ��ư �Ű��ֱ�.
		editor->sheetManager->Close();

		NShape *first = editor->sheetManager->GetTitle(0); //���ο� ���� : �� �տ���
		editor->sheetManager->Change(0);
		editor->sheetManager->ModifyTitles();

		float windowCloseX = first->GetX() + first->GetWidth() - 26 - 3; //24=�簢������,3=��������
		float windowCloseY = first->GetY() + 4;
		editor->windowClose->Move(windowCloseX, windowCloseY);
	}
}

QString DrawingPaper::GetCurrentMode() {
	QString mode;

	switch (this->mode) {
	case IDLE:
		mode = "IDLE"; break;
	case SELECT:
		mode = "SELECT"; break;
	case DRAWING:
		mode = "DRAWING"; break;
	case MOVING:
		mode = "MOVING"; break;
	case SIZING:
		mode = "SIZING"; break;
	default: break;
	}

	return mode;
}