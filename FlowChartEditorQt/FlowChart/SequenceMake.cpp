//SequenceMake.cpp

#include "SequenceMake.h"
#include "DrawingPaper.h"
#include "FlowChart.h"
#include "Shape.h"
#include "Symbol.h"
#include "Decision.h"
#include "Arrow.h"
#include "LeftDown.h"
#include "RightDown.h"
#include "Join.h"
#include "RightDownJoin.h"
#include "RepeatTrue.h"
#include "RepeatFalse.h"
#include "Registrar.h"

SequenceMake::SequenceMake() {
}

SequenceMake::~SequenceMake() {
}

void SequenceMake::Create(DrawingPaper *canvas) {
	// 1. �迭 ÷�ڰ� ���� ���� ������ ã�´�.	
	Long(*indexes);
	Long length;
	NShape *shape;
	NShape *arrow;
	float x, y, width, height;
	Long i, j, k;
	Long index = -1;

	canvas->flowChart->GetSelecteds(&indexes, &length);

	FlowChart buffer(length);
	FlowChart arrows(length);

	for (i = 0; i < length; i++) {
		shape = canvas->flowChart->GetAt(indexes[i]);
		if (dynamic_cast<Symbol *>(shape) 
			|| dynamic_cast<Join*>(shape) || dynamic_cast<RightDownJoin*>(shape)) {
			buffer.Attach(shape->Clone());
			if (index > indexes[i] || index == -1) {
				index = indexes[i];
			}
		}
		else if (!dynamic_cast<Arrow *>(shape)) {
			shape->Select(false);
		}
	}

	if (indexes != 0) {
		delete[] indexes;
	}

	canvas->flowChart->DetachSelectedAll();

	buffer.AscendingSort();

	Attribute attribute;
	for (i = 0; i < buffer.GetLength() - 1; i++) {
		buffer.GetAt(i)->GetAttribute(&attribute);
		x = attribute.pointOut.x();
		y = attribute.pointOut.y();
		width = 0;
		height = buffer.GetAt(i + 1)->GetY() - attribute.pointOut.y();
		if (height > 0) {
			arrow = new Arrow(x, y, width, height, QColor(0, 0, 0), Qt::SolidLine, QColor(0, 0, 0), String(" "));
			arrow->Select(true);
			arrows.Attach(arrow);
		}
	}

	i = 0;
	j = 0;
	k = 0;
	while (i < buffer.GetLength() || j < arrows.GetLength()) {
		if (i < buffer.GetLength()) {
			canvas->flowChart->Insert(index + k, buffer.GetAt(i)->Clone());
			k++;
			i++;
		}
		if (j < arrows.GetLength()) {
			canvas->flowChart->Insert(index + k, arrows.GetAt(j)->Clone());
			canvas->registrar->Register(canvas->flowChart->GetAt(index + k));
			k++;
			j++;
		}
	}

	canvas->repaint();
}