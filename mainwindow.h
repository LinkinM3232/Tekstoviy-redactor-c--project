#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QString>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Файл
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();

    // Формат
    void selectFont();

    // Масштаб
    void zoomIn();
    void zoomOut();

private:
    QTextEdit *textEdit;
    QString currentFileName;
};

#endif