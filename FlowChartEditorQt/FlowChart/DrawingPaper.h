#ifndef _DRAWINGPAPER_H
#define _DRAWINGPAPER_H

// �˾�â
#define IDM_AE_SEQUENCE 10001
#define IDM_AE_SELECION 10002
#define IDM_AE_ITERATION 10003
#define IDM_AE_MOVEMAKE 10004
#define IDM_AE_SIZEMAKE 10005
#define IDM_AE_INTERVALMAKE 1006

#include <QtWidgets/QFrame>
#include "Array.h"
#include "Shape.h"
#include "../Notepad/Subject.h"

typedef unsigned long int DWORD;
typedef signed long int Long;

class Label;
class Tool;
class Painter;
class Label;
class Clipboard;

class ScrollController;
class HistoryController;
class Registrar;

class Zoom;
class QMenu;

class VariableList;

class DrawingPaper : public QFrame, public Subject
{
	Q_OBJECT

public:
	enum { IDLE, SELECT, DRAWING, MOVING, SIZING };

public:
	DrawingPaper(QWidget *parent = Q_NULLPTR);
	~DrawingPaper();


	void DrawSelectingArea();
	void DrawActiveShape(NShape *entity); // ������ ����� �� �ٸ���.
	void DrawActiveShape2(NShape *entity); // ������ ����� �� �ٸ���.

	void DrawSelectionMark(QPoint(*points), Long count);
	void DrawSelectionMark2(QPoint(*points), Long count);
	void FillSelectionMark(QPoint point);

	//void Move(Long xDistance, Long yDistance); //���� ���� ������.
	void New();
	Long Save(QString fileName);
	Long Load(QString fileName);
	void Close();
	QCursor GetCursor(QPoint point); //Ŀ��
	
	QString GetCurrentMode(); //200424 statusBar mode status

public:
	NShape *templateSelected;
	Painter *painter;

	//19.09.16 private���� public : ��ũ�� ó�� ����
	NShape *flowChart; // ����ڵ��� ���ٽ��� ����ϱ� ���ؼ� ������ �����
	ScrollController *scrollController;

	/////////////////////////////////////////////////////
	// ���콺 ����
	float startX;
	float startY;

	// ��� ������ ��
	float currentX;
	float currentY;
	/////////////////////////////////////////////////////
	Long indexOfSelected;

	int mode; //2019.09.20 MemoryController���� Undo ������ ���� �ű�. ���� �Ӽ��� ������ Ȥ�� private������ �Ƿ���?

	Label *label;
	Tool *tool;

	HistoryController *historyController;
	Registrar *registrar;

	Clipboard *clipboard;
	Zoom *zoom;
	NShape *a4Paper;

	QMenu *popup; //�˾��޴�(��Ŭ��)

	bool drawSelectingAreaFlag;

	QColor windowBorderColor;

	VariableList *variableList;
private:
	friend class Label;
	friend class FlowChartTemplate;

	friend class ResizingTool;
	friend class MovingTool;
	friend class DrawingTool;
	friend class SelectingTool;
	friend class ToolFactory;

	friend class SequenceMake;
	friend class IterationMake;
	friend class SelectionMake;

	friend class SizeMake;
	friend class IntervalMake;
	friend class MoveMake;

	friend class ErasingTool;

	friend class File;

	friend class KeyFactory;
	friend class DeleteKey;
	friend class UpKey;
	friend class DownKey;
	friend class LeftKey;
	friend class RightKey;
	friend class EscapeKey;

	friend class Clipboard;

private:
	int hitCode; //���� ���� ���� ����

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event); //���콺 ���� ��ư �ٿ�, ������(���ؽ�Ʈ�޴�)��?
	void mouseMoveEvent(QMouseEvent *event); //tracking ������ ����Ʈ�� ���콺 �ƹ��ų� ������ ���� ȣ���.
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void focusOutEvent(QFocusEvent *event);
	void focusInEvent(QFocusEvent *event);
public slots:
	void OnContextMenu(const QPoint &pos);
	
	void OnMoveMakeMenuClick();
	void OnSizeMakeMenuClick();
	void OnIntervalMakeMenuClick();
	void OnSequenceMenuClick();
	void OnIterationMenuClick();
	void OnSelectionMenuClick();
};

#endif  // _DRWAINGPAPER_H