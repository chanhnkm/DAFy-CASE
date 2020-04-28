/******************************************************************
* ���� �̸� : NShape.h
* ��� : ����(��ȣ,��)�� �߻�ȭ Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 30��
*******************************************************************/

#ifndef _NSHAPE_H
#define _NSHAPE_H

#include "String.h" //path ����

#include <qpoint.h>
#include <qcolor.h>
#include <qpen.h>
#include <qcursor.h>

class ScrollController;

// ��ȣ ���� ��ũ�ε�

#define ID_TERMINAL				40001
#define ID_PREPARATION			40002
#define ID_PUNCHEDCARD		40003
#define ID_PROCESS				40004
#define ID_DECISION				40005
#define ID_DOCUMENT			40006
#define ID_ARROW					40007
#define ID_LEFTDOWN				40008
#define ID_RIGHTDOWN			40009
#define ID_JOIN						40010
#define ID_REPEATTRUE			40011
#define ID_REPEATFALSE			40012
#define ID_RIGHTDOWNJOIN	40013
#define ID_INPUTOUTPUT     	40014

enum SHAPE {
	ALL = 0, SYMBOL = 1, LINE = 2, TERMINAL = 4, PREPARATION = 8, PUNCHEDCARD = 16, PROCESS = 32, DECISION = 64,
	DOCUMENT = 128, INPUTOUTPUT = 256, ARROW = 512, LEFTDOWN = 1024, RIGHTDOWN = 2048,
	JOIN = 4096, REPEATTRUE = 8196, REPEATFALSE = 16384, RIGHTDOWNJOIN = 32768
};

enum ATTRIBUTE { POINT_IN, POINT_OUT, POINT_TRUE, POINT_FALSE };

enum DECISION_ { NONE = 0, SELECTION = 1, ITERATION = 2 };

enum Direction { RIGHT = 0, LEFT = 1 };
enum PenStyle { SOLID, DASH, DOT, DASHDOT, DASHDOTDOT, INSIEDFRAME };

// ���� ���� ��ũ�ε�
#define HIT_NONE				   0
#define HIT_BODY					   1
#define HIT_TOPLEFT			       2
#define HIT_TOPMIDDLE		       3
#define HIT_TOPRIGHT			   4
#define HIT_BOTTOMLEFT		   5
#define HIT_BOTTOMMIDDLE	   6
#define HIT_BOTTOMRIGHT	   7
#define HIT_LEFTMIDDLE		   8
#define HIT_RIGHTMIDDLE		   9
#define HIT_IN                       10
#define HIT_OUT                    11
#define HIT_HANDLE               12
#define HIT_TRUE                   13
#define HIT_FALSE                  14

// ************* ����Ʈ ������ �ʿ��Ѱ� ? ********************
// �Ӽ� ����Ʈ ��
#define BACKGROUNDCOLOR 10
#define BORDERLINE 0
#define BORDERCOLOR 10
#define FONT_HEIGHT 12

// ��ȣ ����Ʈ ��
#define REPEATFALSE_WIDTH 30
#define REPEATFALSE_HEIGHT 40

#define REPEATTRUE_WIDTH 30
#define REPEATTRUE_HEIGHT 30

#define RIGHTDOWNJOIN_WIDTH 30
#define RIGHTDOWNJOIN_HEIGHT 30

#define ANNOTATION_WIDHT 30
#define DIRECTION RIGHT

// ȭ��ǥ
#define ARROW_SIZE 5

// ���εβ�
#define LINETHICKNESS 3

// ���� �ڽ�
#define BOXLENGTH 6 // �ڽ��� �׵θ� ����
#define MARKLENGTH 5 // ��ĥ�� ���� �ڽ� ����
#define BOXSCOPE 7 // ������ �ڽ� ���� ���� CRgn ���� �׵θ� ������ ��ã�� ����

// ���ùڽ� �ִ� ����
#define SELECTIONMARK 8

// �� �ʺ� ����
#define FORMWIDTH 100
#define FORMHEIGHT 30

#define BOXVERTECIES 4 // Attribute ���� ���

class GObject;
class FlowChartVisitor;

class Attribute {
public:
	Attribute();
	~Attribute();

	Attribute(const Attribute& source);
	Attribute& operator = (const Attribute& source);

	bool IsEqual(const Attribute& other);
	bool IsNotEqual(const Attribute& other);

	bool operator == (const Attribute& other);
	bool operator != (const Attribute& other);

	char GetVertexIn() const;
	char GetVertexOut() const;
	char GetVertexTrue() const;
	char GetVertexFalse() const;
	QPointF GetPointIn() const;
	QPointF GetPointOut() const;
	QPointF GetPointTrue() const;
	QPointF GetPointFalse() const;

public:
	char vertexIn;
	char vertexOut;
	char vertexTrue;
	char vertexFalse;
	QPointF pointIn;
	QPointF pointOut;
	QPointF pointTrue;
	QPointF pointFalse;
};

inline char Attribute::GetVertexIn() const {
	return this->vertexIn;
}

inline char Attribute::GetVertexOut() const {
	return this->vertexOut;
}

inline char Attribute::GetVertexTrue() const {
	return this->vertexTrue;
}

inline char Attribute::GetVertexFalse() const {
	return this->vertexFalse;
}

inline QPointF Attribute::GetPointIn() const {
	return this->pointIn;
}

inline QPointF Attribute::GetPointOut() const {
	return this->pointOut;
}

inline QPointF Attribute::GetPointTrue() const {
	return this->pointTrue;
}

inline QPointF Attribute::GetPointFalse() const {
	return this->pointFalse;
}

class NShape {
public:
	NShape();
	NShape(float x, float y, float width, float height,
		QColor backGroundColor = QColor(255, 255, 255), Qt::PenStyle borderLine = Qt::SolidLine,
		QColor borderColor = QColor(0, 0, 0), String contents = static_cast<String>(""));

	virtual ~NShape() = 0;

	NShape(const NShape& source);
	NShape& operator=(const NShape& source);

	virtual bool IsEqual(const NShape& other);
	virtual bool IsNotEqual(const NShape& other);

	Long IsUpperThan(const NShape& other);
	Long IsDownThan(const NShape& other);
	Long IsFrontThan(const NShape& other);
	Long IsRearThan(const NShape& other);

	virtual bool operator ==(const NShape& other);
	virtual bool operator !=(const NShape& other);

	void Move(float x, float y);
	virtual void ReSize(float width, float height);
	void Rewrite(char(*text));

	void Paint(QColor backGroundColor, Qt::PenStyle borderLine, QColor borderColor); //���� �Ӽ� ����

	virtual void DrawActiveShape(GObject *painter); // ���� ���� ���ýô� �̵��� �ʿ������� ���ùڽ��� �׸��� �ʴ´�.

	// Visitor ���� ����
	virtual void Accept(FlowChartVisitor *draw) = 0;
	// Prototype ���� ����
	virtual NShape* Clone() = 0;

	//���콺 �巡�׷� ���� ���� �ѹ��� ���ý� ���
	virtual QRegion GetRegion() = 0;
	virtual QRegion GetRegion(Long thickness) = 0;

	virtual QRegion GetSelectionMarkerAllRegion();

	virtual void GetAttribute(Attribute *Attribute) {};

	float GetX() const;
	float GetY() const;
	float GetWidth() const;
	float GetHeight() const;
	QColor& GetBackGroundColor() const;
	Qt::PenStyle GetBorderLine() const;
	QColor& GetBorderColor() const;

	String& GetContents() const;

	float GetLeft() const;
	float GetTop() const;
	float GetRight() const;
	float GetBottom() const;

	// ���� ���� 
	virtual bool IsIncluded(QPointF point) { return false; };
	virtual bool IsIncluded(const QRectF& rect) { return false; };

	virtual void Select(bool selected);
	virtual bool IsSelected() const;

	virtual int GetHitCode(QPointF point);
	virtual int GetHitCode(const QPointF& point, const QRegion& region);

	virtual QCursor GetCursor(int hit) const; //QCursor class ����

	virtual void Copy(NShape *object);

	virtual void GetLine(char(*line)) {};

	virtual float CenterOfGravityY() const;
	virtual float CenterOfGravityX() const;

	virtual bool Identify(SHAPE identify) { return false; };
	virtual bool IsStyle(Long style);

	int GetSymbolID(); //190903 DrawingPaper.LButtonDown���� Creator.Create ����ϱ� ����
	int GetLineID(); //191227 Interpreter pattern ���� ��
public:
	static void MakeRectToPoint(QPointF point, QRectF *rect);

protected:
	virtual void DrawSelectionMarkers(GObject *painter, ScrollController *scrollController); //painter, scroll ���� �� ��ġ��
	virtual void GetSelectionMarkerRect(int marker, QRectF *rect); //marker - ��ó�� ����� ��ũ��: int�� ��������.

public: //Block virtual ����
	virtual Long Attach(NShape *shape);
	virtual Long Insert(Long index, NShape *shape);
	virtual Long Detach(Long index);
	virtual Long Detach(SHAPE identify);
	virtual bool DetachSelectedAll();
	virtual NShape* GetAt(Long index);

	virtual Long Find(NShape* shape);
	virtual Long Find(QPointF point);
	virtual Long Find(float x, float y);
	virtual Long Find(QRectF rect);
	virtual Long Find(SHAPE identify);
	virtual Long Find(Long fromIndex, SHAPE identify);
	virtual void Find(Long styles, Long* (*indexes), Long *count);

	virtual void Swap(Long toIndex, Long fromIndex);
	virtual void Swap(Long toIndex, NShape *shape);
	virtual void Clear();

	virtual void SelectAll();
	virtual void UnSelectAll();
	virtual void GetSelecteds(Long* (*indexes), Long *count);
	virtual Long CountSelecteds();
	virtual void AscendingSort();
	virtual void DescendingSort();

	virtual Long GetCapacity() const;
	virtual Long GetLength() const;
	virtual Long GetCurrent() const;
protected:
	float x;
	float y;
	float width;
	float height;
	QColor backGroundColor;
	Qt::PenStyle borderLine;
	QColor borderColor;
	String contents;

	bool isSelected;
};

inline float NShape::GetX() const {
	return this->x;
}

inline float NShape::GetY() const {
	return this->y;
}

inline float NShape::GetWidth() const {
	return this->width;
}

inline float NShape::GetHeight() const {
	return this->height;
}

inline QColor& NShape::GetBackGroundColor() const {
	return const_cast<QColor&>(this->backGroundColor);
}

inline Qt::PenStyle NShape::GetBorderLine() const {
	return this->borderLine;
}

inline QColor& NShape::GetBorderColor() const {
	return const_cast<QColor&>(this->borderColor);
}

inline String& NShape::GetContents() const {
	return const_cast<String&>(this->contents);
}

inline float NShape::GetLeft() const {
	return this->x;
}
inline float NShape::GetTop() const {
	return this->y;
}

inline float NShape::GetRight() const {
	return (this->x + this->width);
}

inline float NShape::GetBottom() const {
	return (this->y + this->height);
}

inline bool NShape::IsSelected() const {
	return this->isSelected;
}

inline float NShape::CenterOfGravityY() const {
	return this->y + this->height / 2.0f;
}

inline float NShape::CenterOfGravityX() const {
	return this->x + this->width / 2.0f;
}

inline bool NShape::IsStyle(Long style) {
	return (style == ALL) ? (true) : (false);
}


#endif // _NSHAPE_H