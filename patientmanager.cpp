#include "PatientManager.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

PatientManager::PatientManager() {}

QList<Patient> PatientManager::getPatients() const {
    return patientList;
}

void PatientManager::saveToFile(const QString& filePath, const QList<Patient>& patientList) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Ошибка", "Не удалось открыть файл для записи");
        return;
    }

    QTextStream out(&file);
    for (const Patient& patient : patientList) {
        out << patient.toStringList().join("\t") << "\n";
    }

    file.close();
}

bool PatientManager::loadFromFile(const QString& filePath, QList<Patient>& patientList) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Ошибка", "Не удалось открыть файл для чтения");
        return false;
    }

    QTextStream in(&file);
    patientList.clear();
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("\t");

        if (parts.size() == 6) {
            Patient patient(parts[0], parts[1], parts[2], parts[3].toInt(), parts[4].toInt(), parts[5].toDouble());
            patientList.append(patient);
        }
    }

    file.close();
    return true;
}

QList<Patient> PatientManager::findPatientsByLastName(const QString& lastName) const {
    QList<Patient> foundPatients;
    for (const Patient& patient : patientList) {
        if (patient.getLastName() == lastName) {
            foundPatients.append(patient);
        }
    }
    return foundPatients;
}

QList<Patient> PatientManager::findPatientsByBirthYear(int birthYear) const {
    QList<Patient> foundPatients;
    for (const Patient& patient : patientList) {
        if (patient.getBirthYear() == birthYear) {
            foundPatients.append(patient);
        }
    }
    return foundPatients;
}
