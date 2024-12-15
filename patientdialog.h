#ifndef PATIENTDIALOG_H
#define PATIENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include "Patient.h"

class PatientDialog : public QDialog {
    Q_OBJECT

public:
    PatientDialog(QWidget* parent = nullptr, const Patient& patient = Patient());
    Patient getPatientData() const;

public:
    QLineEdit* editLastName;
    QLineEdit* editFirstName;
    QLineEdit* editMiddleName;
    QDateEdit* editBirthYear;
    QLineEdit* editHeight;
    QLineEdit* editWeight;
    QPushButton* saveButton;
    QPushButton* cancelButton;
    void checkFields();
    void setValidators(QLineEdit *editLastName, QLineEdit *editFirstName, QLineEdit *editMiddleName, QLineEdit *editHeight, QLineEdit *editWeight);
    void createLayout();
};

#endif // PATIENTDIALOG_H
