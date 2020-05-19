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
	Long i;
	Long j = 0;
	NShape *shape;
	NShape* (*indexes) = 0;
	Long count = 0;

	// 1. ���õ� ��ȣ���� ã�´�.
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

	// 3. ����� ��ȣ�� ũ�⸦ ����Ѵ�.
	float width;
	float height;
	width = indexes[index]->GetWidth();
	height = indexes[index]->GetHeight();

	// 4. ���õ� ��ȣ���� ũ�⸦ ���ؿ� �°� �Ѵ�.
	i = 0;
	while (i < count) {
		indexes[i]->ReSize(width, height);
		i++;
	}

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

	// 7. Indexes ����
	if (indexes != 0) {
		delete[] indexes;
	}
}