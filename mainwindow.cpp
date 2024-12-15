#include "mainwindow.h"
#include "menubuilder.h"
#include "PatientDialog.h"
#include "patientmanager.h"
#include <QTableWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QTextStream>
#include <QInputDialog>
#include <QApplication>
#include <QHeaderView>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRegularExpressionValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    tableWidget(new QTableWidget(this)),
    menuHandler(new MenuHandler(this)),
    isModified(false)
{
    resize(800, 900);
    setWindowTitle("Картотека пациентов");

    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderLabels({"Фамилия", "Имя", "Отчество", "Год рождения", "Рост(в см)", "Вес(в кг)"});
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLineEdit* searchLastNameEdit = new QLineEdit(this);
    QLineEdit* searchBirthYearEdit = new QLineEdit(this);
    QPushButton* searchButton = new QPushButton("Поиск", this);
    QPushButton* showAllButton = new QPushButton("Показать всех пациентов", this);

    searchBirthYearEdit->setValidator(new QIntValidator(1900, 2024, this));

    QRegularExpression nameRegex("[A-Za-zА-Яа-яЁё]+"); // Разрешены только буквы
    QRegularExpressionValidator* nameValidator = new QRegularExpressionValidator(nameRegex, this);
    searchLastNameEdit->setValidator(nameValidator);

    searchLayout->addWidget(new QLabel("Фамилия:"));
    searchLayout->addWidget(searchLastNameEdit);
    searchLayout->addWidget(new QLabel("Год рождения:"));
    searchLayout->addWidget(searchBirthYearEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(showAllButton);

    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(tableWidget);

    connect(searchButton, &QPushButton::clicked, [this, searchLastNameEdit, searchBirthYearEdit]() {
        QString lastName = searchLastNameEdit->text();
        int birthYear = searchBirthYearEdit->text().toInt();

        QList<Patient> foundPatients;

        if (!lastName.isEmpty()) {
            foundPatients = patientManager->findPatientsByLastName(lastName);
        }
        else if (!searchBirthYearEdit->text().isEmpty()) {
            foundPatients = patientManager->findPatientsByBirthYear(birthYear);
        }
        else {
            QMessageBox::warning(this, "Ошибка", "Введите корректные данные для поиска.");
            return;
        }

        if (foundPatients.isEmpty()) {
            QMessageBox::information(this, "Результаты поиска", "Пациент(ы) не найден(ы).");
            updateTable(patientManager->getPatients());
            searchLastNameEdit->clear();
            searchBirthYearEdit->clear();
        } else {
            updateTable(foundPatients);
        }
    });


    connect(showAllButton, &QPushButton::clicked, [this, searchLastNameEdit, searchBirthYearEdit]() {

        searchLastNameEdit->clear();
        searchBirthYearEdit->clear();

        updateTable(patientManager->getPatients());
    });
}

MainWindow::~MainWindow() {
    delete menuHandler;
}

void MainWindow::saveToFileDialog() {
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "Text Files (*.txt);;All Files (*)");
    if (!filePath.isEmpty()) {
        currentFilePath = filePath;
        PatientManager::saveToFile(currentFilePath, patientList);
        isModified = false;
        updateWindowTitle();
        QMessageBox::information(this, "Сохранение", "Файл успешно сохранен");
    }
     QMessageBox::information(this, "Сохранение файла", "Файл не сохранён.");
}

void MainWindow::loadFromFileDialog() {
    if (checkAndSaveChanges()) {

        QString filePath = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Text Files (*.txt);;All Files (*)");
        if (!filePath.isEmpty()) {

            if (!filePath.endsWith(".txt", Qt::CaseInsensitive)) {
                QMessageBox::warning(this, "Ошибка", "Файл не загружен. Выбранный файл не текстового формата(.txt).");
                return;
            }

            currentFilePath = filePath;
            if (PatientManager::loadFromFile(currentFilePath, patientList)) {
                updateTable(patientManager->getPatients());
                isModified = false;
                updateWindowTitle();
                QMessageBox::information(this, "Загрузка файла", "Файл успешно загружен");
            }
        }
    }
}

void MainWindow::exitApp() {
    QApplication::quit();
}

void MainWindow::addPatient() {
    PatientDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Patient newPatient = dialog.getPatientData();
        patientList.append(newPatient);
        updateTable(patientManager->getPatients());
        isModified = true;
        updateWindowTitle();
        QMessageBox::information(this, "Добавление пациента", "Пациент успешно добавлен!");
    }
}

void MainWindow::closeData() {
    if (this->checkAndSaveChanges()) {
        this->patientList.clear();
        updateTable(patientManager->getPatients());
        QMessageBox::information(this, "Закрытие", "Данные закрыты. Открыта чистая таблица");
        MainWindow::updateWindowTitle();
    }
}

void MainWindow::editPatient() {
    int currentRow = tableWidget->currentRow();
    if (currentRow >= 0) {
        PatientDialog* editDialog = new PatientDialog(this, patientList[currentRow]);

        if (editDialog->exec() == QDialog::Accepted) {
            Patient updatedPatient = editDialog->getPatientData();
            patientList[currentRow] = updatedPatient;
            updateTable(patientManager->getPatients());
            isModified = true;
            QMessageBox::information(this, "Редактирование данных", "Данные пациента изменены");
            updateWindowTitle();
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Выберите пациента для редактирования");
    }
}

void MainWindow::removePatient() {
    int currentRow = tableWidget->currentRow();
    if (currentRow >= 0) {

        QString lastName = tableWidget->item(currentRow, 0)->text();

        QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение удаления", "Вы уверены, что хотите удалить пациента с фамилией '" + lastName + "'?",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {

            patientList.removeAt(currentRow);
            isModified = true;
            updateWindowTitle();
            updateTable(patientManager->getPatients());
            QMessageBox::information(this, "Удаление пациента", "Пациент успешно удален");
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Выберите пациента для удаления");
    }
}

void MainWindow::showAppInfo() {
    QFile file("../../appinfo.txt");
    QString aboutText;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        aboutText = file.readAll();
        file.close();
    } else {
        aboutText = "Не удалось загрузить информацию о программе.";
    }

    QMessageBox::information(this, "О программе", aboutText);
}

bool MainWindow::checkAndSaveChanges()
{
    if (isModified) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Сохранение данных", "У вас есть несохраненные изменения. Вы хотите сохранить изменения?",
                                                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            saveToFileDialog();
            return true;
        }
        else if (reply == QMessageBox::Cancel) {
            return false;
        }
        else if (reply == QMessageBox::No) {
            isModified = false;
        }
    }
    return true;
}

void MainWindow::updateTable(const QList<Patient>& patients)
{
    tableWidget->setRowCount(patients.size());
    for (int i = 0; i < patients.size(); ++i) {
        const Patient& patient = patients[i];
        tableWidget->setItem(i, 0, new QTableWidgetItem(patient.getLastName()));
        tableWidget->setItem(i, 1, new QTableWidgetItem(patient.getFirstName()));
        tableWidget->setItem(i, 2, new QTableWidgetItem(patient.getMiddleName()));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(patient.getBirthYear())));
        tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(patient.getHeight())));
        tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(patient.getWeight())));
        tableWidget->setRowHeight(i, 50);
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (checkAndSaveChanges()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::updateWindowTitle()
{
    setWindowTitle(isModified ? "Картотека пациентов (несохраненные изменения)" : "Картотека пациентов");
}

