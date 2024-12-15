#include "Patient.h"

Patient::Patient() : lastName(""), firstName(""), middleName(""), birthYear(0), height(0), weight(0.0) {}

Patient::Patient(const QString& lastName, const QString& firstName, const QString& middleName, int birthYear, double height, double weight)
    : lastName(lastName), firstName(firstName), middleName(middleName), birthYear(birthYear), height(height), weight(weight) {}

QStringList Patient::toStringList() const {
    QStringList list;
    list << lastName << firstName << middleName << QString::number(birthYear) << QString::number(height) << QString::number(weight);
    return list;
}

QString Patient::getLastName() const { return lastName; }
QString Patient::getFirstName() const { return firstName; }
QString Patient::getMiddleName() const { return middleName; }
int Patient::getBirthYear() const { return birthYear; }
double Patient::getHeight() const { return height; }
double Patient::getWeight() const { return weight; }
