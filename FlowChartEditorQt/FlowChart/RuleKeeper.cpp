#include "RuleKeeper.h"
#include "VariableList.h"

#include <qchar.h>

RuleKeeper::RuleKeeper()
	: variableList() {
	// ! " ' ( ) * + , - . / 0~9 < = > A~Z ^ _ a~z
	char buffer;
	Long number = 33;
	Long i = 0;
	while (number <= 122) {
		if ((number >= 33 && number <= 34) || (number >= 39 && number <= 57) || // ! ~ " OR ' ~ 9 OR
			(number >= 60 && number <= 62) || (number >= 65 && number <= 90) || // < ~ > OR A ~ Z OR
			(number >= 94 && number <= 95) || (number >= 97 && number <= 122)) { // ^ ~ _ OR a ~ z
			buffer = number;
			this->allowedKeys[i++] = buffer;
		}
		number++;
	}

	this->isQuotes = false;
}

RuleKeeper::RuleKeeper(const RuleKeeper& source)
	: variableList(source.variableList) {
	Long i = 0;
	while (i < 78) {
		this->allowedKeys[i] = source.allowedKeys[i];
		i++;
	}

	this->isQuotes = source.isQuotes;
}

RuleKeeper::~RuleKeeper() {
	if (this->variableList != 0) {
		delete this->variableList;
	}
}

RuleKeeper& RuleKeeper::operator=(const RuleKeeper& source) {
	this->variableList = source.variableList;

	Long i = 0;
	while (i < 78) {
		this->allowedKeys[i] = source.allowedKeys[i];
		i++;
	}

	this->isQuotes = source.isQuotes;

	return *this;
}

bool RuleKeeper::IsAllowed(char key) {
	bool isAllowed = true;
	if (this->isQuotes == false) {
		Long i = 0;
		while (i < 77 && key != this->allowedKeys[i]) {
			i++;
		}
		if (i >= 77) {
			isAllowed = false;
		}
	}

	return isAllowed;
}

bool RuleKeeper::IsKeptVariableRule(String allContents) {
	//���� ���ڿ� ���� ���ڸ� �����Ѵ�.
	allContents.Remove(' ');
	allContents.Remove('\n');

	bool isKept = true;

	VariableList tempList;
	char character;
	Long contentsLength = allContents.GetLength();
	String variable;

	//�ӽ� ���� ����� �����.
	Long i = 0;
	while (i < contentsLength) {
		character = allContents.GetAt(i);
		while (character != ',' && character != '=' && i < contentsLength) {
			variable.Store(variable.GetLength(), character);
			character = allContents.GetAt(++i);
		}
		tempList.Add(variable);
		if (character == '=') {
			while (character != ',' && i < contentsLength) {
				character = allContents.GetAt(++i);
			}
		}
		i++;
	}
	//�ӽ� ���� ����� ��Ģ�� �˸��� �������� ������ �ִ��� Ȯ���Ѵ�.
	//��Ģ�� ��߳� ������ �ϳ��� ������ ������� ���� ���̴�.
	Long j = 0;
	while (j < tempList.GetLength()) {
		variable = tempList.GetAt(j);
		character = variable.GetAt(0);
		//���� ������ ù ��° ���ڰ� �����ڰ� �ƴϸ� ��Ģ�� ��߳���.
		if (!((character >= 65 && character <= 90) || (character >= 97 && character <= 122))) {
			isKept = false;
		}
		//���� ������ ù ��° ���ڰ� ���� �빮���̸� ��� �빮�ڰ� �ƴϸ� ��Ģ�� ��߳���.(��ȣ���)
		Long k = 1;
		while (k < variable.GetLength() && (character >= 97 && character <= 122)) {
			character = variable.GetAt(k);
			k++;
		}
		if (k < variable.GetLength()) { //�빮�ڰ� �ƴ� ���ڸ� ã������
			isKept = false;
		}
		//���� ������ _�� ������ Ư�����ڰ� ������(�������̰ų� �����̰ų� _�̸� ��) ��Ģ�� ��߳���.
		k = 1;
		while (k < variable.GetLength() &&
			((character >= 65 && character <= 90) || (character >= 97 && character <= 122) ||
			(character >= 48 && character <= 57) ||
				(character == 95))) {
			character = variable.GetAt(k);
			k++;
		}
		if (k < variable.GetLength()) { //_�� ������ Ư�����ڸ� ã������
			isKept = false;
		}

		j++;
	}

	//��Ģ�� �°� ������ ���������� ���� ����� ����Ѵ�.
	if (isKept == true) {
		this->variableList = &tempList;
		this->variableList->SetKeywords();
	}

	return isKept;
}

Long RuleKeeper::FindVariable(String allContents) {
	allContents.Remove(' ');
	allContents.Remove('\n');

	Long index = 0;

	VariableList tempList;
	char character;
	Long contentsLength = allContents.GetLength();
	String variable;

	//�ӽ� ���� ����� �����.
	Long i = 0;
	while (i < contentsLength) {
		character = allContents.GetAt(i);
		//������, ����, _ �� ���� �ݺ��Ѵ�.
		while (i < contentsLength &&
			((character >= 65 && character <= 90) || (character >= 97 && character <= 122) ||
			(character >= 48 && character <= 57) ||
				(character == 95))) {
			variable.Store(variable.GetLength(), character);
			character = allContents.GetAt(i++);
		}
		//!!!���� ���� �ʵ��� �ؾ� �Ѵ�!!!
		tempList.Add(variable); 
		//���� ����� �� ���� �� �ƴϸ� ���� �ҹ��ڸ� ã�´�.
		while (i < contentsLength &&
			(character < 97 || character > 122)) {
			character = allContents.GetAt(i++);
		}

		i++;
	}

	//�ϳ��� ���°� ������ �ȵ�.
	Long j = 0;
	while (j < tempList.GetLength() && index != -1) {
		variable = tempList.GetAt(j);
		index = this->variableList->Find(variable);

		j++;
	}

	return index;
}

bool RuleKeeper::CorrectOperator(String allContents) {
	//'��' '��' '��'
	allContents.Remove(' ');
	allContents.Remove('\n');

	bool isCorrect = true;

	char character;
	Long contentsLength = allContents.GetLength();
	String writtenOperator;

	Long i = 0;
	while (i < contentsLength) {
		character = allContents.GetAt(i);
		// ! * - + = < > / ^ �� ��ȣ���� ��쿡�� �����ڸ� �����.
		if (character == '!' || character == '*' || character == '-' || character == '+' ||
			character == '=' || character == '<' || character == '>' || character == '/' ||
			character == '^') {
			writtenOperator.Store(writtenOperator.GetLength(), character);
		}
		else{
			writtenOperator.Clear();
		}

		if (writtenOperator.GetLength() > 1) {
			if (writtenOperator == "<=") {
				allContents.Delete(i - 1, 2);
				allContents.Store(i - 1, '��');
				isCorrect = true;
			}
			else if (writtenOperator == ">=") {
				allContents.Delete(i - 1, 2);
				allContents.Store(i - 1, '��');
				isCorrect = true;
			}
			else if (writtenOperator == "!=") {
				allContents.Delete(i - 1, 2);
				allContents.Store(i - 1, '��');
				isCorrect = true;
			}
			else if (writtenOperator == "<>") {
				isCorrect = true;
			}
			else {
				isCorrect = false;
			}
		}

		i++;
	}


	return isCorrect;
}