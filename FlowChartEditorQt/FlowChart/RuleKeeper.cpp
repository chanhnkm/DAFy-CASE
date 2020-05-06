#include "RuleKeeper.h"
#include "VariableList.h"

RuleKeeper::RuleKeeper() {

}

RuleKeeper::RuleKeeper(const RuleKeeper& source) {

}

RuleKeeper::~RuleKeeper() {

}

RuleKeeper& RuleKeeper::operator=(const RuleKeeper& source) {
	return *this;
}

VariableList* RuleKeeper::CheckVariableNamingRule(Array<String> variables) {
	bool isKept = true;
	String variable;
	char character;
	Long j;
	//1. ���� ����� �����.
	VariableList *variableList = new VariableList(10);

	//2. ������ ������ŭ �׸��� ��Ģ�� ��Ű�� ���� �ݺ��ϴ�.
	Long i = 0;
	while (i < variables.GetLength() && isKept == true) {
		//2.1. ������ ��������.
		variable = variables.GetAt(i);
		//2.2. ������ ù ��° ���ڰ� �����ڰ� �ƴϸ� ��Ģ�� ��߳���.
		character = variable.GetAt(0);
		if ((character < 65 || character > 90) && (character < 97 || character > 122)) {
			isKept = false;
		}
		//2.3. ���ڰ� �빮���� ���� �׸��� ���� ������ŭ �ݺ��Ѵ�.
		j = 1;
		while (j < variable.GetLength() && (character >= 65 && character <= 90)) {
			//2.3.1. ���ڸ� ��������.
			character = variable.GetAt(++j);
		}
		//2.4. ù ��° ���ڰ� �빮���ε� �빮�ڰ� �ƴ� ���ڸ� ã������ ��Ģ�� ��߳���.
		if (j < variable.GetLength() && (variable.GetAt(0) >= 65 && variable.GetAt(0) <= 90)) {
			isKept = false;
		}
		//2.5. ���� ��Ͽ� �߰��ϴ�.
		variableList->Add(variable);
		i++;
	}
	//3. ���� ����� ����ϴ�.
	if (isKept == false) {
		variableList->RemoveAll();
	}

	return variableList;
	//4. ������.
}

bool RuleKeeper::CheckVariableUsingRule(Array<String> variables, VariableList *variableList) {
	bool isKept = true;
	String variable;
	Long index = -1;
	//1. ������ ������ŭ �׸��� ���� ��Ͽ� ���� ������ �ִ� ���� �ݺ��ϴ�.
	Long i = 0;
	while (i < variables.GetLength() && (index != -1 || i == 0)) {
		//1.1. ������ ��������.
		variable = variables.GetAt(i);
		//1.2. ���� ��Ͽ��� ã�´�.
		index = variableList->Find(variable);
		i++;
	}
	//2. ���� ��Ͽ� �Է¹��� ��� �������� �ִ��� ���ο� ���� ��Ģ �ؼ� ���θ� �����Ѵ�.
	if (index == -1) {
		isKept = false;
	}
	//3. ��Ģ �ؼ� ���θ� ����ϴ�.
	return isKept;
	//4. ������.
}

bool RuleKeeper::CheckOperatorRule(Array<String> operators) {
	bool isKept = true;
	String oper;
	//1. �����ڵ� ������ŭ �ݺ��ϴ�.
	Long i = 0;
	while (i < operators.GetLength() && isKept == true) {
		//1.1. �����ڸ� ��������.
		oper = operators.GetAt(i);
		//1.2. ���� �����ڰ� �ƴϸ� ��Ģ�� ��߳���.
		if (!(oper == "(" || oper == ")" || oper == "*" || oper == "+" || oper == "-" || oper == "/" ||
			oper == "<" || oper == "=" || oper == ">" || oper == "^" ||
			oper == "<>" || oper == "<=" || oper == ">=" || oper == "!=")) {
			isKept = false;
		}
		i++;
	}
	//2. ��Ģ �ؼ� ���θ� ����ϴ�.
	return isKept;
	//3. ������.
}