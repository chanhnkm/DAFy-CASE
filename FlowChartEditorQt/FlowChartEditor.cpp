#include "FlowChartEditor.h"
#include "FlowChart/FlowChartCommandFactory.h"
#include "FlowChart/FlowChartCommands.h"
#include "FlowChart/DrawingPaper.h"
#include "FlowChart/FlowChartTemplate.h"
#include "FlowChart/WindowTitle.h"
#include "GObject/GObject.h"
#include "GObject/QtGObjectFactory.h"
#include "GObject/QtPainter.h"
#include "FlowChart/SketchBook.h"
#include "FlowChart/WindowTitle.h"
#include "FlowChart/FlowChartVisitor.h"
#include "FlowChart/DrawVisitor.h"
#include "FlowChart/WindowClose.h"
#include "FlowChart/Memory.h"
#include "FlowChart/MemoryController.h"
#include "FlowChart/Zoom.h"

#include <qmenubar.h>
#include <qevent.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include <qstatusbar.h>
#include <qlabel.h>

FlowChartEditor::FlowChartEditor(QWidget *parent)
	: QFrame(parent)
{
	ui.setupUi(this);

	this->setMouseTracking(true);
	this->setFocusPolicy(Qt::StrongFocus);
	this->installEventFilter(this);

	this->setStyleSheet("QStatusBar { background:rgb(102,204,204); }"
						"QStatusBar QLabel { color:white;}"
						"QStatusBar::item { border:None;}"
	);

	this->menuBar = NULL;
	this->statusBar = NULL;

	QRect frameRect = this->frameRect();

	this->CreateActions();
	this->CreateMenus();
	this->menuBar->resize(frameRect.width(), this->menuBar->height());

	//�ڽ� ������ ���� - ��ġ�� ũ��, ��Ÿ�� ����
	DrawingPaper *drawingPaper = new DrawingPaper(this);
	drawingPaper->move(200, this->menuBar->height() + 5 + 32);
	this->windows.Store(0, drawingPaper);

	FlowChartTemplate *fTemplate = new FlowChartTemplate(this);
	fTemplate->move(5, 5 + this->menuBar->height());
	this->windows.Store(1, fTemplate);

	this->painter = new QtPainter(frameRect.width(), frameRect.height(), QColor(235, 235, 235));

	this->sketchBook = new SketchBook;

	Long height = 26;
	NShape *firstTitle = new WindowTitle(drawingPaper->x(), drawingPaper->y() - height - 4, 186, height, QColor(102, 204, 204),
		Qt::SolidLine, QColor(102, 204, 204), String(" �������"));
	Long current = this->sketchBook->Add(firstTitle, drawingPaper->flowChart->Clone());

	firstTitle = this->sketchBook->GetCanvas(current);
	Long windowCloseX = firstTitle->GetX() + firstTitle->GetWidth() - 26 - 3; //24=�簢������,3=��������
	Long windowCloseY = firstTitle->GetY() + 4;
	this->windowClose = new WindowClose(windowCloseX, windowCloseY, 26, 23, QColor(102, 204, 204),
		Qt::SolidLine, QColor(255, 255, 255));

	this->CreateStatusBar();
}

void FlowChartEditor::closeEvent(QCloseEvent *event) {
	QMessageBox messageBox(QMessageBox::Warning, QString::fromLocal8Bit("�ݱ�"),
		QString::fromLocal8Bit("�����Ͱ� �սǵ� �� �ֽ��ϴ�. ������ ��� �����ߴ��� Ȯ���Ͻʽÿ�. ������ �����Ͻðڽ��ϱ�?"),
		QMessageBox::Yes | QMessageBox::No, this);
	int ret = messageBox.exec();
	if (ret == QMessageBox::Yes) {
		if (this->menuBar != NULL) {
			delete this->menuBar;
		}
		if (this->statusBar != NULL) {
			delete this->statusBar;
		}
		if (this->painter != NULL) {
			delete this->painter;
		}
		if (this->sketchBook != NULL) {
			delete this->sketchBook;
		}

		Long i = 0;
		while (i < 2) {
			delete this->windows[i];
			i++;
		}

		QFrame::closeEvent(event);
	}
	else {
		event->ignore();
	}
}

void FlowChartEditor::resizeEvent(QResizeEvent *event) {
	QRect frameRect = this->frameRect();

	this->menuBar->resize(frameRect.width(), this->menuBar->height());
	this->statusBar->move(frameRect.x(), frameRect.bottom() - this->statusBar->height());
	this->statusBar->resize(frameRect.width(), this->statusBar->height());

	this->windows[0]->resize(frameRect.width() - 200, frameRect.height() - this->menuBar->height() - 32 - 20 - this->statusBar->height());
	this->windows[0]->repaint();

	this->windows[1]->resize(190, frameRect.height() - 20 - this->menuBar->height() - this->statusBar->height());
	this->windows[1]->repaint();
}

void FlowChartEditor::paintEvent(QPaintEvent *event) {
	QPainter painter(this);

	QRect frameRect = this->frameRect();

	this->painter->Resize(frameRect.width(), frameRect.height(), QColor(235, 235, 235));

	DrawingPaper *canvas = dynamic_cast<DrawingPaper*>(this->windows[0]);
	QColor windowBorderColor = canvas->windowBorderColor;
	//=======â �׵θ�=========
	QtGObjectFactory factory;
	GObject *pen = factory.MakePen(QBrush(windowBorderColor), 3);
	GObject *oldPen = this->painter->SelectObject(*pen);
	this->painter->Update();

	QPoint p1(canvas->x() + 1, canvas->y() - 2);
	QPoint p2(canvas->x() + 1 + canvas->width(), canvas->y() - 2);
	this->painter->DrawLine(p1, p2);

	this->painter->SelectObject(*oldPen);
	this->painter->Update();
	if (pen != 0) {
		delete pen;
	}
	//=======â �׵θ�=========

	FlowChartVisitor *visitor = new DrawVisitor(this->painter);

	this->sketchBook->Draw(visitor);
	//�ݱ�
	this->windowClose->Accept(visitor);

	this->painter->Render(&painter, 0, 0);
}

void FlowChartEditor::mouseMoveEvent(QMouseEvent *event) {
	//������ ��
	QRect pinRect(this->windowClose->GetX(), this->windowClose->GetY(), this->windowClose->GetWidth(), this->windowClose->GetHeight());
	bool isContain = pinRect.contains(event->pos());
	if (isContain == true) {
		this->windowClose->Paint(QColor(102, 255, 255), Qt::SolidLine, this->windowClose->GetBorderColor());
	}
	else {
		this->windowClose->Paint(QColor(102, 204, 204), Qt::SolidLine, this->windowClose->GetBorderColor());
	}
	this->repaint();
}

void FlowChartEditor::mouseReleaseEvent(QMouseEvent *event) {
	DrawingPaper *canvas = static_cast<DrawingPaper*>(this->windows[0]);
	QRect pinRect(this->windowClose->GetX(), this->windowClose->GetY(), this->windowClose->GetWidth(), this->windowClose->GetHeight());
	bool isContain = pinRect.contains(event->pos());
	if (isContain == true) {
		if (this->sketchBook->GetLength() > 1) { //�� �� �̻��� ���� ���� �� ����.
			canvas->Close(); //���� ĵ���� �����ϰų� ���ϰų� ó������.
		}
		else {
			QMessageBox messageBox(QMessageBox::Warning, QString::fromLocal8Bit("���"),
				QString::fromLocal8Bit("��� ���� �� �����ϴ�."), QMessageBox::Ok, this);
			int ret = messageBox.exec();
		}
	}
	else {
		//����ġ���� ���´� : ���� ������ �ִ� ĵ������ �������� �����Ѵ�.
		this->sketchBook->Unfold(canvas->flowChart->Clone(),
			new Memory(*canvas->memoryController->GetUndoMemory()),
			new Memory(*canvas->memoryController->GetRedoMemory()));
		//����ġ���� ��ģ�� : ���� ĵ������ �� �ٲٱ�
		Long current = this->sketchBook->Fold(event->pos());
		//����ġ���� ��ģ�� : ���� �ٲ��ֱ�.
		this->sketchBook->Update();
		//����ġ���� ��ģ�� : ��ģ ĵ������ ����Ǿ��ִ� �������� �ٲ۴�. �޸𸮵� �����ش�.
		canvas->flowChart = this->sketchBook->GetFlowChart(this->sketchBook->GetCurrent())->Clone();
		Memory *undoMemory = new Memory(*this->sketchBook->GetUndoMemory(this->sketchBook->GetCurrent()));
		Memory *redoMemory = new Memory(*this->sketchBook->GetRedoMemory(this->sketchBook->GetCurrent()));
		canvas->memoryController->ChangeMemory(undoMemory, redoMemory);

		NShape *currentTitle = this->sketchBook->GetCanvas(current);
		Long windowCloseX = currentTitle->GetX() + currentTitle->GetWidth() - 26 - 3; //24=�簢������,3=��������
		Long windowCloseY = currentTitle->GetY() + 4;
		this->windowClose->Move(windowCloseX, windowCloseY);
	}
	this->repaint();
	canvas->repaint();
}

bool FlowChartEditor::eventFilter(QObject* o, QEvent* e) {
	if (o == this && e->type() == QEvent::ShortcutOverride) {
		e->accept();
	}
	return QFrame::eventFilter(o, e);
}

/*��Ŭ�� �޴��ΰ�?
void FlowChartEditor::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu) {
	CWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
}
*/

/*�޴� Ŀ�ǵ�
void FlowChartEditor::OnCommandRange(UINT uID) {
	FlowChartCommandFactory commandFactory(this);
	FlowChartCommand *command = commandFactory.Make(uID);
	if (command != NULL) {
		command->Execute();
		delete command;
	}
	this->Invalidate();
}
*/

/*Accelerators
BOOL FlowChartEditor::PreTranslateMessage(MSG *pMsg) {
	if (this->hAccel != NULL) {
		if (::TranslateAccelerator(*(AfxGetMainWnd()), this->hAccel, pMsg)) {
			return TRUE;
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}
*/

/* ���� �ε� ���� ���� ó�� ���� �Լ��ε�
void FlowChartEditor::OnDropFiles(HDROP hDropInfo) {
	char filePath[256];
	CString fileName;
	int count;
	(static_cast<DrawingPaper *>(this->windows[0]))->New();

	count = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (count > 0) {
		DragQueryFile(hDropInfo, 0, filePath, 255);
		this->fileOpenPath = filePath;
		(static_cast<DrawingPaper *>(this->windows[0]))->Load(this->fileOpenPath);
		fileName = this->fileOpenPath + " - FlowChart";
		this->SetWindowText(fileName);
		(static_cast<DrawingPaper *>(this->windows[0]))->SetFocus();
		this->RedrawWindow();
	}
}
*/

/* �޴� ��Ʈ��
void FlowChartEditor::OnUpdateDrawingUnModeCommand(CCmdUI *cCmdUI) {
	if (this->isUnModeMenuEnabled == FALSE) { //������ ��Ȱ��ȭ�Ǿ�� �ϸ�
		cCmdUI->Enable(FALSE); //������ ��Ȱ��ȭ�Ѵ�.
	}
	else {
		cCmdUI->Enable(TRUE); //������ Ȱ��ȭ�Ѵ�.
	}
}

void FlowChartEditor::OnUpdateDrawingModeCommand(CCmdUI *cCmdUI) {
	if (this->isUnModeMenuEnabled == FALSE) { //������ ��Ȱ��ȭ�Ǿ�� �ϸ�
		cCmdUI->Enable(TRUE); //������ Ȱ��ȭ�Ѵ�.
	}
	else {
		cCmdUI->Enable(FALSE); //������ ��Ȱ��ȭ�Ѵ�.
	}
}

void FlowChartEditor::OnUpdateCopyCutDeleteCommand(CCmdUI *cCmdUI) {
	if (dynamic_cast<DrawingPaper*>(this->windows[0])->mode != DrawingPaper::SELECT) { //���� ���°� �ƴϸ� ��Ȱ��ȭ�Ѵ�.
		cCmdUI->Enable(FALSE);
	}
	else { //���� ���¸� Ȱ��ȭ�Ѵ�.
		cCmdUI->Enable(TRUE);
	}
}

void FlowChartEditor::OnUpdatePasteCommand(CCmdUI *cCmdUI) {
	if (dynamic_cast<DrawingPaper*>(this->windows[0])->clipboard->GetLength() <= 0) { //Ŭ������ ���ۿ� �ƹ��͵� ������ ��Ȱ��ȭ�Ѵ�.
		cCmdUI->Enable(FALSE);
	}
	else {
		cCmdUI->Enable(TRUE);
	}
}

void FlowChartEditor::OnUpdateUndoCommand(CCmdUI *cCmdUI) {
	if (dynamic_cast<DrawingPaper*>(this->windows[0])->memoryController->GetUndoMemory()->GetLength() <= 0) { //UndoMemory�� �ƹ��͵� ������ ��Ȱ��ȭ�Ѵ�.
		cCmdUI->Enable(FALSE);
	}
	else {
		cCmdUI->Enable(TRUE);
	}
}

void FlowChartEditor::OnUpdateRedoCommand(CCmdUI *cCmdUI) {
	if (dynamic_cast<DrawingPaper*>(this->windows[0])->memoryController->GetRedoMemory()->GetLength() <= 0) { //RedoMemory�� �ƹ��͵� ������ ��Ȱ��ȭ�Ѵ�.
		cCmdUI->Enable(FALSE);
	}
	else {
		cCmdUI->Enable(TRUE);
	}
}

void FlowChartEditor::OnUpdatePositionCommand(CCmdUI *cCmdUI) {
	Long(*indexes);
	Long count;
	(dynamic_cast<FlowChart *>(dynamic_cast<DrawingPaper*>(this->windows[0])->flowChart))->GetSelecteds(&indexes, &count);

	if (count > 1) {
		cCmdUI->Enable(TRUE);
	}
	else {
		cCmdUI->Enable(FALSE);
	}
}

void FlowChartEditor::OnUpdateSizeCommand(CCmdUI *cCmdUI) {
	Long(*indexes);
	Long count;
	(dynamic_cast<FlowChart *>(dynamic_cast<DrawingPaper*>(this->windows[0])->flowChart))->GetSelecteds(&indexes, &count);

	if (count > 1) {
		cCmdUI->Enable(TRUE);
	}
	else {
		cCmdUI->Enable(FALSE);
	}
}

void FlowChartEditor::OnUpdateIntervalCommand(CCmdUI *cCmdUI) {
	Long(*indexes);
	Long count;
	(dynamic_cast<FlowChart *>(dynamic_cast<DrawingPaper*>(this->windows[0])->flowChart))->GetSelecteds(&indexes, &count);

	if (count > 1) {
		cCmdUI->Enable(TRUE);
	}
	else {
		cCmdUI->Enable(FALSE);
	}
}

void FlowChartEditor::OnUpdateSequenceCommand(CCmdUI *cCmdUI) {
	Long(*indexes);
	Long count;
	(dynamic_cast<FlowChart *>(dynamic_cast<DrawingPaper*>(this->windows[0])->flowChart))->GetSelecteds(&indexes, &count);

	if (count > 1) {
		cCmdUI->Enable(TRUE);
	}
	else {
		cCmdUI->Enable(FALSE);
	}
}

void FlowChartEditor::OnUpdateIterationCommand(CCmdUI *cCmdUI) {
	Long(*indexes);
	Long count;
	(dynamic_cast<FlowChart *>(dynamic_cast<DrawingPaper*>(this->windows[0])->flowChart))->GetSelecteds(&indexes, &count);
	if (count > 1) {
		NShape *shape = dynamic_cast<FlowChart*>(dynamic_cast<DrawingPaper*>(this->windows[0])->
			flowChart)->GetAt(indexes[0]);
		if (dynamic_cast<Decision*>(shape)) {
			cCmdUI->Enable(TRUE);
		}
		else {
			cCmdUI->Enable(FALSE);
		}
	}
}

void FlowChartEditor::OnUpdateSelectionCommand(CCmdUI *cCmdUI) {
	Long(*indexes);
	Long count;
	(dynamic_cast<FlowChart *>(dynamic_cast<DrawingPaper*>(this->windows[0])->flowChart))->GetSelecteds(&indexes, &count);
	if (count > 1) {
		NShape *shape = dynamic_cast<FlowChart*>(dynamic_cast<DrawingPaper*>(this->windows[0])->
			flowChart)->GetAt(indexes[0]);
		if (dynamic_cast<Decision*>(shape)) {
			Long leftCount = 0;
			Long i = 1;
			while (i < count) {
				NShape *left = dynamic_cast<FlowChart*>(dynamic_cast<DrawingPaper*>(this->windows[0])->flowChart)->GetAt(indexes[i]);
				if (left->CenterOfGravityX() < shape->CenterOfGravityX()) {
					leftCount++;
				}
				i++;
			}
			if (leftCount > 0) {
				cCmdUI->Enable(TRUE);
			}
			else {
				cCmdUI->Enable(FALSE);
			}
		}
		else {
			cCmdUI->Enable(FALSE);
		}
	}
}
*/

void FlowChartEditor::CommandRange(string text) { //���ڿ��� �ƴ� #define���� �����صΰ� ���� �� ȿ���� ������?
	FlowChartCommandFactory commandFactory(this);
	FlowChartCommand *command = commandFactory.Make(text); //action->text()
	if (command != NULL) {
		command->Execute();
		delete command;
	}

	DrawingPaper *drawingPaper = static_cast<DrawingPaper*>(this->windows[0]);
	QString mode = drawingPaper->GetCurrentMode();
	this->modeStatus->setText(mode);
	this->statusBar->repaint();

	this->repaint();
}

void FlowChartEditor::CreateActions() {
	//==================== File Menu ====================
	this->newAction = new QAction(QString::fromLocal8Bit(("�� ����(&N)")), this);
	this->newAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	connect(this->newAction, &QAction::triggered, this, [=]() { this->CommandRange("New"); });

	this->openAction = new QAction(QString::fromLocal8Bit(("����(&O)")), this);
	this->openAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	connect(this->openAction, &QAction::triggered, this, [=]() { this->CommandRange("Open"); });

	this->saveAction = new QAction(QString::fromLocal8Bit(("����(&S)")), this);
	this->saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
	connect(this->saveAction, &QAction::triggered, this, [=]() { this->CommandRange("Save"); });

	this->saveAsAction = new QAction(QString::fromLocal8Bit(("�ٸ� �̸����� ����(&A)...")), this);
	this->saveAsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_S));
	connect(this->saveAsAction, &QAction::triggered, this, [=]() { this->CommandRange("SaveAs"); });

	this->saveAsImageAction = new QAction(QString::fromLocal8Bit(("�̹���(JPG)�� ����(&I)...")), this);
	this->saveAsImageAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_I));
	connect(this->saveAsImageAction, &QAction::triggered, this, [=]() { this->CommandRange("SaveAsImage"); });

	this->printAction = new QAction(QString::fromLocal8Bit(("�μ�(&P)...")), this);
	this->printAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	connect(this->printAction, &QAction::triggered, this, [=]() { this->CommandRange("Print"); });

	this->exitAction = new QAction(QString::fromLocal8Bit(("������(&X)...")), this);
	this->exitAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
	connect(this->exitAction, &QAction::triggered, this, [=]() { this->CommandRange("Close"); });
	//==================== File Menu ====================

	//==================== Edit Menu ====================
	this->undoAction = new QAction(QString::fromLocal8Bit(("���� ���(&U)")), this); //���� ���(U) Ctrl + Z
	this->undoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
	connect(this->undoAction, &QAction::triggered, this, [=]() { this->CommandRange("Undo"); });

	this->redoAction = new QAction(QString::fromLocal8Bit(("�ٽ� ����(&R)")), this); //�ٽ� ����(R) Ctrl + Y
	this->redoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
	connect(this->redoAction, &QAction::triggered, this, [=]() { this->CommandRange("Redo"); });

	this->copyAction = new QAction(QString::fromLocal8Bit(("�����ϱ�(&C)")), this); //�����ϱ�(C) Ctrl + C
	this->copyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	connect(this->copyAction, &QAction::triggered, this, [=]() { this->CommandRange("Copy"); });

	this->pasteAction = new QAction(QString::fromLocal8Bit(("�ٿ��ֱ�(&P)")), this); //�ٿ��ֱ�(P) Ctrl + V
	this->pasteAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
	connect(this->pasteAction, &QAction::triggered, this, [=]() { this->CommandRange("Paste"); });

	this->cutAction = new QAction(QString::fromLocal8Bit(("�߶󳻱�(&T)")), this); //�߶󳻱�(T) Ctrl + X
	this->cutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	connect(this->cutAction, &QAction::triggered, this, [=]() { this->CommandRange("Cut"); });

	this->deleteAction = new QAction(QString::fromLocal8Bit(("����(&L)")), this); //����(L) Del
	this->deleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
	connect(this->deleteAction, &QAction::triggered, this, [=]() { this->CommandRange("Delete"); });

	this->selectAllAction = new QAction(QString::fromLocal8Bit(("��� ����(&A)")), this); //��� ����(A) Ctrl + A
	this->selectAllAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
	connect(this->selectAllAction, &QAction::triggered, this, [=]() { this->CommandRange("SelectAll"); });

	this->positionAction = new QAction(QString::fromLocal8Bit(("��ȣ ��ġ ����(&O)")), this); //��ȣ ��ġ ����(O)
	connect(this->positionAction, &QAction::triggered, this, [=]() { this->CommandRange("Position"); });

	this->sizeAction = new QAction(QString::fromLocal8Bit(("��ȣ ũ�� ����(&S)")), this); //��ȣ ũ�� ����(S)
	connect(this->sizeAction, &QAction::triggered, this, [=]() { this->CommandRange("Size"); });

	this->intervalAction = new QAction(QString::fromLocal8Bit(("��ȣ ���� ����(&I)")), this); //��ȣ ���� ����(I)
	connect(this->intervalAction, &QAction::triggered, this, [=]() { this->CommandRange("Interval"); });
	//==================== Edit Menu ====================

	//==================== Format Menu ====================
	this->fontSetAction = new QAction(QString::fromLocal8Bit(("�۲�(&F)...")), this);
	connect(this->fontSetAction, &QAction::triggered, this, [=]() { this->CommandRange("FontSet"); });

	this->pageSetAction = new QAction(QString::fromLocal8Bit(("������ ����(&U)...")), this);
	connect(this->pageSetAction, &QAction::triggered, this, [=]() { this->CommandRange("PageSet"); });
	//==================== Format Menu ====================

	//==================== Add Menu ====================
	this->drawingModeAction = new QAction(QString::fromLocal8Bit(("�׸��� ���(&M)")), this); //�׸��� ���(M) Ctrl + D
	this->drawingModeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
	connect(this->drawingModeAction, &QAction::triggered, this, [=]() { this->CommandRange("DrawingMode"); });

	this->drawingUnModeAction = new QAction(QString::fromLocal8Bit(("�׸��� ��� ����(&U)")), this); //�׸��� ��� ���� ESC
	this->drawingUnModeAction->setShortcut(QKeySequence(Qt::Key_Escape));
	connect(this->drawingUnModeAction, &QAction::triggered, this, [=]() { this->CommandRange("DrawingUnMode"); });

	this->startTerminalSymbolAction = new QAction(QString::fromLocal8Bit(("���� �ܸ� ��ȣ(&S)")), this); //���� �ܸ� ��ȣ(S)
	connect(this->startTerminalSymbolAction, &QAction::triggered, this, [=]() { this->CommandRange("StartTerminalSymbol"); });

	this->preparationSymbolAction = new QAction(QString::fromLocal8Bit(("�غ� ��ȣ(&P)")), this); //�غ� ��ȣ(P)
	connect(this->preparationSymbolAction, &QAction::triggered, this, [=]() { this->CommandRange("PreparationSymbol"); });

	this->inputSymbolAction = new QAction(QString::fromLocal8Bit(("�Է� ��ȣ(&I)")), this); //�Է� ��ȣ(I)
	connect(this->inputSymbolAction, &QAction::triggered, this, [=]() { this->CommandRange("InputSymbol"); });

	this->processSymbolAction = new QAction(QString::fromLocal8Bit(("ó�� ��ȣ(&R)")), this); //ó�� ��ȣ(R)
	connect(this->processSymbolAction, &QAction::triggered, this, [=]() { this->CommandRange("ProcessSymbol"); });

	this->decisionSymbolAction = new QAction(QString::fromLocal8Bit(("�Ǵ� ��ȣ(&D)")), this); //�Ǵ� ��ȣ(D)
	connect(this->decisionSymbolAction, &QAction::triggered, this, [=]() { this->CommandRange("DecisionSymbol"); });

	this->outputSymbolAction = new QAction(QString::fromLocal8Bit(("��� ��ȣ(&O)")), this); //��� ��ȣ(O)
	connect(this->outputSymbolAction, &QAction::triggered, this, [=]() { this->CommandRange("OutputSymbol"); });

	this->stopTerminalSymbolAction = new QAction(QString::fromLocal8Bit(("���� �ܸ� ��ȣ(&T)")), this); //���� �ܸ� ��ȣ(T)
	connect(this->stopTerminalSymbolAction, &QAction::triggered, this, [=]() { this->CommandRange("StopTerminalSymbol"); });
	//==================== Add Menu ====================

	//==================== ControlArchitecture Menu ====================
	this->sequenceArchitectureAction = new QAction(QString::fromLocal8Bit(("��������(&S)")), this); //��������(S)
	connect(this->sequenceArchitectureAction, &QAction::triggered, this, [=]() { this->CommandRange("SequenceArchitecture"); });

	this->iterationArchitectureAction = new QAction(QString::fromLocal8Bit(("�ݺ�����(&I)")), this); //�ݺ�����(I)
	connect(this->iterationArchitectureAction, &QAction::triggered, this, [=]() { this->CommandRange("IterationArchitecture"); });

	this->selectionArchitectureAction = new QAction(QString::fromLocal8Bit(("���ñ���(&E)")), this); //���ñ���(E)
	connect(this->selectionArchitectureAction, &QAction::triggered, this, [=]() { this->CommandRange("SelectionArchitecture"); });
	//==================== ControlArchitecture Menu ====================

	//==================== Help Menu ====================

	//==================== Help Menu ====================
}

void FlowChartEditor::CreateMenus() {
	this->menuBar = new QMenuBar(this);

	this->fileMenu = this->menuBar->addMenu(QString::fromLocal8Bit(("����(&F)")));
	this->fileMenu->addAction(this->newAction); //�� ����(N) Ctrl + N
	this->fileMenu->addAction(this->openAction); //����(O) Ctrl + O
	this->fileMenu->addAction(this->saveAction); //����(S) Ctrl + S
	this->fileMenu->addAction(this->saveAsAction); //�ٸ� �̸����� ����(A)... Ctrl + Alt + S
	this->fileMenu->addAction(this->saveAsImageAction); //�̹���(JPG)�� ����(I)... Ctrl + Alt + I
	this->fileMenu->addSeparator(); //���м�
	this->fileMenu->addAction(this->printAction); //�μ�(P)... Ctrl + P
	this->fileMenu->addSeparator(); //���м�
	this->fileMenu->addAction(this->exitAction); //������(X) Alt + F4

	this->editMenu = this->menuBar->addMenu(QString::fromLocal8Bit(("����(&E)")));
	this->editMenu->addAction(this->undoAction); //���� ���(U) Ctrl + Z
	this->editMenu->addAction(this->redoAction); //�ٽ� ����(R) Ctrl + Y
	this->editMenu->addSeparator(); //���м�
	this->editMenu->addAction(this->copyAction); //�����ϱ�(C) Ctrl + C
	this->editMenu->addAction(this->pasteAction); //�ٿ��ֱ�(P) Ctrl + V
	this->editMenu->addAction(this->cutAction); //�߶󳻱�(T) Ctrl + X
	this->editMenu->addAction(this->deleteAction); //����(L) Del
	this->editMenu->addSeparator(); //���м�
	this->editMenu->addAction(this->selectAllAction); //��� ����(A) Ctrl + A
	this->editMenu->addSeparator(); //���м�
	this->editMenu->addAction(this->positionAction); //��ȣ ��ġ ����(O)
	this->editMenu->addAction(this->sizeAction); //��ȣ ũ�� ����(S)
	this->editMenu->addAction(this->intervalAction); //��ȣ ���� ����(I)

	this->formatMenu = this->menuBar->addMenu(QString::fromLocal8Bit(("����(&O)")));
	this->formatMenu->addAction(this->fontSetAction); //�۲�(F)...
	this->formatMenu->addAction(this->pageSetAction); //������ ����(U)...

	this->addMenu = this->menuBar->addMenu(QString::fromLocal8Bit(("����(&A)")));
	this->addMenu->addAction(this->drawingModeAction); //�׸��� ���(M) Ctrl + D
	this->addMenu->addAction(this->drawingUnModeAction); //�׸��� ��� ���� ESC
	this->addMenu->addSeparator(); //���м�
	this->addMenu->addAction(this->startTerminalSymbolAction); //���� �ܸ� ��ȣ(S)
	this->addMenu->addAction(this->preparationSymbolAction); //�غ� ��ȣ(P)
	this->addMenu->addAction(this->inputSymbolAction); //�Է� ��ȣ(I)
	this->addMenu->addAction(this->processSymbolAction); //ó�� ��ȣ(R)
	this->addMenu->addAction(this->decisionSymbolAction); //�Ǵ� ��ȣ(D)
	this->addMenu->addAction(this->outputSymbolAction); //��� ��ȣ(O)
	this->addMenu->addAction(this->stopTerminalSymbolAction); //���� �ܸ� ��ȣ(T)

	this->controlArchitectureMenu = this->menuBar->addMenu(QString::fromLocal8Bit(("�����(&C)")));
	this->controlArchitectureMenu->addAction(this->sequenceArchitectureAction); //��������(S)
	this->controlArchitectureMenu->addAction(this->iterationArchitectureAction); //�ݺ�����(I)
	this->controlArchitectureMenu->addAction(this->selectionArchitectureAction); //���ñ���(E)

	this->helpMenu = this->menuBar->addMenu(QString::fromLocal8Bit(("����(&H)")));
}

void FlowChartEditor::CreateStatusBar() {
	this->statusBar = new QStatusBar(this);

	this->symbolStatus = new QLabel(QString::fromLocal8Bit(""));
	this->statusBar->addPermanentWidget(this->symbolStatus, 5);
	
	this->modeStatus = new QLabel(QString::fromLocal8Bit("IDLE"));
	this->statusBar->addPermanentWidget(this->modeStatus, 2);
	
	this->xStatus = new QLabel(QString::fromLocal8Bit("X: "));
	this->statusBar->addPermanentWidget(this->xStatus, 2);
	
	this->yStatus = new QLabel(QString::fromLocal8Bit("Y: "));
	this->statusBar->addPermanentWidget(this->yStatus, 2);

	Long zoomRate = dynamic_cast<DrawingPaper*>(this->windows[0])->zoom->GetRate();
	QString zoomString = QString::number(zoomRate);
	zoomString += '%';
	this->zoomStatus = new QLabel(zoomString);
	this->statusBar->addPermanentWidget(this->zoomStatus, 2);
	
	this->adStatus1 = new QLabel(QString::fromLocal8Bit("���� ��Ī"));
	this->adStatus1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	this->statusBar->addPermanentWidget(this->adStatus1, 2);
	
	this->adStatus2 = new QLabel(QString::fromLocal8Bit("02)587-9424"));
	this->statusBar->addPermanentWidget(this->adStatus2, 1);
}