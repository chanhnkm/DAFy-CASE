#include "HistoryWriter.h"
#include "HistoryBook.h"
#include "Historys.h"
#include "FlowChart.h"
#include "SelectingTool.h"

HistoryWriter::HistoryWriter() {
	this->undoHistoryBook = new HistoryBook;
	this->redoHistoryBook = new HistoryBook;
}

HistoryWriter::HistoryWriter(const HistoryWriter& source) {
	this->undoHistoryBook = new HistoryBook(*source.undoHistoryBook);
	this->redoHistoryBook = new HistoryBook(*source.redoHistoryBook);
}

HistoryWriter::~HistoryWriter() {
	if (this->undoHistoryBook != NULL) {
		delete this->undoHistoryBook;
	}
	if (this->redoHistoryBook != NULL) {
		delete this->redoHistoryBook;
	}
}

HistoryWriter& HistoryWriter::operator=(const HistoryWriter& source) {
	this->undoHistoryBook = new HistoryBook(*source.undoHistoryBook);
	this->redoHistoryBook = new HistoryBook(*source.redoHistoryBook);

	return *this;
}
#if 0
void HistoryWriter::Undo() {
	//=========���� ����ϸ� ���������� �����ߴ� �͸� �ǵ�����.===================
	History *History = this->undoHistoryBook->GetAt(this->undoHistoryBook->GetLength() - 1);

	//1. ó���ߴ� ��ȣ�� ������ŭ �ݺ��ϴ�.
	Long i = History->GetLength() - 1;
	while (i >= 0) {
		//1.1. ���� ��ȣ�� ��ġ�� ���ϴ�.
		if (dynamic_cast<AddHistory*>(History)) { //1.2. �����ߴ� ó���� '�߰�'������ �����.
			//���� �� ������ positions���� ������ positions ���� �� ���� shape�� ���� ����� �� ū positions�� ������.
			//NShape *shape = History->GetShape(i);
			//Long index = dynamic_cast<FlowChart*>(this->drawingPaper->flowChart)->Find(shape);
			Long index = History->GetPosition(i);
			this->drawingPaper->flowChart->Detach(index);
		}
		else if (dynamic_cast<RemoveHistory*>(History)) { //1.3. �����ߴ� ó���� '����'������ ���� ��ȣ�� �߰��Ѵ�.
			Long position = History->GetPosition(History->GetLength() - (i + 1)); //���ؾ��Ҷ��� �տ�����
			if (position > this->drawingPaper->flowChart->GetLength()) {
				position = this->drawingPaper->flowChart->GetLength();
			}
			NShape *shape = History->GetShape(History->GetLength() - (i + 1));
			this->drawingPaper->flowChart->Insert(position, shape->Clone());
			/*
			//�߰��� �� ����ó��(DrawingTool LbuttonUp ����)
			if (this->drawingPaper->templateSelected != NULL) {
				delete this->drawingPaper->templateSelected;
				this->drawingPaper->templateSelected = NULL;
			}
			this->drawingPaper->mode = DrawingPaper::SELECT;
			this->drawingPaper->tool = SelectingTool::Instance();
			*/
		}
		else if (dynamic_cast<OtherHistory*>(History)) { //1.4. �����ߴ� ó���� '����'�̾����� ���� ��ȣ�� ġȯ�Ѵ�.
			Long position = History->GetPosition(i);
			NShape *cloneShape = History->GetShape(i);
			NShape *shape = dynamic_cast<FlowChart*>(this->drawingPaper->flowChart)->GetAt(position);
			shape->Move(cloneShape->GetX(), cloneShape->GetY());
			shape->ReSize(cloneShape->GetWidth(), cloneShape->GetHeight());
			shape->Rewrite(cloneShape->GetContents());
			/*
			//������ �� ����ó��(MovingTool, ResizingTool LButtonUp ����)
			this->drawingPaper->mode = DrawingPaper::SELECT;
			this->drawingPaper->tool = SelectingTool::Instance();
			*/
		}
		this->undoHistoryBook->Quadrate(this->drawingPaper->flowChart);
		i--;
	}
	//ó���� ������ �����ش�. (���⼭ History�� �Ҹ��� ȣ��)
	this->undoHistoryBook->Remove(this->undoHistoryBook->GetLength() - 1);
}

void HistoryWriter::Redo() {
	//=========�ٽ� �����ϸ� ���������� ���� ����ߴ� �͸� �ٽ� �����Ѵ�.===================
	History *History = this->redoHistoryBook->GetAt(this->redoHistoryBook->GetLength() - 1);
	//1. ó���ߴ� ��ȣ�� ������ŭ �ݺ��ϴ�.
	Long i = History->GetLength() - 1;
	while (i >= 0) {
		//1.1. ���� ��ȣ�� ��ġ�� ���ϴ�.
		if (dynamic_cast<AddHistory*>(History)) { //1.2. ���� ����ߴ� ó���� '�߰�'������ ���� ��ȣ�� �߰��Ѵ�.
			Long position = History->GetPosition(History->GetLength() - (i + 1)); //���ؾ��Ҷ��� �տ�����
			if (position > dynamic_cast<FlowChart*>(this->drawingPaper->flowChart)->GetLength()) {
				position = dynamic_cast<FlowChart*>(this->drawingPaper->flowChart)->GetLength();
			}
			NShape *shape = History->GetShape(History->GetLength() - (i + 1));
			this->drawingPaper->flowChart->Insert(position, shape->Clone());
		}
		else if (dynamic_cast<RemoveHistory*>(History)) { //1.3. �����ߴ� ó���� '����'������ �����Ѵ�.
			//NShape *shape = History->GetShape(i);
			//Long index = dynamic_cast<FlowChart*>(this->drawingPaper->flowChart)->Find(shape);
			Long index = History->GetPosition(i);
			this->drawingPaper->flowChart->Detach(index);
		}
		else if (dynamic_cast<OtherHistory*>(History)) { //1.4. �����ߴ� ó���� '����'�̾����� ���� ��ȣ�� ġȯ�Ѵ�.
			Long position = History->GetPosition(i);
			NShape *cloneShape = History->GetShape(i);
			NShape *shape = this->drawingPaper->flowChart->GetAt(position);
			shape->Move(cloneShape->GetX(), cloneShape->GetY());
			shape->ReSize(cloneShape->GetWidth(), cloneShape->GetHeight());
			shape->Rewrite(cloneShape->GetContents());
		}
		this->redoHistoryBook->Quadrate(this->drawingPaper->flowChart);
		i--;
	}
	//ó���� ������ �����ش�. (���⼭ History�� �Ҹ��� ȣ��)
	this->redoHistoryBook->Remove(this->redoHistoryBook->GetLength() - 1);
}
#endif
Long HistoryWriter::RememberAdd(NShape (*shapes), Long count) {
	//FlowChart�� Attach �� ��ȣ�� �߰��� ó�� ���Ŀ� ȣ���ϱ�
	History *history = new AddHistory;
	Long i = 0;
	while (i < count) {
		history->Add(shapes[i].Clone());
		i++;
	}
	Long index = this->undoHistoryBook->Add(history);

	//undoHistoryBook�� Redo�� �ƴ� �ٸ� ó���� ���� �߰��� ��� redoHistoryBook�� ��������.
	if (this->redoHistoryBook != 0) {
		this->redoHistoryBook->RemoveAll();
	}

	return index;
}

Long HistoryWriter::RememberRemove(NShape (*shapes), Long count) {
	//FlowChart�� Erase �� ��ȣ�� ���� ó�� ������ ȣ���ϱ�
	History *history = new RemoveHistory;
	Long i = 0;
	while (i < count) {
		history->Add(shapes[i].Clone());
		i++;
	}
	Long index = this->undoHistoryBook->Add(history);

	//undoHistoryBook�� Redo�� �ƴ� �ٸ� ó���� ���� �߰��� ��� redoHistoryBook�� ��������.
	if (this->redoHistoryBook != 0) {
		this->redoHistoryBook->RemoveAll();
	}

	return index;
}

Long HistoryWriter::RememberOther(NShape (*shapes), Long count) {
	//FlowChart�� Move, ReSize, ReWirte �� ��ȣ�� ������ ó�� ������ ȣ���ϱ�
	History *history = new OtherHistory;
	Long i = 0;
	while (i < count) {
		history->Add(shapes[i].Clone());
		i++;
	}
	Long index = this->undoHistoryBook->Add(history);

	//undoHistoryBook�� Redo�� �ƴ� �ٸ� ó���� ���� �߰��� ��� redoHistoryBook�� ��������.
	if (this->redoHistoryBook != 0) {
		this->redoHistoryBook->RemoveAll();
	}

	return index;
}

Long HistoryWriter::RememberRedo() {
	Long index;
	History *history = this->redoHistoryBook->GetAt(this->redoHistoryBook->GetLength() - 1);
	if (dynamic_cast<OtherHistory*>(history)) { //redoHistoryBook�� ������ History�� other�̸�
		//�ش� History�� ������ �ִ� flowChart������ ��ġ�� �ش��ϴ� ��ȣ���� ����(redo ��) ������ ���� History�� �����. 
		History *otherHistory = new OtherHistory;
		Long i = 0;
		while (i < history->GetLength()) {
			Long position = history->GetPosition(i);
			NShape *shape = this->drawingPaper->flowChart->GetAt(position);
			otherHistory->Add(shape->Clone(), position);
			i++;
		}
		index = this->undoHistoryBook->Add(otherHistory);
	}
	else { //add or remove : clone ����
		index = this->undoHistoryBook->Add(history->Clone());
	}
	return index;
}

Long HistoryWriter::RememberUndo() {
	Long index;
	History *history = this->undoHistoryBook->GetAt(this->undoHistoryBook->GetLength() - 1);
	if (dynamic_cast<OtherHistory*>(history)) { //undoHistoryBook�� ������ History�� other�̸�
		//�ش� History�� ������ �ִ� flowChart������ ��ġ�� �ش��ϴ� ��ȣ���� ����(undo ��) ������ ���� History�� �����. 
		History *otherHistory = new OtherHistory;
		Long i = 0;
		while (i < history->GetLength()) {
			Long position = history->GetPosition(i);
			NShape *shape = this->drawingPaper->flowChart->GetAt(position);
			otherHistory->Add(shape->Clone(), position);
			i++;
		}
		index = this->redoHistoryBook->Add(otherHistory);
	}
	else { //add or remove : clone ����
		index = this->redoHistoryBook->Add(history->Clone());
	}
	return index;
}

void HistoryWriter::ChangeHistoryBook(HistoryBook *undoHistoryBook, HistoryBook *redoHistoryBook) {
	this->undoHistoryBook = undoHistoryBook;
	this->redoHistoryBook = redoHistoryBook;
}