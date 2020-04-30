#include "VariableList.h"

VariableList::VariableList(Long capacity)
	: variables(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

VariableList::VariableList(const VariableList& source)
	: variables(source.variables) {
	this->capacity = source.capacity;
	this->length = source.length;
}

VariableList::~VariableList() {

}

VariableList& VariableList::operator=(const VariableList& source) {
	this->variables = source.variables;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

Long VariableList::Add(String contents) {
	Long index = -1;
	if (this->length < this->capacity) {
		index = this->variables.Store(this->length, contents);
	}
	else {
		index = this->variables.AppendFromRear(contents);
		this->capacity++;
	}
	this->length++;

	return index;
}

Long VariableList::Find(String contents) {
	//��� : contents �� num �̸� num ���� �����ϴ� �������� �ִ��� Ȯ���Ѵ�.
	Long ret = -1;
	Long i = 0;
	//1. ��������� ������ŭ �׸��� contents�� �����ϴ� �������� ���� ���� �ݺ��Ѵ�.
	while (i < this->length && this->variables[i].FindUntilLength(contents) == -1) {
		//���� �������� contents�� �����ϴ� �������� �ƴϸ� -1 ���
		i++;
	}
	if (i < this->length) {
		ret = i;
	}
	return ret;
}

String& VariableList::GetAt(Long index) {
	return this->variables.GetAt(index);
}

void VariableList::SetKeywords() {
	this->variables.Store(0, String("AND"));
	this->variables.Store(1, String("EOF"));
	this->variables.Store(2, String("FALSE"));
	this->variables.Store(3, String("NOT"));
	this->variables.Store(4, String("OR"));
	this->variables.Store(5, String("PRINT"));
	this->variables.Store(6, String("READ"));
	this->variables.Store(7, String("START"));
	this->variables.Store(8, String("STOP"));
	this->variables.Store(9, String("TRUE"));
	this->length += 10;
}