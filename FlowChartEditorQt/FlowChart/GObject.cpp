#include "GObject.h"

GObject::GObject() {

}

GObject::GObject(const GObject& source) {

}

GObject::~GObject() {

}

GObject& GObject::operator=(const GObject& source) {
	return *this;
}

void GObject::SetObject(QPainter *qPainter) {

}

string GObject::GetIdentify() {
	return "";
}

//Painter ���� �Լ� ����
void GObject::SelectObject(const GObject& object, GObject *previous, Long *index) {

}

GObject* GObject::CurrentObject(string identify) {
	return 0;
}

void GObject::Update(Long index) {

}

GObject* GObject::GetAt(Long index) {
	return 0;
}

Long GObject::GetCapacity() const {
	return 0;
}

Long GObject::GetLength() const {
	return -1;
}