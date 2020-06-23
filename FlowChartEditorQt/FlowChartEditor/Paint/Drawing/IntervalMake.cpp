// IntervalMake.cpp

#include "IntervalMake.h"
#include "DrawingPaper.h"
#include "Shape.h"
#include "Symbol.h"

IntervalMake::IntervalMake(){
}

IntervalMake::~IntervalMake(){
}

void IntervalMake::Create(DrawingPaper *canvas){
	canvas->flowChart->AscendingSort();
	// 1. ���õ� �������� ã�´�.
	NShape *shape;
	Long count;
	Long(*indexes);
	canvas->flowChart->GetSelecteds(&indexes, &count);

#if 0

	Long it = canvas->flowChart->GetLength();
	NShape* (*indexes) = new NShape* [it];

	Long i = 0;	
	while ( i < it ){
		shape = canvas->flowChart->GetAt(i);
		if( shape->IsSelected()){
			//  ���õ� ��ȣ�� �� ���� ���� �����Ѵ�.
			if( dynamic_cast<Symbol *>(shape) ){
				indexes[count] = shape->Clone();
				count++;
			}
			else{
				shape->Select(false);
			}
		}
		i++;
	}
	// 2. �������� y ��ǥ�� �������� ������������ ���� �Ѵ�. (��������)
	NShape *temp;	
	float y;
	float y_;
	Long j;
	Long k;
	for( i = 1 ; i < count; i++){		
		y = indexes[i]->GetY();		
		j = 0;		
		y_ = indexes[j]->GetY();
		while( j < i &&  y_ < y ){
			j++;
			y_ = indexes[j]->GetY();
		}

		temp = indexes[i];

		for(k = i ; k >= j+1; k--){
			indexes[k] = indexes[k-1];
		}
		indexes[k] = temp;
	}
#endif
	shape = canvas->flowChart->GetAt(indexes[count - 1]);
	float lastY = shape->GetY();
	shape = canvas->flowChart->GetAt(indexes[0]);
	float firstY = shape->GetY();
	float firstHeight = shape->GetHeight();
	// 3. ������ ������ ���Ѵ�. ��ü���̿��� �������� ���̸� ���� ������ ���� - 1 ���� ������.
	//float height = indexes[count-1]->GetY() - indexes[0]->GetY() - indexes[0]->GetHeight();
	float height = lastY - firstY - firstHeight;

	Long i = 1;
	while ( i < count-1 ){
		shape = canvas->flowChart->GetAt(indexes[i]);
		height = height - shape->GetHeight();
		i++;
	}
	float interval = height/(count-1);

	// 4. ������ �� ��ġ �Ѵ�.

	float previousHeight; // ���������� ����
	shape = canvas->flowChart->GetAt(indexes[0]);
	previousHeight = shape->GetY() + shape->GetHeight();
	i = 1; // ù�� ° ������ ��ġ ����
	while ( i < count ){
		shape = canvas->flowChart->GetAt(indexes[i]);
		shape->Move(shape->GetX(), previousHeight + interval);
		previousHeight = previousHeight + interval + shape->GetHeight();
		i++;
	}
#if 0
	// 5. �������� ���õ� ������ �����.
	i = it -1;
	while ( i >= 0 ){
		shape = canvas->flowChart->GetAt(i);
		if( shape->IsSelected()){
			canvas->flowChart->Detach(i);
		}
		i--;
	}

	// 6. �������� ���õ� ������ ���� �ִ´�.
	i = 0;
	while ( i < count ){
		canvas->flowChart->Attach(indexes[i]);
		i++;
	}
#endif

	if ( indexes != 0 ){
		delete[] indexes;
	}
}