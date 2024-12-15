#include "PatientDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QDateEdit>
#include <QCalendarWidget>
#include "Patient.h"

PatientDialog::PatientDialog(QWidget* parent, const Patient& patient) : QDialog(parent) {
    editLastName = new QLineEdit(patient.getLastName());
    editFirstName = new QLineEdit(patient.getFirstName());
    editMiddleName = new QLineEdit(patient.getMiddleName());

    editBirthYear = new QDateEdit(this);
    editBirthYear->setDisplayFormat("yyyy");
    editBirthYear->setCalendarPopup(true); // Всплывающее окно календаря
    editBirthYear->setDate(QDate(patient.getBirthYear() > 0 ? patient.getBirthYear() : 2024, 1, 1));
    editBirthYear->setMinimumDate(QDate(1900, 1, 1));
    editBirthYear->setMaximumDate(QDate::currentDate());

    QCalendarWidget* calendar = editBirthYear->calendarWidget();
    calendar->setMinimumSize(300, 200);

    editHeight = new QLineEdit(patient.getHeight() > 0 ? QString::number(patient.getHeight()) : "");
    editWeight = new QLineEdit(patient.getWeight() > 0 ? QString::number(patient.getWeight()) : "");

    setWindowTitle("Добавление/редактирование пациента");
    setValidators(editLastName, editFirstName, editMiddleName, editHeight, editWeight);
    createLayout();
}

void PatientDialog::setValidators(QLineEdit *editLastName, QLineEdit *editFirstName, QLineEdit *editMiddleName, QLineEdit *editHeight, QLineEdit *editWeight) {
    QRegularExpression nameRegex("[A-Za-zА-Яа-яЁё]+"); // Разрешаем буквы и символы, такие как Ё
    QRegularExpressionValidator *nameValidator = new QRegularExpressionValidator(nameRegex, this);
    editLastName->setValidator(nameValidator);
    editFirstName->setValidator(nameValidator);
    editMiddleName->setValidator(nameValidator);

    QIntValidator *heightValidator = new QIntValidator(0, 300, this);
    editHeight->setValidator(heightValidator);

    QDoubleValidator *weightValidator = new QDoubleValidator(0.0, 300.0, 3, this);
    weightValidator->setNotation(QDoubleValidator::StandardNotation);
    editWeight->setValidator(weightValidator);
}

void PatientDialog::createLayout() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Фамилия:"));
    layout->addWidget(editLastName);
    layout->addWidget(new QLabel("Имя:"));
    layout->addWidget(editFirstName);
    layout->addWidget(new QLabel("Отчество(если имеется):"));
    layout->addWidget(editMiddleName);
    layout->addWidget(new QLabel("Год рождения:"));
    layout->addWidget(editBirthYear);
    layout->addWidget(new QLabel("Рост(в см):"));
    layout->addWidget(editHeight);
    layout->addWidget(new QLabel("Вес(в кг):"));
    layout->addWidget(editWeight);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    saveButton = new QPushButton("Сохранить");
    cancelButton = new QPushButton("Отмена");
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, this, &PatientDialog::checkFields);
    connect(cancelButton, &QPushButton::clicked, this, &PatientDialog::reject);
}

void PatientDialog::checkFields() {
    if (editLastName->text().isEmpty() ||
        editFirstName->text().isEmpty() ||
        editBirthYear->text().isEmpty() ||
        editHeight->text().isEmpty() ||
        editWeight->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все обязательные поля.");
        return;
    }
    accept();
}

Patient PatientDialog::getPatientData() const {
    return Patient(editLastName->text(), editFirstName->text(), editMiddleName->text(), editBirthYear->text().toInt(), editHeight->text().toInt(), editWeight->text().toDouble());
}
