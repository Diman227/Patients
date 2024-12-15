#ifndef PATIENT_H
#define PATIENT_H

#include <QString>
#include <QStringList>

class Patient {
public:
    Patient();  // Конструктор по умолчанию
    Patient(const QString& lastName, const QString& firstName, const QString& middleName,
            int birthYear, double height, double weight);  // Конструктор с параметрами

    // Методы доступа (геттеры)
    QString getLastName() const;
    QString getFirstName() const;
    QString getMiddleName() const;
    int getBirthYear() const;
    double getHeight() const;
    double getWeight() const;

    // Метод для преобразования объекта в строковый список
    QStringList toStringList() const;

private:
    QString lastName;
    QString firstName;
    QString middleName;
    int birthYear;
    double height;
    double weight;
};

#endif // PATIENT_H
