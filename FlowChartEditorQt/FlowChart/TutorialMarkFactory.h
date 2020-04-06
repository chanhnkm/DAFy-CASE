#ifndef _TUTORIALMARKFACTORY_H
#define _TUTORIALMARKFACTORY_H

typedef signed long int Long;
class TutorialMark;
#include "Shape.h"

using namespace FlowChartShape;

class TutorialMarkFactory {
public:
	TutorialMarkFactory(Shape *sample = 0);
	TutorialMarkFactory(const TutorialMarkFactory& source);
	~TutorialMarkFactory();
	TutorialMarkFactory& operator=(const TutorialMarkFactory& source);

	TutorialMark* Make(Long length); //Ʃ�丮�� �ó������� ���� ����(���� � �ó���������)�� ���� ǥ�ø� �޸� ��.
private:
	Shape *sample;
};

#endif //_TUTORIALMARKFACTORY_H