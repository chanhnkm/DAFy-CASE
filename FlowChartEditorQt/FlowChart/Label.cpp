/******************************************************************
 * ���� �̸� :  Label.cpp
 * ��     �� : Text Class boundary
 * ��  �� �� : ����â
 * �ۼ� ���� : 2015�� 4�� 20��
*******************************************************************/
#include "Label.h"
#include "String.h"
#include "Shape.h"

#include "FlowChart.h"
#include "../FlowChartEditor.h"
#include "DrawingPaper.h"

#include "../Notepad/GlyphFactory.h"
#include "../Notepad/Glyph.h"
#include "../Notepad/Row.h"
#include "../Notepad/CharacterMetrics.h"
#include "Process.h"
#include "Creator.h"
#include "SizeController.h"
#include "VariableList.h"
#include "Preparation.h"
#include "../Notepad/GlyphFactory.h"
#include "../GObject/Font.h"
#include "RuleKeeper.h"

#include <qevent.h>
#include <windows.h>

Label* Label::instance = 0;

Label* Label::Instance(String *text, QColor color, QWidget *parent)
{
	if (instance == 0)
	{
		instance = new Label(text, color, parent);
	}
	return instance;
}

Label* Label::Instance(QWidget *parent)
{
	if (instance == 0)
	{
		instance = new Label(parent);
	}
	return instance;
}


Label::Label(QWidget *parent)
	: Notepad(parent) {
	this->x = 0.0F;
	this->y = 0.0F;
	this->width = 0.0F;
	this->height = 0.0F;
	this->sizeController = NULL;
	this->color = QColor(255, 255, 255);

	this->sizeController = new SizeController(this);
}

Label::Label(String *text, QColor color, QWidget *parent)
	: Notepad(text, parent) {
	this->x = 0.0F;
	this->y = 0.0F;
	this->width = 0.0F;
	this->height = 0.0F;
	this->sizeController = NULL;
	this->color = color;

	this->sizeController = new SizeController(this);
}

Label::~Label() {
	if (this->sizeController != NULL) {
		delete this->sizeController;
	}
}

void Label::Destroy() {
	if (instance != 0)
	{
		delete instance;
	}
	instance = 0;
}

void Label::Open(float x, float y, float width, float height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void Label::resizeEvent(QResizeEvent *event) {
	Notepad::resizeEvent(event);
}

void Label::keyPressEvent(QKeyEvent *event) {
	SHORT isCtrl = GetKeyState(VK_CONTROL) & 0X8000;
	SHORT isShift = GetKeyState(VK_SHIFT) & 0X8000;

	int key = event->key();

	bool isKeyAct = false;
	if ((isShift && isCtrl && key == Qt::Key_Left) || (isShift && isCtrl && key == Qt::Key_Right) ||
		(isShift && isCtrl && key == Qt::Key_Home) || (isShift && isCtrl && key == Qt::Key_End) ||
		(isShift && key == Qt::Key_Left) || (isShift && key == Qt::Key_Right) ||
		(isShift && key == Qt::Key_Up) || (isShift && key == Qt::Key_Down) ||
		(isShift && key == Qt::Key_Home) || (isShift && key == Qt::Key_End) ||
		(isCtrl && key == Qt::Key_Left) || (isCtrl && key == Qt::Key_Right) ||
		(isCtrl && key == Qt::Key_Home) || (isCtrl && key == Qt::Key_End) ||
		(isCtrl && key == Qt::Key_A) || (isCtrl && key == Qt::Key_C) || 
		(isCtrl && key == Qt::Key_V) || (isCtrl && key == Qt::Key_X) ||
		(key == Qt::Key_Left) || (key == Qt::Key_Right) ||
		(key == Qt::Key_Up) || (key == Qt::Key_Down) ||
		(key == Qt::Key_Home) || (key == Qt::Key_End) ||
		(key == Qt::Key_Delete) || (key == Qt::Key_Backspace)) {
		isKeyAct = true;
	}

	DrawingPaper *drawingPaper = (DrawingPaper*)this->parentWidget();
	char character = event->text().at(0).combiningClass();
	if (drawingPaper->ruleKeeper->IsAllowed(character) == true  || isKeyAct == true) {
		Notepad::keyPressEvent(event);
	}
#if 0
	int nChar = event->key();
	bool isControlPressed = ((::GetKeyState(VK_CONTROL) & 0x8000) != 0);
	DrawingPaper *drawingPaper = (DrawingPaper*)this->parentWidget();
	NShape *shape = drawingPaper->flowChart->GetAt(drawingPaper->indexOfSelected);
	//�غ��ȣ�� �ƴ� �� ó���Ѵ�.
	if (!(dynamic_cast<Preparation*>(shape)) && !isControlPressed) {
		bool isMustCheck = false;
		//������ �ԷµǸ� ������ ó���Ѵ�.
		if ((nChar >= Qt::Key_A && nChar <= Qt::Key_Z) /*|| (nChar >= 97 && nChar <= 122)*/) {
			isMustCheck = true;
		}
		//���ڰ� �ԷµǸ� ���� �� ���� �̻��� �ԷµǾ� �־���� �ϰ� �� ���� ���ڰ� �����̸� ó���Ѵ�.
		else if ((nChar >= Qt::Key_0 && nChar <= Qt::Key_9) && this->current->GetCurrent() > 1) {
			char previous = *(this->current->GetAt(this->current->GetCurrent() - 2)->GetContent().c_str());
			if ((previous >= Qt::Key_A && previous <= Qt::Key_Z) /*|| (previous >= 97 && previous <= 122)*/) {
				isMustCheck = true;
			}
		}
		if (isMustCheck == true) {
			//���� ��ġ�� �ش��ϴ� �ܾ �����ϱ�
			String variable;

			Glyph *line = this->current->Clone();
			Long startIndex = line->MovePreviousWord();
			Long endIndex = line->MoveNextWord();
			Long i = startIndex;
			char character = *(line->GetAt(i++)->GetContent().c_str());
			variable += character;
			while (i < endIndex && ((character >= Qt::Key_A && character <= Qt::Key_Z) /*|| (character >= 97 && character <= 122)*/ ||
				(variable.GetLength() > 0 && (character >= Qt::Key_0 && character <= Qt::Key_9)))) {
				character = *(line->GetAt(i)->GetContent().c_str());
				variable += character;
				i++;
			}
			Long ret = drawingPaper->variableList->Find(variable);
			character = *(line->GetAt(startIndex)->GetContent().c_str());
			if (ret == -1 && (character != Qt::Key_Apostrophe && character != Qt::Key_QuoteDbl)) { //����ǥ �ڿ� �� ���ڸ� ����
				Long removeIndex = this->current->GetCurrent() - 1;
				if (removeIndex < 0) {
					removeIndex = 0;
				}
				this->current->Remove(removeIndex);
				BOOL result = PlaySound((LPCWSTR)"sound_button_wrong0.2.wav", NULL, SND_FILENAME);
			}
		}
	}
#endif
}

void Label::inputMethodEvent(QInputMethodEvent *event) {
	Notepad::inputMethodEvent(event);
}

void Label::paintEvent(QPaintEvent *event) {
	QRect frameRect = this->frameRect();
	this->painter->Resize(frameRect.width(), frameRect.height(), this->color);
	Notepad::paintEvent(event);
}

void Label::focusInEvent(QFocusEvent *event) {
	Notepad::focusInEvent(event);
}

void Label::focusOutEvent(QFocusEvent *event) {
	Notepad::focusOutEvent(event);

	//19.09.03 Label�� (������)������ ��ȣ ���� ���� �����ͷ� �ִ� ó��==================
	DrawingPaper *canvas = (DrawingPaper*)this->parentWidget();

	string content = this->note->GetContent();
	String contents(content);

	NShape *shape = canvas->flowChart->GetAt(canvas->indexOfSelected);
	//=====================intellisense========================
	if (dynamic_cast<Preparation*>(shape)) {
		if (canvas->variableList != NULL) {
			delete canvas->variableList;
		}
		canvas->variableList = new VariableList;
		canvas->variableList->Add(shape->GetContents());
	}
	//=========================================================

	shape->Rewrite(contents);

	this->Destroy();
}

void Label::mousePressEvent(QMouseEvent *event) {
	if (this->sizeController != NULL) {
		delete this->sizeController;
		this->sizeController = NULL;
	}

	Notepad::mousePressEvent(event);

	if (this->sizeController == NULL) {
		this->sizeController = new SizeController(this);
	}
}

void Label::mouseMoveEvent(QMouseEvent *event) {
	Notepad::mouseMoveEvent(event);
}