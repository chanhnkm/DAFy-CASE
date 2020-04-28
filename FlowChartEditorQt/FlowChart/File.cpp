// File.cpp

#include "File.h"
#include "DrawingPaper.h"
#include "FlowChart.h"
#include "Shape.h"
#include "Creator.h"
#include "Tokenizer.h"
#include "Preparation.h"
#include "VariableList.h"
#include "Zoom.h"
#include "../GObject/Painter.h"

#include <qfile.h>
#include <qtextstream.h>

File::File() {}

File::~File() {}

Long File::Load(DrawingPaper *canvas, QString fileName) {
	QFile file(fileName);
	String lineRead;
	//char line[513];

	Long count = 0;

	NShape * shape;

	Creator creator;
	Tokenizer tokenizer;
	QString qContents;

	canvas->flowChart->Clear();
	bool isOpen = file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (isOpen == true) {
		//�� ó��
		QTextStream textStream(&file);
		qContents = textStream.readLine();
		lineRead = qContents.toLocal8Bit().data();
		canvas->zoom->Set(atoi(lineRead));
		//���� ó��
		while (!textStream.atEnd()) {
			qContents = textStream.readLine();
			lineRead = qContents.toLocal8Bit().data();
			tokenizer.Scan(lineRead, '\t');
			String contents(" ");
			if (!(tokenizer.GetAt(7) == " ")) {
				contents = String(tokenizer.GetAt(7));
			}
			contents.Replace('\r', '\n');
			shape = creator.Create(atof(tokenizer.GetAt(0)), atof(tokenizer.GetAt(1)), atof(tokenizer.GetAt(2)), atof(tokenizer.GetAt(3)),
				atof(tokenizer.GetAt(4)), atof(tokenizer.GetAt(5)), atof(tokenizer.GetAt(6)), contents);
			canvas->flowChart->Attach(shape);

			//=====================intellisense========================
			if (dynamic_cast<Preparation*>(shape)) {
				if (canvas->variableList != NULL) {
					delete canvas->variableList;
				}
				canvas->variableList = new VariableList;
				canvas->variableList->Add(shape->GetContents());
			}
			//=========================================================

			count++;
		}
		file.close();
	}

	return count;
}

Long File::Save(DrawingPaper *canvas, QString fileName) {
	QFile file(fileName);
	Long i = 0;
	Long end;
	Long count = 0;
	char line[256];
	NShape *shape;
	Long rate;
	// 3. �����Ѵ�.
	bool isOpen = file.open(QIODevice::WriteOnly | QIODevice::Text);
	if (isOpen == true) {
		QTextStream textStream(&file);

		rate = canvas->zoom->GetRate();
		textStream << rate << "\n"; //���๮�� ���� ���
		end = canvas->flowChart->GetLength();
		while (i < end) {
			shape = canvas->flowChart->GetAt(i);
			shape->GetLine(line);
			textStream << line; //���๮�� ?
			count++;
			i++;
		}
		file.close();
	}

	return count;
}