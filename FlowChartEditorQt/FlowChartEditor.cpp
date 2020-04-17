#include "FlowChartEditor.h"
#include "FlowChart/FlowChartCommandFactory.h"
#include "FlowChart/FlowChartCommands.h"
#include "FlowChart/DrawingPaper.h"
#include "FlowChart/FlowChartTemplate.h"
#include "FlowChart/WindowTitle.h"

#include <qmenubar.h>
#include <qevent.h>
#include <qpainter.h>

FlowChartEditor::FlowChartEditor(QWidget *parent)
	: QFrame(parent)
{
	ui.setupUi(this);

	this->setMouseTracking(true);
	
	this->menuBar = NULL;

	QRect frameRect = this->frameRect();
	
	this->CreateActions();
	this->CreateMenus();
	this->menuBar->resize(frameRect.width(), this->menuBar->height());

	//�ڽ� ������ ���� - ��ġ�� ũ��, ��Ÿ�� ����
	DrawingPaper *drawingPaper = new DrawingPaper(this);
	drawingPaper->move(200, 1 + this->menuBar->height());
	this->windows.Store(0, drawingPaper);

	FlowChartTemplate *fTemplate = new FlowChartTemplate(this);
	fTemplate->move(5, 5 + this->menuBar->height());
	this->windows.Store(1, fTemplate);
	//this->windows.Store(2, NULL);

	//this->isUnModeMenuEnabled = FALSE; //�޴� ��Ʈ�ѿ� ���� �Ӽ� �ʱ�ȭ

	/*������
	HICON flowChartEditorIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	this->SetIcon(flowChartEditorIcon, TRUE);
	*/

	/*���� ǥ����
	HINSTANCE hInstance = (HINSTANCE)::GetWindowLong(this->m_hWnd, GWL_HINSTANCE);
	this->GetClientRect(rect);
	// Create the status bar.
	HWND hWndStatusBar = ::CreateWindowEx(0, STATUSCLASSNAME, (PCTSTR)NULL, SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 0, 0, this->m_hWnd, (HMENU)3000, hInstance, NULL);

	this->statusBar = new StatusBar(6, hWndStatusBar);

	int statusBars[7];
	statusBars[0] = rect.right - 700;
	statusBars[1] = statusBars[0] + 100;
	statusBars[2] = statusBars[1] + 100;
	statusBars[3] = statusBars[2] + 100;
	statusBars[4] = statusBars[3] + 100;
	statusBars[5] = statusBars[4] + 200;
	statusBars[6] = statusBars[5] + 100;
	// Tell the status bar to create the window parts.
	::SendMessage(this->statusBar->GetHandle(), SB_SETPARTS, (WPARAM)7, (LPARAM)statusBars);

	this->statusBar->Add(0, String(""));

	this->statusBar->Add(1, String(""));

	this->statusBar->Add(2, String("X: "));
	this->statusBar->Add(3, String("Y: "));
	CString rate;
	Long zoomRate = dynamic_cast<DrawingPaper*>(this->windows[0])->zoom->GetRate();
	rate.Format("%d", zoomRate);
	rate += "%";
	this->statusBar->Add(4, String((LPCTSTR)rate));

	this->statusBar->Add(5, String("���� : 02)587-9424"));
	this->statusBar->Add(6, String("���� ��Ī"));
	this->statusBar->Print();
	*/

	/*��� �޴�
	menu.LoadMenuA(IDR_MENU);
	SetMenu(&menu);
	*/

	//����ó���� ���õ� ���ε�
	//DragAcceptFiles(TRUE);

	//��Ʈ
	//this->font = new FlowChartFont(this);

	//Accelerators
	//this->hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));
}

void FlowChartEditor::closeEvent(QCloseEvent *event) {
	if (this->menuBar != NULL) {
		delete this->menuBar;
	}

	/*�ڽ� ������ �迭
	Long i = 0;
	while (i < this->windows.GetLength()) {
		if (this->windows[i] = NULL) {
			delete this->windows[i];
		}
		this->windows[i] = NULL;
		i++;
	}
	*/
	/*��Ʈ
	if (this->font != NULL) {
		delete this->font;
	}
	*/
	/*���� ǥ����
	if (this->statusBar != NULL) {
		delete this->statusBar;
	}
	*/
	/*����
	if (this->toolTip != NULL) {
		this->toolTip->Destroy();
	}
	*/
	//Accelerators
	//DestroyAcceleratorTable(this->hAccel);

	QFrame::closeEvent(event);
}

void FlowChartEditor::resizeEvent(QResizeEvent *event) {
	QRect frameRect = this->frameRect();

	this->menuBar->resize(frameRect.width(), this->menuBar->height());

	this->windows[0]->resize(frameRect.width() - 200, frameRect.height() - this->menuBar->height() + 1);
	this->windows[0]->repaint();
	
	Long height = frameRect.height() - this->menuBar->height() - 20;
	this->windows[1]->resize(190, frameRect.height() - this->menuBar->height() - 20);
	this->windows[1]->repaint();
	/*frameRect ���� ����?
	rect.left = 205;
	rect2.right = 200;
	*/

	/*���� ǥ����
	Long gap;
	WINDOWPLACEMENT sbPosition;
	int statusBars[7];
	HWND hWndStatus = this->statusBar->GetHandle();
	if (hWndStatus != NULL) {
		::GetWindowPlacement(hWndStatus, &sbPosition);
		gap = sbPosition.rcNormalPosition.bottom - sbPosition.rcNormalPosition.top;
		rect.bottom = rect.bottom - gap;
		rect2.bottom = rect2.bottom - gap;
		::MoveWindow(hWndStatus, 0, rect.bottom, rect.right, gap, FALSE);

		statusBars[0] = rect.right - 700;
		statusBars[1] = statusBars[0] + 100;
		statusBars[2] = statusBars[1] + 100;
		statusBars[3] = statusBars[2] + 100;
		statusBars[4] = statusBars[3] + 100;
		statusBars[5] = statusBars[4] + 200;
		statusBars[6] = statusBars[5] + 100;
		::SendMessage(hWndStatus, SB_SETPARTS, (WPARAM)7, (LPARAM)statusBars);
	}
	*/
	/*mfc���� ���� �ذ��ε�
	if (this->windows[0] != NULL && this->windows[1] != NULL) { //�ִ�ȭ�ؼ� ����ϴ� OnClose ���Ŀ� OnSize�� ȣ��.
		this->windows[1]->MoveWindow(rect2);
		this->windows[0]->MoveWindow(rect);
	}
	*/
}

void FlowChartEditor::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.fillRect(this->frameRect(), QColor(235, 235, 235));
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

	this->repaint();
}

void FlowChartEditor::CreateActions() {
	//this->fontSetAction->setStatusTip(tr("Set Font"));

	//==================== File Menu ====================
	this->newAction = new QAction(QString::fromLocal8Bit(("�� ����(&N)")), this);
	connect(this->newAction, &QAction::triggered, this, [=]() { this->CommandRange("New"); });

	this->openAction = new QAction(QString::fromLocal8Bit(("����(&O)")), this);
	connect(this->openAction, &QAction::triggered, this, [=]() { this->CommandRange("Open"); });

	this->saveAction = new QAction(QString::fromLocal8Bit(("����(&S)")), this);
	connect(this->saveAction, &QAction::triggered, this, [=]() { this->CommandRange("Save"); });

	this->saveAsAction = new QAction(QString::fromLocal8Bit(("�ٸ� �̸����� ����(&A)...")), this);
	connect(this->saveAsAction, &QAction::triggered, this, [=]() { this->CommandRange("SaveAs"); });

	this->saveAsImageAction = new QAction(QString::fromLocal8Bit(("�̹���(JPG)�� ����(&I)...")), this);
	connect(this->saveAsImageAction, &QAction::triggered, this, [=]() { this->CommandRange("SaveAsImage"); });

	this->printAction = new QAction(QString::fromLocal8Bit(("�μ�(&P)...")), this);
	connect(this->printAction, &QAction::triggered, this, [=]() { this->CommandRange("Print"); });

	this->exitAction = new QAction(QString::fromLocal8Bit(("������(&X)...")), this);
	connect(this->exitAction, &QAction::triggered, this, [=]() { this->CommandRange("Exit"); });
	//==================== File Menu ====================

	//==================== Edit Menu ====================
	this->undoAction = new QAction(QString::fromLocal8Bit(("���� ���(&U)")), this); //���� ���(U) Ctrl + Z
	connect(this->undoAction, &QAction::triggered, this, [=]() { this->CommandRange("Undo"); });

	this->redoAction = new QAction(QString::fromLocal8Bit(("�ٽ� ����(&R)")), this); //�ٽ� ����(R) Ctrl + Y
	connect(this->redoAction, &QAction::triggered, this, [=]() { this->CommandRange("Redo"); });

	this->copyAction = new QAction(QString::fromLocal8Bit(("�����ϱ�(&C)")), this); //�����ϱ�(C) Ctrl + C
	connect(this->copyAction, &QAction::triggered, this, [=]() { this->CommandRange("Copy"); });

	this->pasteAction = new QAction(QString::fromLocal8Bit(("�ٿ��ֱ�(&P)")), this); //�ٿ��ֱ�(P) Ctrl + V
	connect(this->pasteAction, &QAction::triggered, this, [=]() { this->CommandRange("Paste"); });

	this->cutAction = new QAction(QString::fromLocal8Bit(("�߶󳻱�(&T)")), this); //�߶󳻱�(T) Ctrl + X
	connect(this->cutAction, &QAction::triggered, this, [=]() { this->CommandRange("Cut"); });

	this->deleteAction = new QAction(QString::fromLocal8Bit(("����(&L)")), this); //����(L) Del
	connect(this->deleteAction, &QAction::triggered, this, [=]() { this->CommandRange("Delete"); });

	this->selectAllAction = new QAction(QString::fromLocal8Bit(("��� ����(&A)")), this); //��� ����(A) Ctrl + A
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
	connect(this->drawingModeAction, &QAction::triggered, this, [=]() { this->CommandRange("DrawingMode"); });

	this->drawingUnModeAction = new QAction(QString::fromLocal8Bit(("�׸��� ��� ����(&U)")), this); //�׸��� ��� ���� ESC
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