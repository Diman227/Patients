#include "FileHandler.h"
#include <QFileDialog>

FileHandler::FileHandler(QObject* parent) : QObject(parent) {}

bool FileHandler::saveToFile(const QList<Patient>& patientList, const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Ошибка", "Не удалось открыть файл для записи");
        return false;
    }

    QTextStream out(&file);
    for (const Patient& patient : patientList) {
        out << patient.lastName << "\t" << patient.firstName << "\t" << patient.middleName << "\t"
            << patient.birthYear << "\t" << patient.height << "\t" << patient.weight << "\n";
    }

    file.close();
    return true;
}

bool FileHandler::loadFromFile(QList<Patient>& patientList, const QString& filePath) {
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
            Patient patient(parts[0], parts[1], parts[2], parts[3].toInt(), parts[4].toDouble(), parts[5].toDouble());
            patientList.append(patient);
        }
    }

    file.close();
    return true;
}

QString FileHandler::saveFileDialog() {
    return QFileDialog::getSaveFileName(nullptr, "Сохранить файл", "", "Text Files (*.txt);;All Files (*)");
}

QString FileHandler::openFileDialog() {
    return QFileDialog::getOpenFileName(nullptr, "Открыть файл", "", "Text Files (*.txt);;All Files (*)");
}
