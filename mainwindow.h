#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QList>
#include "Patient.h"
#include "patientmanager.h"

class MenuHandler;
class PatientDialog;  // Для диалога добавления/редактирования пациента

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void saveToFileDialog();
    void loadFromFileDialog();
    void exitApp();
    void addPatient();
    void editPatient();
    void removePatient();
    void showAppInfo();
    void closeData();
    void updateTable(const QList<Patient>& patients);
    //void setupShortcuts();
    void updateWindowTitle();
    bool checkAndSaveChanges();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    // Виджеты интерфейса
    QTableWidget* tableWidget;

    // Меню
    MenuHandler* menuHandler;

    PatientManager* patientManager;

    // Данные
    QList<Patient> patientList;
    QString currentFilePath;
    bool isModified;


};

#endif // MAINWINDOW_H
