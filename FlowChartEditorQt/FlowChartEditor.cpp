#include "FlowChartEditor.h"

FlowChartEditor::FlowChartEditor(QWidget *parent)
	: QFrame(parent)
{
	ui.setupUi(this);

	this->font = NULL;
	this->statusBar = NULL;
	this->toolTip = NULL;

	QRect frameRect = this->frameRect();

	/*�ڽ� ������ ���� - ��ġ�� ũ��, ��Ÿ�� ����
	this->windows.Store(0, new DrawingPaper);
	this->windows.Store(1, new FlowChartTemplate);
	this->windows.Store(2, NULL);

	//this->windows[0]->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS, rect, this, 1000);
	//this->windows[1]->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS, rect, this, 2000);
	*/

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
	QRect frameRect1 = this->frameRect();
	QRect frameRect2 = this->frameRect();

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
		Shape *shape = dynamic_cast<FlowChart*>(dynamic_cast<DrawingPaper*>(this->windows[0])->
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
		Shape *shape = dynamic_cast<FlowChart*>(dynamic_cast<DrawingPaper*>(this->windows[0])->
			flowChart)->GetAt(indexes[0]);
		if (dynamic_cast<Decision*>(shape)) {
			Long leftCount = 0;
			Long i = 1;
			while (i < count) {
				Shape *left = dynamic_cast<FlowChart*>(dynamic_cast<DrawingPaper*>(this->windows[0])->flowChart)->GetAt(indexes[i]);
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