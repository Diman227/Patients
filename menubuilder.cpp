#include "menubuilder.h"
#include <QMenuBar>
#include <QFileDialog>

MenuHandler::MenuHandler(MainWindow* parent) : QObject(parent), mainWindow(parent) {
    QMenuBar* menuBar = new QMenuBar(mainWindow);

    menuBar->addMenu(createFileMenu());
    menuBar->addMenu(createPatientMenu());
    menuBar->addMenu(createInfoMenu());

    mainWindow->setMenuBar(menuBar);
}

QMenu* MenuHandler::createFileMenu() {
    QMenu* fileMenu = new QMenu("Файл", mainWindow);
    QAction* saveAction = new QAction("Сохранить", mainWindow);
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    QAction* openAction = new QAction("Открыть", mainWindow);
    openAction->setShortcut(QKeySequence("Ctrl+D"));
    QAction* closeAction = new QAction("Закрыть", mainWindow);
    closeAction->setShortcut(QKeySequence("Ctrl+F"));
    QAction* exitAction = new QAction("Выход", mainWindow);

    fileMenu->addAction(saveAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(closeAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    connect(saveAction, &QAction::triggered, mainWindow, &MainWindow::saveToFileDialog);
    connect(openAction, &QAction::triggered, mainWindow, &MainWindow::loadFromFileDialog);
    connect(closeAction, &QAction::triggered, mainWindow, &MainWindow::closeData);
    connect(exitAction, &QAction::triggered, mainWindow, &MainWindow::exitApp);

    return fileMenu;
}

QMenu* MenuHandler::createPatientMenu() {
    QMenu* patientMenu = new QMenu("Операции", mainWindow);
    QAction* addAction = new QAction("Добавить пациента", mainWindow);
    QAction* editAction = new QAction("Редактировать пациента", mainWindow);
    QAction* removeAction = new QAction("Удалить пациента", mainWindow);
    removeAction->setShortcut(QKeySequence("Delete"));

    patientMenu->addAction(addAction);
    patientMenu->addAction(editAction);
    patientMenu->addAction(removeAction);

    connect(addAction, &QAction::triggered, mainWindow, &MainWindow::addPatient);
    connect(editAction, &QAction::triggered, mainWindow, &MainWindow::editPatient);
    connect(removeAction, &QAction::triggered, mainWindow, &MainWindow::removePatient);

    return patientMenu;
}

QMenu* MenuHandler::createInfoMenu() {
    QMenu* infoMenu = new QMenu("Справка", mainWindow);
    QAction* showAppInfoAction = new QAction("О приложении", mainWindow);
    infoMenu->addAction(showAppInfoAction);
    connect(showAppInfoAction, &QAction::triggered, mainWindow, &MainWindow::showAppInfo);

    return infoMenu;
}

