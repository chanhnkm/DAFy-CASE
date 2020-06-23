#include "ScrollController.h"
#include "../Interface/Scroll.h"
#include "../Interface/DrawingPaper.h"

#include <qDebug.h>

ScrollController::ScrollController(DrawingPaper *drawingPaper) 
	: scrolls(2) {
	this->drawingPaper = drawingPaper;

	this->height = this->drawingPaper->a4Paper->GetY() * 2 + this->drawingPaper->a4Paper->GetHeight();
	this->width = this->drawingPaper->a4Paper->GetX() * 2 + this->drawingPaper->a4Paper->GetWidth();

	QRect rect = this->drawingPaper->frameRect();
	Long clientWidth = rect.width();
	Long clientHeight = rect.height();

	Scroll *verticalScroll = new Scroll(Qt::Vertical, this->drawingPaper);
	verticalScroll->SetFromConstructorFlag(true); //�����ڿ��� Scroll::Move ȣ��Ǹ� �ȵ�
	//maximum : height - ȭ��ũ��
	verticalScroll->setMaximum(this->height - clientHeight);
	//minimum : 0
	verticalScroll->setMinimum(0);
	//pageStep : ȭ�� ����
	verticalScroll->setPageStep(clientHeight);
	//singleStep : ������ �۾� ����
	verticalScroll->setSingleStep(100);
	//value : 0
	verticalScroll->setValue(0);
	verticalScroll->SetFromConstructorFlag(false); //�����ڿ��� Scroll::Move ȣ��Ǹ� �ȵ�
	this->scrolls.Store(this->scrolls.GetLength(), verticalScroll);

	Scroll *horizontalScroll = new Scroll(Qt::Horizontal, this->drawingPaper);
	horizontalScroll->SetFromConstructorFlag(true); //�����ڿ��� Scroll::Move ȣ��Ǹ� �ȵ�
	//maximum : height - ȭ��ũ��
	horizontalScroll->setMaximum(this->width - clientWidth);
	//minimum : 0
	horizontalScroll->setMinimum(0);
	//pageStep : ȭ�� ����
	horizontalScroll->setPageStep(clientWidth);
	//singleStep : ������ �۾� ����
	horizontalScroll->setSingleStep(150);
	//value : 0
	horizontalScroll->setValue(0);
	horizontalScroll->SetFromConstructorFlag(false); //�����ڿ��� Scroll::Move ȣ��Ǹ� �ȵ�
	this->scrolls.Store(this->scrolls.GetLength(), horizontalScroll);

	this->drawingPaper->AttachObserver(this);
}

ScrollController::ScrollController(const ScrollController& source)
	: scrolls(source.scrolls) {
	Long i = 0;
	while (i < 2) {
		Scroll *scroll = this->scrolls.GetAt(i);
		scroll = new Scroll(*scroll);
		this->scrolls.Modify(i, scroll);
		i++;
	}

	this->drawingPaper = source.drawingPaper;
	this->width = source.width;
	this->height = source.height;
}

ScrollController::~ScrollController() {
	Long i = 0;
	while (i < 2) {
		delete this->scrolls[i];
		i++;
	}

	this->drawingPaper->DetachObserver(this);
}

ScrollController& ScrollController::operator=(const ScrollController& source) {
	Long i = 0;
	while (i < 2) {
		delete this->scrolls[i];
		i++;
	}

	this->scrolls = source.scrolls;
	i = 0;
	while (i < 2) {
		Scroll *scroll = this->scrolls.GetAt(i);
		scroll = new Scroll(*scroll);
		this->scrolls.Modify(i, scroll);
		i++;
	}

	this->drawingPaper = source.drawingPaper;
	this->width = source.width;
	this->height = source.height;

	return *this;
}

void ScrollController::Update() {
	Long minimum;
	Long maximum;
	Long pageSize;
	Long lineSize;
	Long position;
	QRect clientRect = this->drawingPaper->frameRect();

	//=====================���� ��ũ�� ����========================
	//1. ��ü ������ ���̸� ���ϴ�.
	this->height = this->drawingPaper->a4Paper->GetY() * 2 + this->drawingPaper->a4Paper->GetHeight();
	//2. Ŭ���̾�Ʈ ������ ���̸� ���ϴ�.
	Long clientHeight = clientRect.height();
	//3. Ŭ���̾�Ʈ ���� ���̰� ��ü ���� ���̺��� ������ ��ũ���� �����Ѵ�.
	if (clientHeight < this->height) {
		minimum = this->scrolls.GetAt(0)->minimum();
		maximum = this->height - clientHeight;
		pageSize = clientHeight;
		lineSize = 100;
		position = this->scrolls.GetAt(0)->value();
		if (this->scrolls.GetAt(0) != NULL) {
			delete this->scrolls.GetAt(0);
			this->scrolls.Delete(0);
		}
		this->scrolls.Insert(0, new Scroll(Qt::Vertical, this->drawingPaper)); //setValue->Move->paintEvent->Visit->GetScroll(1) ����: Delete�ؼ� 1��°�� ����.
		Scroll *scroll = this->scrolls.GetAt(0);
		scroll->setMinimum(minimum);
		scroll->setMaximum(maximum);
		scroll->setPageStep(pageSize);
		scroll->setSingleStep(lineSize);
		scroll->setValue(position);
		
		Long right = this->drawingPaper->frameRect().right()+1;
		scroll->resize(20, this->drawingPaper->height());
		scroll->move(right - 20, scroll->y());
		scroll->show();
	}
	else {
		if (this->scrolls.GetAt(0) != 0) {
			delete this->scrolls.GetAt(0);
			this->scrolls.Delete(0);
		}
	}

	//=====================���� ��ũ�� ����========================
	//1. ��ü ������ �ʺ� ���ϴ�.
	this->width = this->drawingPaper->a4Paper->GetX() * 2 + this->drawingPaper->a4Paper->GetWidth();
	//2. Ŭ���̾�Ʈ ������ �ʺ� ���ϴ�.
	Long clientWidth = clientRect.width();
	//3. Ŭ���̾�Ʈ ���� �ʺ� ��ü ���� �ʺ񺸴� ������ ��ũ���� �����Ѵ�.
	if (clientWidth < this->width) {
		minimum = this->scrolls.GetAt(1)->minimum();
		maximum = this->width - clientWidth;
		pageSize = clientWidth;
		lineSize = 150;
		position = this->scrolls.GetAt(1)->value();
		if (this->scrolls.GetAt(1) != NULL) {
			delete this->scrolls.GetAt(1);
			this->scrolls.Delete(1);
		}
		this->scrolls.Insert(1, new Scroll(Qt::Horizontal, this->drawingPaper));
		Scroll *scroll = this->scrolls.GetAt(1);
		scroll->setMinimum(minimum);
		scroll->setMaximum(maximum);
		scroll->setPageStep(pageSize);
		scroll->setSingleStep(lineSize);
		scroll->setValue(position);

		Long bottom = this->drawingPaper->frameRect().bottom() + 1;
		scroll->resize(this->drawingPaper->width() - 20, 20);
		scroll->move(scroll->x(), bottom - 20);
		scroll->show();
	}
	else {
		if (this->scrolls.GetAt(1) != 0) {
			delete this->scrolls.GetAt(1);
			this->scrolls.Delete(1);
		}
	}

	this->drawingPaper->repaint();
}

Long ScrollController::Rotate(QPoint delta) {
	Scroll *verticalScroll = this->scrolls.GetAt(0);
	Long value = verticalScroll->value();
	Long singleStep = verticalScroll->singleStep();
	value -= delta.ry() / 120 * singleStep;

	Long minimum = verticalScroll->minimum();
	Long maximum = verticalScroll->maximum();
	if (value < minimum) {
		value = minimum;
	}
	else if (value > maximum) {
		value = maximum;
	}

	verticalScroll->setValue(value);

	return value;
}