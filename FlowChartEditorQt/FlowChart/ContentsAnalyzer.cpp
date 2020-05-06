#include "ContentsAnalyzer.h"

ContentsAnalyzer::ContentsAnalyzer() {

}

ContentsAnalyzer::~ContentsAnalyzer() {

}

Array<String> ContentsAnalyzer::MakeOperators(String contents) {
	Array<String> operators;
	char character;
	//1. ������ ó������ ������ �ݺ��ϴ�.
	Long i = 0;
	while (i < contents.GetLength()) {
		String oper;
		//1.1. ���ڸ� ��������.
		character = contents.GetAt(i);
		//1.2. _ , .�� ������ Ư����ȣ�̰� �� ���ڰ� �ƴ� ���� �ݺ��ϴ�.
		while ((character == 33 || character == 40 || character == 41 || character == 42 || character == 43 ||
			character == 45 || character == 47 || character == 60 || character == 61 || character == 62) &&
			character != '\0') {
			//1.2.2. �����ڸ� �����.
			oper += character;
			//1.2.1. ���ڸ� ��������.
			character = contents.GetAt(++i);
		}
		//1.3. �����ڿ� �ش��ϴ� Ư����ȣ�̸� �����ڸ� �����ڵ鿡 �߰��ϴ�.
		if (oper.GetLength() > 0) {
			operators.Store(operators.GetLength(), oper);
		}
		//1.4. _�� ������ Ư����ȣ�� �ƴ� ���� �ݺ��ϴ�.
		while ((!(character == 33 || character == 40 || character == 41 || character == 42 || character == 43 ||
			character == 45 || character == 47 || character == 60 || character == 61 || character == 62)) &&
			character != '\0') {
			character = contents.GetAt(++i);
		}
		if (character != '\0') { //Ư����ȣ�� ã������ Ư����ȣ���� ������ �� �ֵ��� ÷�ڸ� �ٿ��ش�.
			i--;
		}
		i++;
	}
	//2. �����ڵ��� ����ϴ�.
	return operators;
	//3. ������.
}

Array<String> ContentsAnalyzer::MakeVariables(String contents) {
	Array<String> variables;
	char character;
	bool isQuotes = false;
	//1. ������ ó������ ������ �ݺ��ϴ�.
	Long i = 0;
	while (i < contents.GetLength()) {
		String variable;
		//1.1. ���ڸ� ��������.
		character = contents.GetAt(i);
		//1.2. �������̰ų� �����̰ų� _�̰� �� ���ڰ� �ƴ� ���� �ݺ��ϴ�.
		while (((character >= 48 && character <= 57) ||
			(character >= 65 && character <= 90) ||
			(character >= 97 && character <= 122) ||
			character == 95) &&
			character != '\0') {
			//1.2.2. ������ �����.
			variable += character;
			//1.2.1. ���ڸ� ��������.
			character = contents.GetAt(++i);
		}
		//1.3. ������ �����鿡 �߰��ϴ�.
		if (variable.GetLength() > 0) {
			variables.Store(variables.GetLength(), variable);
		}

		//1.4. �����ڰ� �ƴ� ���� �Ǵ� ����ǥ ���� ���� �ݺ��ϴ�.
		// (����ǥ ���̶�� �����ڿ��� �ݺ��Ѵ�.)
		while (((!((character >= 65 && character <= 90) ||
			(character >= 97 && character <= 122))) ||
			isQuotes == true) &&
			character != '\0') {
			if (isQuotes == false && (character == 34 || character == 39)) { //����ǥ�� �ٽ� ������ ���ݺ��� ����ǥ ����.
				isQuotes = true;
			}
			else if (isQuotes == true && (character == 34 || character == 39)) { //����ǥ�� �ٽ� ������ ���ݺ��� ����ǥ ����.
				isQuotes = false;
			}
			character = contents.GetAt(++i);
		}
		if (character != '\0') { //�����ڸ� ã������ �����ں��� ������ �� �ֵ��� ÷�ڸ� �ٿ��ش�.
			i--;
		}
		i++;
	}
	//2. �������� ����ϴ�.
	return variables;
	//3. ������.
}

String ContentsAnalyzer::CorrectOperators(String contents) {
	//'��' '��' '��'
	char character;
	//1. ������ ó������ ������ �ݺ��ϴ�.
	Long i = 0;
	while (i < contents.GetLength()) {
		//1.1. ���ڸ� ��������.
		character = contents.GetAt(i);
		//1.2. ���ڰ� = �̰� ù ��° ���ڰ� �ƴϸ�
		if (character == '=' && i > 0) {
			//1.2.1. ���� ���ڸ� ��������.
			character = contents.GetAt(i - 1);
			switch (character) {
			case '!': //1.2.2. ���� ���ڰ� ! �̸� ���� ��ġ��.
				contents.Delete(i - 1, 2);
				contents.Insert(i - 1, "��");
				break;
			case '<': //1.2.3. ���� ���ڰ� < �̸� �·� ��ġ��.
				contents.Delete(i - 1, 2);
				contents.Insert(i - 1, "��");
				break;
			case '>': //1.2.4. ���� ���ڰ� > �̸� �÷� ��ġ��.
				contents.Delete(i - 1, 2);
				contents.Insert(i - 1, "��");
				break;
			default:
				break;
			}
		}
		i++;
	}
	//2. ������ ����ϴ�.
	return contents;
	//3. ������.
}