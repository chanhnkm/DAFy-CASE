#include "Painter.h"

Painter::Painter(Long capacity)
	: GObject(), objects(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

Painter::Painter(const Painter& source)
	: GObject(source), objects(source.objects) {
	Long i = 0;
	while (i < source.length) {
		this->objects.Modify(i, (const_cast<Painter&>(source)).objects[i]->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

Painter::~Painter() {
	Long i = 0;
	while (i < this->length) {
		if (this->objects[i] != 0) {
			delete this->objects[i];
		}
		i++;
	}
}

Painter& Painter::operator=(const Painter& source) {
	GObject *object;

	Long i = 0;
	while (i < this->length) {
		object = this->objects.GetAt(i);
		if (object != 0) {
			delete object;
		}
		i++;
	}

	this->objects = source.objects;

	i = 0;
	while (i < source.GetLength()) {
		object = const_cast<Painter&>(source).GetAt(i)->Clone();
		this->objects.Modify(i, object);
		i++;
	}

	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

void Painter::SelectObject(const GObject& object, GObject *previous, Long *index) {
	//1. ù ��° �׷��� ��ü�� ��������.
	previous = this->objects.GetAt(0);
	//2. �ٲ� �׷��� ��ü�� ��ü���� ��������.
	string identify = const_cast<GObject&>(object).GetIdentify();
	//3. ù ��° �׷��� ��ü�� ��ü���� ��������.
	string previousIdenfity = previous->GetIdentify();

	//4. ��뷮��ŭ �׸��� ���� �׷��� ��ü�� ��ü���� �ٲ� �׷��� ��ü�� ��ü���� �ٸ� ���� �ݺ��Ѵ�.
	Long i = 1;
	while (i < this->length && previousIdenfity != identify) {
		//4.1. ���� �׷��� ��ü�� ��������.
		previous = this->objects.GetAt(i);
		//4.2. ���� �׷��� ��ü�� ��ü���� ��������.
		previousIdenfity = previous->GetIdentify();
		i++;
	}
	*index = i - 1;
	//5. �ٲ� �׷��� ��ü�� ���� ������ �׷��� ��ü�� �迭���� ã������
	if (*index < this->length) {
		//5.1. ���� �׷��� ��ü�� �����ͼ� �����ϴ�.
		previous = this->objects.GetAt(*index)->Clone();
		//5.2. ���� ��ġ�� �ٲ� �׷��� ��ü�� �����ϴ�.
		this->objects.Modify(*index, &const_cast<GObject&>(object));
	}
	//6. ���� �׷��� ��ü�� ����ϴ�.
	//7. ���� ��ġ�� ����ϴ�.
}

GObject* Painter::CurrentObject(string identify) {
	Long index = -1;
	//1. ù ��° �׷��� ��ü�� ��������.
	GObject *current = this->objects.GetAt(0);
	//2. ù ��° �׷��� ��ü�� ��ü���� ��������.
	string currentIdenfity = current->GetIdentify();

	//3. ��뷮��ŭ �׸��� ���� �׷��� ��ü�� ��ü���� ã�� ��ü���� �ٸ� ���� �ݺ��Ѵ�.
	Long i = 1;
	while (i < this->length && currentIdenfity != identify) {
		//3.1. ���� �׷��� ��ü�� ��������.
		current = this->objects.GetAt(i);
		//3.2. ���� �׷��� ��ü�� ��ü���� ��������.
		currentIdenfity = current->GetIdentify();
		i++;
	}
	Long index = i - 1;
	if (index < this->length) { //4. ã������ ���� �׷��� ��ü�� ����Ѵ�.
		current = this->objects.GetAt(index);
	}
}

GObject* Painter::GetAt(Long index) {
	return this->objects.GetAt(index);
}