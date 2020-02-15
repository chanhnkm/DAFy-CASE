/******************************************************************
* ���� �̸� : Template.h
* ��� : ������ �ۼ��� ��Ʈ�� Ŭ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 4�� 13�� 
*******************************************************************/

#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include "Shape.h"
#include "Array.h"

typedef signed long int Long;

class Template : public Shape{
public:
	Template(Long capacity = 30);
	~Template();

	Template(const Template& source);
	Template& operator =(const Template& source);

	Long Register(Shape* shape);
	Long UnRegister(Long index);
	void Clear();


	Shape* GetAt(Long index);
	Long GetCapacity() const;
	Long GetLength() const;

	virtual Long Find(CDC *dc, CPoint point);
	virtual Long Find(Painter *painter, QPoint point);

	virtual void Accept(FlowChartVisitor *draw);

	virtual Shape* Clone();		

	virtual void GetRegion(Painter *painter, CRgn *region){};
	virtual void GetRegion(Painter *painter, Long thickness, CRgn *region){};

private:
	Array<Shape *> templates;
	Long capacity;
	Long length;
};
Long CompareCoordinateForTemplate(void *one, void *other);

inline Long Template::GetCapacity() const{
	return this->capacity;
}

inline Long Template::GetLength() const{
	return this->length;
}

#endif //_TEMPLATE_H
