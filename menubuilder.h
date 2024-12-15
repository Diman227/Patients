#ifndef MENUBUILDER_H
#define MENUBUILDER_H

#include <QObject>
#include <QMenu>
#include <QAction>
#include "MainWindow.h"

class MenuHandler : public QObject {
    Q_OBJECT

public:
    explicit MenuHandler(MainWindow* parent);

private:
    QMenu* createFileMenu();
    QMenu* createPatientMenu();
    QMenu* createInfoMenu();
    QMenu* createSearchMenu();
    MainWindow* mainWindow;
};

#endif // MENUBUILDER_H
