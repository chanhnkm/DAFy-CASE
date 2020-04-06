/******************************************************************
* ���� �̸� :  Label.h
* ��     �� : Text Class boundary
* ��  �� �� : ����â
* �ۼ� ���� : 2015�� 4�� 20�� 
*******************************************************************/
#ifndef _LABEL_H
#define _LABEL_H

#include "../Notepad/Notepad.h"

typedef signed long int Long;
typedef unsigned long int ULong;

class Text;
class String;
class Caret;
class Shape;
class SizeController;

class Label : public Notepad {
public:
	static Label* Instance();
	static Label* Instance(String *text);
	static void Destroy();
	void Open(Long x, Long y, Long width, Long height, String* text);

	Long GetX() const;
	Long GetY() const;
	Long GetWidth() const;
	Long GetHeight() const;

protected:
	Label();
	Label(String *text, COLORREF color);
	~Label();

public:
	SizeController *sizeController;
private:
	// ��ġ�� ũ��
	Long x;
	Long y;
	Long width;
	Long height;

	COLORREF color;

	// �ٰ���

	// �ѱ� �Է� ����
	//BOOL isHangulInputing;

private:
	static Label *instance;
protected:
	afx_msg void OnKillFocus( CWnd* pNewWnd );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint(); 
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnImeStartComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeChar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

inline Long Label::GetX() const {
	return this->x;
}
inline Long Label::GetY() const {
	return this->y;
}
inline Long Label::GetWidth() const {
	return this->width;
}
inline Long Label::GetHeight() const {
	return this->height;
}

#endif //_LABEL_H