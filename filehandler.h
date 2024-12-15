#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include "Patient.h"

class FileHandler : public QObject {
    Q_OBJECT

public:
    explicit FileHandler(QObject* parent = nullptr);

    bool saveToFile(const QList<Patient>& patientList, const QString& filePath);  // Сохранить данные в файл
    bool loadFromFile(QList<Patient>& patientList, const QString& filePath);  // Загрузить данные из файла
    QString saveFileDialog();  // Показать диалог сохранения файла
    QString openFileDialog();  // Показать диалог открытия файла
};

#endif // FILEHANDLER_H
