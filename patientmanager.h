#ifndef PATIENTMANAGER_H
#define PATIENTMANAGER_H

#include <QList>
#include <QString>
#include "Patient.h"

class PatientDialog;

class PatientManager {
public:
    PatientManager();
    QList<Patient> findPatientsByLastName(const QString& lastName) const;
    QList<Patient> findPatientsByBirthYear(int birthYear) const;
    QList<Patient> getPatients() const;
    static void saveToFile(const QString& filePath, const QList<Patient>& patientList);
    static bool loadFromFile(const QString& filePath, QList<Patient>& patientList);

private:
    QList<Patient> patientList;

};

#endif // PATIENTMANAGER_H

