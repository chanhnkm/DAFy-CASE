// SizeMake.h
#include "SizeMake.h"
#include "DrawingPaper.h"
#include "FlowChart.h"
#include "Shape.h"
#include "Symbol.h"

SizeMake::SizeMake() {
}

SizeMake::~SizeMake() {
}

void SizeMake::Create(DrawingPaper *canvas) {
	canvas->flowChart->AscendingSort();

	Long i;
	Long j = 0;
	NShape *shape;
	//NShape* (*indexes) = 0;

	// 1. ���õ� ��ȣ���� ã�´�.
	Long count;
	Long(*indexes);
	canvas->flowChart->GetSelecteds(&indexes, &count);
#if 0
	Long it = canvas->flowChart->GetLength();

	if (it > 0) {
		indexes = new NShape*[it];
	}

	i = 0;
	while (i < it) {
		shape = canvas->flowChart->GetAt(i);
		if (shape->IsSelected()) {
			if (dynamic_cast<Symbol *>(shape)) {
				indexes[j] = shape->Clone();
				count++;
				j++;
			}
			else {
				shape->Select(false);
			}
		}
		i++;
	}
	// 2. ���� ����� ��ȣ�� ã�´�.
	Long index;
	Long y = 0;
	i = 0;
	while (i < count) {
		if (indexes[i]->GetY() < y || y == 0) {
			y = indexes[i]->GetY();
			index = i;
		}
		i++;
	}
#endif

	// 3. ����� ��ȣ�� ũ�⸦ ����Ѵ�.
	float width;
	float height;
	shape = canvas->flowChart->GetAt(indexes[0]);
	width = shape->GetWidth();
	height = shape->GetHeight();

	// 4. ���õ� ��ȣ���� ũ�⸦ ���ؿ� �°� �Ѵ�.
	i = 1;
	while (i < count) {
		shape = canvas->flowChart->GetAt(indexes[i]);
		shape->ReSize(width, height);
		i++;
	}
#if 0
	// 5. �������� ��ȣ�� �����.
	i = it - 1;
	while (i >= 0) {
		shape = canvas->flowChart->GetAt(i);
		if (shape->IsSelected()) {
			canvas->flowChart->Detach(i);
		}
		i--;
	}

	// 6. ���õ� ������ �������� �����ִ´�.
	i = 0;
	while (i < count) {
		canvas->flowChart->Attach(indexes[i]);
		i++;
	}
#endif
	// 7. Indexes ����
	if (indexes != 0) {
		delete[] indexes;
	}
}