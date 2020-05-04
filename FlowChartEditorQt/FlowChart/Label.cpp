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
#include "../Notepad/Highlight.h"
#include "../Notepad/Editor.h"
#include "../Notepad/Selector.h"
#include "../Notepad/KeyActionFactory.h"
#include "../Notepad/KeyActions.h"
#include "InputOutput.h"

#include <qevent.h>
#include <windows.h>

Label *Label::instance = 0;

Label *Label::Instance(String *text, QColor color, QWidget *parent)
{
	if (instance == 0)
	{
		instance = new Label(text, color, parent);
	}
	return instance;
}

Label *Label::Instance(QWidget *parent)
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

	//���� ! " ' ( ) * + , - . / 0~9 < = > A~Z ^ _ a~z
	char buffer;
	Long number = 32;
	Long i = 0;
	while (number <= 122) {
		if ((number >= 32 && number <= 34) || (number >= 39 && number <= 57) || // ���鹮�� ~ " OR ' ~ 9 OR
			(number >= 60 && number <= 62) || (number >= 65 && number <= 90) || // < ~ > OR A ~ Z OR
			(number >= 94 && number <= 95) || (number >= 97 && number <= 122)) { // ^ ~ _ OR a ~ z
			buffer = number;
			this->allowedKeys[i++] = buffer;
		}
		number++;
	}

	this->isQuotes = false;
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

	//���� ! " ' ( ) * + , - . / 0~9 < = > A~Z ^ _ a~z
	char buffer;
	Long number = 32;
	Long i = 0;
	while (number <= 122) {
		if ((number >= 32 && number <= 34) || (number >= 39 && number <= 57) || // ���鹮�� ~ " OR ' ~ 9 OR
			(number >= 60 && number <= 62) || (number >= 65 && number <= 90) || // < ~ > OR A ~ Z OR
			(number >= 94 && number <= 95) || (number >= 97 && number <= 122)) { // ^ ~ _ OR a ~ z
			buffer = number;
			this->allowedKeys[i++] = buffer;
		}
		number++;
	}

	this->isQuotes = false;
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
	int key = event->key();

	KeyActionFactory keyActionFactory(this);
	KeyAction *keyAction = keyActionFactory.Make(key);

	DrawingPaper *drawingPaper = (DrawingPaper *)this->parentWidget();
	String text = event->text().toStdString();
	char *character = text;
	if (this->IsAllowed(*character) == true || keyAction != 0 || key == Qt::Key_Return) {
		Notepad::keyPressEvent(event);
		if (keyAction != 0) {
			delete keyAction;
		}
	}
#if 0
	int nChar = event->key();
	bool isControlPressed = ((::GetKeyState(VK_CONTROL) & 0x8000) != 0);
	DrawingPaper *drawingPaper = (DrawingPaper *)this->parentWidget();
	NShape *shape = drawingPaper->flowChart->GetAt(drawingPaper->indexOfSelected);
	//�غ��ȣ�� �ƴ� �� ó���Ѵ�.
	if (!(dynamic_cast<Preparation *>(shape)) && !isControlPressed) {
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
	DrawingPaper *drawingPaper = (DrawingPaper *)this->parentWidget();
	bool isQuotes = drawingPaper->ruleKeeper->GetIsQuotes();
	if (isQuotes == true) {
		Notepad::inputMethodEvent(event);
	}
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
	DrawingPaper *canvas = (DrawingPaper *)this->parentWidget();

	string content = this->note->GetContent();
	String contents(content);

	bool isKeptVariableRule = false;
	Long index = -1;
	bool isOkOperator = false;
	NShape *shape = canvas->flowChart->GetAt(canvas->indexOfSelected);
	
	//=====================intellisense========================
	if (dynamic_cast<Preparation *>(shape)) {
		isKeptVariableRule = canvas->ruleKeeper->IsKeptVariableRule(contents);
	}
	else {
		if (dynamic_cast<InputOutput*>(shape)) {
			//Read�� Print�� 5�� ����� �� : 
			//Read�� Read�ڿ� �������, Print�� Print�� ����� �Ǳ� ����.
			contents.Delete(0, 5);
		}
		index = canvas->ruleKeeper->FindVariable(contents);
		isOkOperator = canvas->ruleKeeper->CorrectOperator(contents);
	}

	if (isKeptVariableRule == true || (index != -1 && isOkOperator == true)) {
		shape->Rewrite(contents);
		this->Destroy();
		canvas->label = NULL;
	}
	else {
		if (this->highlight == NULL) {
			this->highlight = new Highlight;
			this->editor->selector = new Selector(this);

			Long index = this->note->Last();
			this->current = this->note->GetAt(index);
			this->current->Last();
			
			Long i = 0;
			while (i <= index) {
				Glyph *line = this->note->GetAt(i);
				Long startColumn = 0;
				Long endColumn = line->GetLength();
				this->editor->selector->Right(i, startColumn, endColumn);
				i++;
			}
		}
		this->setFocus();
	}
	//=========================================================
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

bool Label::IsAllowed(char key) {
	bool isAllowed = true;
	if (this->isQuotes == false) {
		Long i = 0;
		while (i < ALLOWEDKEYCOUNT && key != this->allowedKeys[i]) {
			i++;
		}
		if (key == 34 || key == 39) {
			this->isQuotes = true;
		}
		else if (i >= ALLOWEDKEYCOUNT) {
			isAllowed = false;
		}

	}

	return isAllowed;
}