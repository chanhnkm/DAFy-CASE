#include "HistoryController.h"
#include "DrawingPaper.h"
#include "HistoryBook.h"
#include "Registrar.h"
#include "Historys.h"
#include "Block.h"

#include "../FlowChartEditor.h"
#include "Sheet.h"
#include "SheetBinder.h"
#include "SheetManager.h"

HistoryController::HistoryController(DrawingPaper *canvas) {
	this->canvas = canvas;
	this->undoHistoryBook = new HistoryBook;
	this->redoHistoryBook = new HistoryBook;
	this->previousFlowChart = this->canvas->flowChart->Clone();
	this->canvas->AttachObserver(this);
}

HistoryController::HistoryController(const HistoryController& source) {
	this->canvas = source.canvas;
	this->undoHistoryBook = new HistoryBook(*const_cast<HistoryController&>(source).undoHistoryBook);
	this->redoHistoryBook = new HistoryBook(*const_cast<HistoryController&>(source).redoHistoryBook);
	this->previousFlowChart = const_cast<HistoryController&>(source).previousFlowChart->Clone();
}

HistoryController::~HistoryController() {
	//HistoryBook, previousFlowChart ������ ��ü�� Sheet�̴�. ���� �Ҹ��Ű�� �ʴ´�.
	this->canvas->DetachObserver(this);
}

HistoryController& HistoryController::operator=(const HistoryController& source) {
	this->canvas = source.canvas;
	this->undoHistoryBook = source.undoHistoryBook->Clone();
	this->redoHistoryBook = source.redoHistoryBook->Clone();
	this->previousFlowChart = source.previousFlowChart->Clone();

	return *this;
}

void HistoryController::Update() {
	History *history = 0;

	Long i;
	NShape *shape;
	SHAPE id;
	Long registrationNumber;
	Long index;

	NShape *currentFlowChart = this->canvas->flowChart;
	Long previousLength = this->previousFlowChart->GetLength();
	Long currentLength = currentFlowChart->GetLength();
	//1. ���� �������� ������ ���� �������� �������� ������
	if (previousLength < currentLength) {
		//1.1.�߰� ���縦 �����.
		history = new AddHistory;
		//1.2.���� ������ ������ŭ �ݺ��ϴ�.
		i = 0;
		while (i < currentLength) {
			//1.2.1.shape�� ��������.
			shape = currentFlowChart->GetAt(i);
			id = shape->GetIdentify();
			registrationNumber = shape->GetRegistrationNumber();
			//1.2.2.���� ���������� ã��.
			index = this->previousFlowChart->FindByRegistrationNumber(id, registrationNumber);
			//1.2.3.��ã������ ���翡 �߰��ϴ�.
			if (index < 0) {
				history->Add(shape->Clone());
			}
			i++;
		}
	}
	//2. ���� �������� ������ ���� �������� �������� ������
	else if (previousLength > currentLength) {
		//2.1.���� ���縦 �����.
		history = new RemoveHistory;
		//2.2.���� ������ ������ŭ �ݺ��ϴ�.
		i = 0;
		while (i < previousLength) {
			//2.2.1.shape�� ��������.
			shape = this->previousFlowChart->GetAt(i);
			id = shape->GetIdentify();
			registrationNumber = shape->GetRegistrationNumber();
			//2.2.2.���� ���������� ã��.
			index = currentFlowChart->FindByRegistrationNumber(id, registrationNumber);
			//2.2.3.��ã������ ���翡 �߰��ϴ�.
			if (index < 0) {
				history->Add(shape->Clone());
			}
			i++;
		}
	}
	//3. ���� �������� ������ ���� �������� ������ ���� ���� �������� ���� �������� ���� ������
	else if (!(this->previousFlowChart->IsSame(*currentFlowChart))) {
		//3.1.��Ÿ ���縦 �����.
		history = new OtherHistory;
		//3.2.���� ������ ������ŭ �ݺ��ϴ�.
		i = 0;
		while (i < previousLength) {
			//3.2.1.shape�� ��������.
			shape = this->previousFlowChart->GetAt(i);
			id = shape->GetIdentify();
			registrationNumber = shape->GetRegistrationNumber();
			//3.2.2.���� ���������� ã��.
			index = currentFlowChart->FindByRegistrationNumber(id, registrationNumber);
			//3.2.3.shape�� ã�� shape�� �������� ������ shape�� ���翡 �߰��ϴ�.
			if (shape != currentFlowChart->GetAt(index)) {
				history->Add(shape->Clone());
			}
			i++;
		}
	}
	if (history != 0) {
		//4. ���� ��� ����å�� ���縦 �߰��ϴ�.
		this->undoHistoryBook->Add(history);
		//5. �ٽ� ���� ����å�� ����.
		if (this->redoHistoryBook->GetLength() > 0) {
			this->redoHistoryBook->RemoveAll();
		}
		//6. ���� �������� ���� �������� �ٲٴ�.
		this->previousFlowChart = currentFlowChart->Clone();
		FlowChartEditor *editor = static_cast<FlowChartEditor*>(this->canvas->parentWidget());
		editor->sheetManager->ModifyPreviousFlowChart(this->previousFlowChart);
	}
}

void HistoryController::Undo() {
	Long i;
	NShape *shape;
	Long index = -1;
	SHAPE id;
	Long registrationNumber;
	NShape *temp;

	//1. ���� ��� ����å�� ������ ���縦 ��������.
	Long historyLength = this->undoHistoryBook->GetLength();
	History *lastHistory = this->undoHistoryBook->GetAt(historyLength - 1);
	//2. ������ shape ������ŭ �ݺ��ϴ�.
	i = 0;
	while (i < lastHistory->GetLength()) {
		//2.1.shape�� ��������.
		shape = lastHistory->GetShape(i);
		id = shape->GetIdentify();
		registrationNumber = shape->GetRegistrationNumber();
		//2.2.�߰� �����
		if (dynamic_cast<AddHistory*>(lastHistory)) {
			//2.2.1.shape�� ĵ������ ���������� ã��.
			index = this->canvas->flowChart->FindByRegistrationNumber(id, registrationNumber);
			//2.2.2.ã�� shape�� ĵ������ ���������� �����.
			this->canvas->flowChart->Detach(index);
		}
		//2.3.���� �����
		else if (dynamic_cast<RemoveHistory*>(lastHistory)) {
			//2.3.1.shape�� ĵ������ �������� �߰��ϴ�.
			this->canvas->flowChart->Attach(shape->Clone());
		}
		//2.4.��Ÿ �����
		else if (dynamic_cast<OtherHistory*>(lastHistory)) {
			//2.4.1.shape�� ĵ������ ���������� ã��.
			index = this->canvas->flowChart->FindByRegistrationNumber(id, registrationNumber);
			//2.4.2.ã�� shape�� shape�� ���� �ٲٴ�.
			temp = shape->Clone();
			lastHistory->Modify(i, this->canvas->flowChart->GetAt(index)->Clone());
			this->canvas->flowChart->Swap(index, temp);
		}
		i++;
	}
	if (historyLength > 0) {
		//3. �ٽ� ���� ����å�� ���縦 �߰��ϴ�.
		this->redoHistoryBook->Add(lastHistory->Clone());
		//4. ���� ��� ����å���� ���縦 �����.
		this->undoHistoryBook->Remove(historyLength - 1);

		this->previousFlowChart = this->canvas->flowChart->Clone();
		FlowChartEditor *editor = static_cast<FlowChartEditor*>(this->canvas->parentWidget());
		editor->sheetManager->ModifyPreviousFlowChart(this->previousFlowChart);
	}
}

void HistoryController::Redo() {
	Long i;
	NShape *shape;
	Long index = -1;
	SHAPE id;
	Long registrationNumber;
	NShape *temp;

	//1. �ٽ� ���� ����å�� ������ ���縦 ��������.
	Long historyLength = this->redoHistoryBook->GetLength();
	History *lastHistory = this->redoHistoryBook->GetAt(historyLength - 1);
	//2. ������ shape ������ŭ �ݺ��ϴ�.
	i = 0;
	while (i < lastHistory->GetLength()) {
		//2.1.shape�� ��������.
		shape = lastHistory->GetShape(i);
		id = shape->GetIdentify();
		registrationNumber = shape->GetRegistrationNumber();
		//2.2.�߰� �����
		if (dynamic_cast<AddHistory*>(lastHistory)) {
			//2.2.1.shape�� ĵ������ �������� �߰��ϴ�.
			this->canvas->flowChart->Attach(shape->Clone());
		}
		//2.3.���� �����
		else if (dynamic_cast<RemoveHistory*>(lastHistory)) {
			//2.3.1.shape�� ĵ������ ���������� ã��.
			index = this->canvas->flowChart->FindByRegistrationNumber(id, registrationNumber);
			//2.3.2.ã�� shape�� ĵ������ ���������� �����.
			this->canvas->flowChart->Detach(index);
		}
		//2.4.��Ÿ �����
		else if (dynamic_cast<OtherHistory*>(lastHistory)) {
			//2.4.1.shape�� ĵ������ ���������� ã��.
			index = this->canvas->flowChart->FindByRegistrationNumber(id, registrationNumber);
			//2.4.2.ã�� shape�� shape�� ���� �ٲٴ�.
			temp = shape->Clone();
			lastHistory->Modify(i, this->canvas->flowChart->GetAt(index)->Clone());
			this->canvas->flowChart->Swap(index, temp);
		}
		i++;
	}
	if (historyLength > 0) {
		//3. ���� ��� ����å�� ���縦 �߰��ϴ�.
		this->undoHistoryBook->Add(lastHistory->Clone());
		//4. �ٽ� ���� ����å���� ���縦 �����.
		this->redoHistoryBook->Remove(historyLength - 1);

		this->previousFlowChart = this->canvas->flowChart->Clone();
		
		FlowChartEditor *editor = static_cast<FlowChartEditor*>(this->canvas->parentWidget());
		editor->sheetManager->ModifyPreviousFlowChart(this->previousFlowChart);
	}
}

void HistoryController::ChangeAll(HistoryBook *undoHistoryBook, HistoryBook *redoHistoryBook, 
	NShape *previousFlowChart) {
	this->undoHistoryBook = undoHistoryBook;
	this->redoHistoryBook = redoHistoryBook;
	this->previousFlowChart = previousFlowChart;
}