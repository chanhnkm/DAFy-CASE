#ifndef _FLOWCHARTEDITOR_H
#define _FLOWCHARTEDITOR_H

#include <QtWidgets/QFrame>
#include <qstring.h>
#include "ui_FlowChartEditor.h"
#include "FlowChart/Array.h"
#include <iostream>
using namespace std;

#define INVALID_SPACE 5

class DrawingPaper;
class FlowChartTemplate;
//class StatusBar;
//class ToolTip;
class QMenuBar;
class QMenu;
class QAction;
class GObject;
class SketchBook;
class NShape;
class QStatusBar;
class QLabel;

class FlowChartEditor : public QFrame
{
	Q_OBJECT

public:
	FlowChartEditor(QWidget *parent = Q_NULLPTR);
private:
	FlowChartEditorUi::Ui::FlowChartEditorClass ui;

public:
	Array<QFrame*> windows;
	QMenuBar *menuBar;
	GObject *painter;
	SketchBook *sketchBook;
	NShape *windowClose;

	//===Status Bar
	QStatusBar *statusBar;
	QLabel *symbolStatus;
	QLabel *modeStatus;
	QLabel *xStatus;
	QLabel *yStatus;
	QLabel *zoomStatus;
	QLabel *adStatus1;
	QLabel *adStatus2;
	//===Status Bar

	//bool isUnModeMenuEnabled; //�޴� ��Ʈ�� ����
	//StatusBar *statusBar;
	//ToolTip *toolTip;
private:
	//HACCEL hAccel; //Accelerator ����Ű
protected:
	void closeEvent(QCloseEvent *event);
	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	bool eventFilter(QObject* o, QEvent* e);
	/* �ش��ϴ� �Լ� ã��
	BOOL PreTranslateMessage(MSG *pMsg);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnCommandRange(UINT uID);
	*/
	/* �޴� ��Ʈ�ѿ� ���� ó��
	afx_msg void OnUpdateDrawingUnModeCommand(CCmdUI *cCmdUI);
	afx_msg void OnUpdateDrawingModeCommand(CCmdUI *cCmdUI);
	afx_msg void OnUpdateCopyCutDeleteCommand(CCmdUI *cCmdUI);
	afx_msg void OnUpdatePasteCommand(CCmdUI *cCmdUI);
	afx_msg void OnUpdateUndoCommand(CCmdUI *cCmdUI);
	afx_msg void OnUpdateRedoCommand(CCmdUI *cCmdUI);
	afx_msg void OnUpdatePositionCommand(CCmdUI *cCmdUI);
	afx_msg void OnUpdateSizeCommand(CCmdUI *cCmdUI);
	afx_msg void OnUpdateIntervalCommand(CCmdUI *cCmdUI);
	afx_msg void OnUpdateSequenceCommand(CCmdUI *cCmdUI);
	afx_msg void OnUpdateIterationCommand(CCmdUI *cCmdUI);
	afx_msg void OnUpdateSelectionCommand(CCmdUI *cCmdUI);
	*/

	//�޴�
private slots:
	void CommandRange(string text);

private:
	void CreateActions();
	void CreateMenus();

	QMenu *fileMenu; //����(F)
	QAction *newAction; //�� ����(N) Ctrl + N
	QAction *openAction; //����(O) Ctrl + O
	QAction *saveAction; //����(S) Ctrl + S
	QAction *saveAsAction; //�ٸ� �̸����� ����(A)... Ctrl + Alt + S
	QAction *saveAsImageAction; //�̹���(JPG)�� ����(I)... Ctrl + Alt + I
	QAction *printAction; //�μ�(P)... Ctrl + P
	QAction *exitAction; //������(X) Alt + F4

	QMenu *editMenu; //����(E)
	QAction *undoAction; //���� ���(U) Ctrl + Z
	QAction *redoAction; //�ٽ� ����(R) Ctrl + Y
	QAction *copyAction; //�����ϱ�(C) Ctrl + C
	QAction *pasteAction; //�ٿ��ֱ�(P) Ctrl + V
	QAction *cutAction; //�߶󳻱�(T) Ctrl + X
	QAction *deleteAction; //����(L) Del
	QAction *selectAllAction; //��� ����(A) Ctrl + A
	QAction *positionAction; //��ȣ ��ġ ����(O)
	QAction *sizeAction; //��ȣ ũ�� ����(S)
	QAction *intervalAction; //��ȣ ���� ����(I)

	QMenu *formatMenu; //����(O)
	QAction *fontSetAction; //�۲�(F)...
	QAction *pageSetAction; //������ ����(U)...

	QMenu *addMenu; //����(A)
	QAction *drawingModeAction; //�׸��� ���(M) Ctrl + D
	QAction *drawingUnModeAction; //�׸��� ��� ���� ESC
	QAction *startTerminalSymbolAction; //���� �ܸ� ��ȣ(S)
	QAction *preparationSymbolAction; //�غ� ��ȣ(P)
	QAction *inputSymbolAction; //�Է� ��ȣ(I)
	QAction *processSymbolAction; //ó�� ��ȣ(R)
	QAction *decisionSymbolAction; //�Ǵ� ��ȣ(D)
	QAction *outputSymbolAction; //��� ��ȣ(O)
	QAction *stopTerminalSymbolAction; //���� �ܸ� ��ȣ(T)

	QMenu *controlArchitectureMenu; //�����(C)
	QAction *sequenceArchitectureAction; //��������(S)
	QAction *iterationArchitectureAction; //�ݺ�����(I)
	QAction *selectionArchitectureAction; //���ñ���(E)

	QMenu *helpMenu; //����(H)

	void CreateStatusBar();
};

#endif _FLOWCHARTEDITOR_H