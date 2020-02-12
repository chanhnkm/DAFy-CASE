#ifndef _FLOWCHARTEDITOR_H
#define _FLOWCHARTEDITOR_H

#include <QtWidgets/QFrame>
#include "ui_FlowChartEditor.h"
#include "FlowChart/Array.h"

#define INVALID_SPACE 5

class DrawingPaper;
class FlowChartTemplate;
class FlowChartFont;
class StatusBar;
class ToolTip;

class FlowChartEditor : public QFrame
{
	Q_OBJECT

public:
	FlowChartEditor(QWidget *parent = Q_NULLPTR);
private:
	Ui::FlowChartEditorClass ui;

public:
	Array<QFrame*> windows;
	//CMenu menu;
	QString fileOpenPath;
	FlowChartFont *font; //QFont�� �ذ� ������ ��
	//bool isUnModeMenuEnabled; //�޴� ��Ʈ�� ����
	StatusBar *statusBar;
	ToolTip *toolTip;
private:
	//HACCEL hAccel; //Accelerator ����Ű
protected:
	void closeEvent(QCloseEvent *event);
	void resizeEvent(QResizeEvent *event);
	/* �ش��ϴ� �Լ� ã��
	BOOL PreTranslateMessage(MSG *pMsg);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
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
};

#endif _FLOWCHARTEDITOR_H