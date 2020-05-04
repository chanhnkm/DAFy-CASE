#include "ContentsAnalyzer.h"

ContentsAnalyzer::ContentsAnalyzer(String contents)
	: contents(contents) {

}

ContentsAnalyzer::~ContentsAnalyzer() {

}

Array<String*> ContentsAnalyzer::MakeOperators() {
	String contents = this->contents; //����
	//1. ���뿡�� ������ �����.
	contents.Remove(' ');
	//2. ���뿡�� ���๮�ڸ� �����.
	contents.Remove('\n');

	Array<String*> operators;
	char character;
	//3. ������ ó������ ������ �ݺ��ϴ�.
	Long i = 0;
	while (i < contents.GetLength()) {
		String *oper = new String;
		//3.1. ���ڸ� ��������.
		character = contents.GetAt(i);
		//3.2. _�� ������ Ư����ȣ�̰� �� ���ڰ� �ƴ� ���� �ݺ��ϴ�.
		while ((character == 33 || character == 40 || character == 41 || character == 42 || character == 42 ||
			character == 45 || character == 47 || character == 60 || character == 61 || character == 62) &&
			character != '\0') {
			//3.2.2. �����ڸ� �����.
			oper += character;
			//3.2.1. ���ڸ� ��������.
			character = contents.GetAt(++i);
		}
		//3.3. �����ڸ� �����ڵ鿡 �߰��ϴ�.
		operators.Store(operators.GetLength(), oper);
		//3.4. _�� ������ Ư����ȣ�� �ƴ� ���� �ݺ��ϴ�.
		while (!(character == 33 || character == 40 || character == 41 || character == 42 || character == 42 ||
			character == 45 || character == 47 || character == 60 || character == 61 || character == 62)) {
			character = contents.GetAt(++i);
		}
		i++;
	}
	//4. �����ڵ��� ����ϴ�.
	return operators;
	//5. ������.
}

Array<String*> ContentsAnalyzer::MakeVariables() {
	String contents = this->contents; //����
	//1. ���뿡�� ������ �����.
	contents.Remove(' ');
	//2. ���뿡�� ���๮�ڸ� �����.
	contents.Remove('\n');

	Array<String*> variables;
	char character;
	//3. ������ ó������ ������ �ݺ��ϴ�.
	Long i = 0;
	while (i < contents.GetLength()) {
		String *variable = new String;
		//3.1. ���ڸ� ��������.
		character = contents.GetAt(i);
		//3.2. �������̰ų� �����̰ų� _�̰� �� ���ڰ� �ƴ� ���� �ݺ��ϴ�.
		while (((character >= 48 && character <= 57) ||
			(character >= 65 && character <= 90) ||
			(character >= 97 && character <= 122) ||
			character == 95) &&
			character != '\0') {
			//3.2.2. �����ڸ� �����.
			variable += character;
			//3.2.1. ���ڸ� ��������.
			character = contents.GetAt(++i);
		}
		//3.3. �����ڸ� �����ڵ鿡 �߰��ϴ�.
		variables.Store(variables.GetLength(), variable);
		//3.4. �������̰ų� �����̰ų� _�� �ƴ� ���� �ݺ��ϴ�.
		while (!((character >= 48 && character <= 57) ||
			(character >= 65 && character <= 90) ||
			(character >= 97 && character <= 122) ||
			character == 95)) {
			character = contents.GetAt(++i);
		}
		i++;
	}
	//4. �����ڵ��� ����ϴ�.
	return variables;
	//5. ������.
}

String ContentsAnalyzer::CorrectOperators() {
	//'��' '��' '��'
	char character;
	//1. ������ ó������ ������ �ݺ��ϴ�.
	Long i = 0;
	while (i < this->contents.GetLength()) {
		//1.1. ���ڸ� ��������.
		character = this->contents.GetAt(i);
		//1.2. ���ڰ� = �̰� ù ��° ���ڰ� �ƴϸ�
		if (character == '=' && i > 0) {
			//1.2.1. ���� ���ڸ� ��������.
			character = this->contents.GetAt(i - 1);
			switch (character) {
			//1.2.2. ���� ���ڰ� ! �̸� ���� ��ġ��.
			case '!':
				this->contents.Delete(i - 1, 2);
				this->contents.Insert(i - 1, '��');
			//1.2.3. ���� ���ڰ� < �̸� �·� ��ġ��.
			case '<':
				this->contents.Delete(i - 1, 2);
				this->contents.Insert(i - 1, '��');
			//1.2.4. ���� ���ڰ� > �̸� �÷� ��ġ��.
			case '>':
				this->contents.Delete(i - 1, 2);
				this->contents.Insert(i - 1, '��');
			}
		}
		i++;
	}
	//2. ������ ����ϴ�.
	return this->contents;
	//3. ������.
}