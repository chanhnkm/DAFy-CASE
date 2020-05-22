#include "FlowChartEditor.h"
#include "FlowChart/FlowChartCommandFactory.h"
#include "FlowChart/FlowChartCommands.h"
#include "FlowChart/DrawingPaper.h"
#include "FlowChart/FlowChartTemplate.h"
#include "FlowChart/WindowTitle.h"
#include "GObject/GObject.h"
#include "GObject/QtGObjectFactory.h"
#include "GObject/QtPainter.h"
#include "FlowChart/WindowTitle.h"
#include "FlowChart/FlowChartVisitor.h"
#include "FlowChart/DrawVisitor.h"
#include "FlowChart/WindowClose.h"
#include "FlowChart/Zoom.h"
#include "FlowChart/VariableList.h"
#include "FlowChart/Clipboard.h"
#include "FlowChart/Decision.h"
#include "FlowChart/HistoryController.h"
#include "FlowChart/HistoryBook.h"
#include "FlowChart/Registrar.h"
#include "FlowChart/SheetManager.h"

#include <qmenubar.h>
#include <qevent.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qicon.h>

FlowChartEditor::FlowChartEditor(QWidget *parent)
	: QFrame(parent)
{
	ui.setupUi(this);

	this->setMouseTracking(true);
	this->setFocusPolicy(Qt::StrongFocus);
	this->installEventFilter(this);

	this->setWindowState(Qt::WindowMaximized);

	this->setStyleSheet("QStatusBar { background:rgb(102,204,204); }"
		"QStatusBar QLabel { color:white;}"
		"QStatusBar::item { border:None;}"
	);

	this->setWindowTitle("Dafy Case");
	QIcon icon("iconTrans_ver.3");
	this->setWindowIcon(icon);

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

	this->painter = new QtPainter((Long)frameRect.width(), (Long)frameRect.height(), QColor(235, 235, 235));

	this->sheetManager = new SheetManager(drawingPaper);

	NShape *firstTitle = this->sheetManager->GetTitle(0);
	float windowCloseX = firstTitle->GetX() + firstTitle->GetWidth() - 26 - 3; //24=�簢������,3=��������
	float windowCloseY = firstTitle->GetY() + 4;
	this->windowClose = new WindowClose(windowCloseX, windowCloseY, 26.0F, 23.0F, QColor(102, 204, 204),
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
		if (this->sheetManager != NULL) {
			delete this->sheetManager;
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

	DrawingPaper *canvas = dynamic_cast<DrawingPaper *>(this->windows[0]);
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

	this->sheetManager->Draw(visitor);
	//�ݱ�
	this->windowClose->Accept(visitor);

	this->painter->Render(&painter, 0, 0);
}

void FlowChartEditor::mouseMoveEvent(QMouseEvent *event) {
	//������ ��
	QRectF pinRect(this->windowClose->GetX(), this->windowClose->GetY(), this->windowClose->GetWidth(), this->windowClose->GetHeight());
	bool isContain = pinRect.contains(event->localPos());
	if (isContain == true) {
		this->windowClose->Paint(QColor(102, 255, 255), Qt::SolidLine, this->windowClose->GetBorderColor());
	}
	else {
		this->windowClose->Paint(QColor(102, 204, 204), Qt::SolidLine, this->windowClose->GetBorderColor());
	}
	this->repaint();
}

void FlowChartEditor::mouseReleaseEvent(QMouseEvent *event) {
	DrawingPaper *canvas = static_cast<DrawingPaper *>(this->windows[0]);
	QRectF pinRect(this->windowClose->GetX(), this->windowClose->GetY(), this->windowClose->GetWidth(), this->windowClose->GetHeight());
	bool isContain = pinRect.contains(event->localPos());
	if (isContain == true) {
		if (this->sheetManager->GetBinderLength() > 1) { //�� �� �̻��� ���� ���� �� ����.
			canvas->Close(); //���� ĵ���� �����ϰų� ���ϰų� ó������.
		}
		else {
			QMessageBox messageBox(QMessageBox::Warning, QString::fromLocal8Bit("���"),
				QString::fromLocal8Bit("��� ���� �� �����ϴ�."), QMessageBox::Ok, this);
			int ret = messageBox.exec();
		}
	}
	else {
		Long current = this->sheetManager->Select(event->pos());
		if (current >= 0) {
			this->sheetManager->Change(current);
			this->sheetManager->ModifyTitles();

			NShape *currentTitle = this->sheetManager->GetTitle(current);
			float windowCloseX = currentTitle->GetX() + currentTitle->GetWidth() - 26 - 3; //24=�簢������,3=��������
			float windowCloseY = currentTitle->GetY() + 4;
			this->windowClose->Move(windowCloseX, windowCloseY);
		}
	}
	this->repaint();
	canvas->repaint();
}

bool FlowChartEditor::eventFilter(QObject *o, QEvent *e) {
	if (o == this && e->type() == QEvent::ShortcutOverride) {
		e->accept();
	}
	return QFrame::eventFilter(o, e);
}

void FlowChartEditor::CommandRange(string text) { //���ڿ��� �ƴ� #define���� �����صΰ� ���� �� ȿ���� ������?
	FlowChartCommandFactory commandFactory(this);
	FlowChartCommand *command = commandFactory.Make(text); //action->text()
	if (command != NULL) {
		command->Execute();
		delete command;
	}
	DrawingPaper *drawingPaper = static_cast<DrawingPaper *>(this->windows[0]);

	QString mode = drawingPaper->GetCurrentMode();
	this->modeStatus->setText(mode);
	this->statusBar->repaint();

	this->repaint();
}

void FlowChartEditor::UpdateEditMenu() {
	DrawingPaper *canvas = static_cast<DrawingPaper *>(this->windows[0]);

	//����, �߶󳻱�, ����
	//���� ���°� �ƴϸ� ��Ȱ��ȭ�Ѵ�.
	if (canvas->mode != DrawingPaper::SELECT) {
		this->copyAction->setEnabled(false);
		this->cutAction->setEnabled(false);
		this->deleteAction->setEnabled(false);
		this->selectAllAction->setEnabled(false);
	}
	else {
		this->copyAction->setEnabled(true);
		this->cutAction->setEnabled(true);
		this->deleteAction->setEnabled(true);
		this->selectAllAction->setEnabled(true);
	}

	//�ٿ��ֱ�
	//Ŭ������ ���ۿ� �ƹ��͵� ������ ��Ȱ��ȭ�Ѵ�.
	(canvas->clipboard->GetLength() < 1) ?
		(this->pasteAction->setEnabled(false)) : (this->pasteAction->setEnabled(true));

	//���� ���
	//UndoMemory�� �ƹ��͵� ������ ��Ȱ��ȭ�Ѵ�.
	(canvas->historyController->GetUndoHistoryBook()->GetLength() < 1) ?
		(this->undoAction->setEnabled(false)) : (this->undoAction->setEnabled(true));

	//�ٽ� ����
	//RedoMemory�� �ƹ��͵� ������ ��Ȱ��ȭ�Ѵ�.
	(canvas->historyController->GetRedoHistoryBook()->GetLength() < 1) ?
		(this->redoAction->setEnabled(false)) : (this->redoAction->setEnabled(true));

	//��ȣ ��ġ ����, ��ȣ ũ�� ����, ��ȣ ���� ����
	//���õ� ��ȣ�� 1�� �ʰ��� Ȱ��ȭ�Ѵ�.
	Long(*indexes);
	Long count;
	canvas->flowChart->GetSelecteds(&indexes, &count);
	if (count <= 1) {
		this->positionAction->setEnabled(false);
		this->sizeAction->setEnabled(false);
		this->intervalAction->setEnabled(false);
	}
	else {
		this->positionAction->setEnabled(true);
		this->sizeAction->setEnabled(true);
		this->intervalAction->setEnabled(true);
	}
}

void FlowChartEditor::UpdateControlArchitectureMenu() {
	DrawingPaper *canvas = static_cast<DrawingPaper *>(this->windows[0]);

	//��������
	//���õ� ��ȣ�� 1�� �ʰ��� Ȱ��ȭ�Ѵ�.
	Long(*indexes);
	Long count;
	canvas->flowChart->GetSelecteds(&indexes, &count);
	if (count <= 1) {
		this->sequenceArchitectureAction->setEnabled(false);
	}
	else {
		this->sequenceArchitectureAction->setEnabled(true);
	}

	//�ݺ�����
	//���õ� ��ȣ�� 1�� �̻��̸鼭 ���õ� ù ��° ��ȣ�� �Ǵܱ�ȣ�̸� Ȱ��ȭ�Ѵ�.
	this->iterationArchitectureAction->setEnabled(false);
	if (count > 1) {
		NShape *shape = dynamic_cast<FlowChart *>(dynamic_cast<DrawingPaper *>(this->windows[0])->
			flowChart)->GetAt(indexes[0]);
		if (dynamic_cast<Decision *>(shape)) {
			this->iterationArchitectureAction->setEnabled(true);
		}
	}

	//���ñ���
	//���õ� ��ȣ�� 1�� �̻��̸鼭 ���õ� ù ��° ��ȣ�� �Ǵܱ�ȣ�̸鼭 �Ǵܱ�ȣ �� ���ʿ� ��ȣ�� �ϳ� �̻� ������ Ȱ��ȭ�Ѵ�.
	this->selectionArchitectureAction->setEnabled(false);
	if (count > 1) {
		NShape *shape = dynamic_cast<FlowChart *>(dynamic_cast<DrawingPaper *>(this->windows[0])->
			flowChart)->GetAt(indexes[0]);
		if (dynamic_cast<Decision *>(shape)) {
			Long leftCount = 0;
			Long i = 1;
			while (i < count) {
				NShape *left = dynamic_cast<FlowChart *>(dynamic_cast<DrawingPaper *>(this->windows[0])->flowChart)->GetAt(indexes[i]);
				if (left->CenterOfGravityX() < shape->CenterOfGravityX()) {
					leftCount++;
				}
				i++;
			}
			if (leftCount > 0) {
				this->selectionArchitectureAction->setEnabled(true);
			}
		}
	}


	if (indexes != 0) {
		delete[] indexes;
	}
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

	this->ruleKeepAction = new QAction(QString::fromLocal8Bit(("��Ģ �˻�(&K)")), this); //��Ģ �˻�(K)
	this->ruleKeepAction->setCheckable(true);
	this->ruleKeepAction->setChecked(true);
	connect(this->ruleKeepAction, &QAction::triggered, this, [=]() { this->CommandRange("RuleKeep"); });
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
	this->drawingModeAction->setCheckable(true);
	this->drawingModeAction->setChecked(false);
	connect(this->drawingModeAction, &QAction::triggered, this, [=]() { this->CommandRange("DrawingMode"); });

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
	this->helpAction = new QAction(QString::fromLocal8Bit(("����(&H)")), this); //����(H)
	connect(this->helpAction, &QAction::triggered, this, [=]() { this->CommandRange("Help"); });
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
	this->editMenu->addSeparator(); //���м�
	this->editMenu->addAction(this->ruleKeepAction); //��Ģ �˻�(K)
	connect(this->editMenu, &QMenu::aboutToShow, this, [=]() { this->UpdateEditMenu(); });

	this->formatMenu = this->menuBar->addMenu(QString::fromLocal8Bit(("����(&O)")));
	this->formatMenu->addAction(this->fontSetAction); //�۲�(F)...
	this->formatMenu->addAction(this->pageSetAction); //������ ����(U)...

	this->addMenu = this->menuBar->addMenu(QString::fromLocal8Bit(("����(&A)")));
	this->addMenu->addAction(this->drawingModeAction); //�׸��� ���(M) Ctrl + D
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
	connect(this->controlArchitectureMenu, &QMenu::aboutToShow, this, [=]() { this->UpdateControlArchitectureMenu(); });

	this->helpMenu = this->menuBar->addMenu(QString::fromLocal8Bit(("����(&H)")));
	this->helpMenu->addAction(this->helpAction); //����(H)
}

void FlowChartEditor::CreateStatusBar() {
	this->statusBar = new QStatusBar(this);

	this->messageStatus = new QLabel(QString::fromLocal8Bit(""));
	this->statusBar->addPermanentWidget(this->messageStatus, 5);

	this->modeStatus = new QLabel(QString::fromLocal8Bit("IDLE"));
	this->statusBar->addPermanentWidget(this->modeStatus, 2);

	this->xStatus = new QLabel(QString::fromLocal8Bit("X: "));
	this->statusBar->addPermanentWidget(this->xStatus, 2);

	this->yStatus = new QLabel(QString::fromLocal8Bit("Y: "));
	this->statusBar->addPermanentWidget(this->yStatus, 2);

	Long zoomRate = dynamic_cast<DrawingPaper *>(this->windows[0])->zoom->GetRate();
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