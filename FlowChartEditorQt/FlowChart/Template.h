/******************************************************************
* ���� �̸� : Template.h
* ��� : ������ �ۼ��� ��Ʈ�� Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 4�� 13�� 
*******************************************************************/

#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include "Block.h"

class Template : public Block {
public:
	Template(Long capacity = 30);
	Template(const Template& source);
	virtual ~Template();
	Template& operator =(const Template& source);

	virtual void Accept(FlowChartVisitor *draw);

	virtual Shape* Clone();		

	virtual void GetRegion(QRegion *region){};
	virtual void GetRegion(Long thickness, QRegion *region){};
};

//Long CompareCoordinateForTemplate(void *one, void *other); //�̰� ���ִ°ɱ�?

#endif //_TEMPLATE_H
