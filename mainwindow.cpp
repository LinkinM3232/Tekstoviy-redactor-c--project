#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFontDialog>
#include <QKeySequence>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 1 Создаем главный центральный виджет и вертикальный слой для него
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(2, 2, 2, 2); // Минимальные отступы по краям окна
    mainLayout->setSpacing(5); // Расстояние между текстом и кнопками

    // 2 Создаем текстовое поле и добавляем его в верхнюю часть главного слоя
    textEdit = new QTextEdit(this);
    QFont defaultFont("Courier New", 11);
    textEdit->setFont(defaultFont);
    mainLayout->addWidget(textEdit);

    // 3 Создаем нижнюю панель для кнопок масштаба
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(0, 0, 5, 5); // Отступы для панели кнопок

    QPushButton *btnMinus = new QPushButton("-", this);
    QPushButton *btnPlus = new QPushButton("+", this);

    // квадратные кнопки(30x30)
    btnMinus->setFixedSize(30, 30);
    btnPlus->setFixedSize(30, 30);

    // Привязываем горячие клавиши прямо к кнопкам, чтобы Ctrl+ и Ctrl- работали
    btnMinus->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Minus));
    btnPlus->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Plus));

    // Добавляем элементы в нижний слой
    bottomLayout->addStretch(); // Эта "пружина" выталкивает всё, что идет после неё, в правый угол
    bottomLayout->addWidget(btnMinus);
    bottomLayout->addWidget(btnPlus);

    // Добавляем нижнюю панель в самый низ главного вертикального слоя
    mainLayout->addLayout(bottomLayout);

    // 4 Верхнее меню
    QMenuBar *menuBar = this->menuBar();

    // Меню "Файл"
    QMenu *fileMenu = menuBar->addMenu("Файл");
    QAction *actionNew = fileMenu->addAction("Создать");
    actionNew->setShortcut(QKeySequence::New);
    QAction *actionOpen = fileMenu->addAction("Открыть...");
    actionOpen->setShortcut(QKeySequence::Open);
    QAction *actionSave = fileMenu->addAction("Сохранить");
    actionSave->setShortcut(QKeySequence::Save);
    QAction *actionSaveAs = fileMenu->addAction("Сохранить как...");
    actionSaveAs->setShortcut(QKeySequence::SaveAs);
    fileMenu->addSeparator();
    QAction *actionExit = fileMenu->addAction("Выход");
    actionExit->setShortcut(QKeySequence::Quit);

    // Меню "Формат"
    QMenu *formatMenu = menuBar->addMenu("Формат");
    QAction *actionFont = formatMenu->addAction("Шрифт...");

    // 5 Подключение сигналов
    connect(actionNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::saveFileAs);
    connect(actionExit, &QAction::triggered, this, &QWidget::close);
    connect(actionFont, &QAction::triggered, this, &MainWindow::selectFont);

    // подключение кнопок масштаба к слотам
    connect(btnMinus, &QPushButton::clicked, this, &MainWindow::zoomOut);
    connect(btnPlus, &QPushButton::clicked, this, &MainWindow::zoomIn);

    resize(800, 600);
    setWindowTitle("ScalaPENdra - Новый файл");
}

MainWindow::~MainWindow() {}

// Ниже логика работы с файлами
void MainWindow::newFile() {
    textEdit->clear();
    currentFileName.clear();
    setWindowTitle("ScalaPENdra - Новый файл");
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QTextStream in(&file);
    textEdit->setPlainText(in.readAll());
    file.close();

    currentFileName = fileName;
    setWindowTitle("Блокнот - " + currentFileName);
}

void MainWindow::saveFile() {
    if (currentFileName.isEmpty()) {
        saveFileAs();
        return;
    }

    QFile file(currentFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл");
        return;
    }

    QTextStream out(&file);
    out << textEdit->toPlainText();
    file.close();
}

void MainWindow::saveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл как", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty()) return;

    currentFileName = fileName;
    saveFile();
    setWindowTitle("Блокнот - " + currentFileName);
}

void MainWindow::selectFont() {
    bool ok;
    QFont font = QFontDialog::getFont(&ok, textEdit->font(), this, "Выберите шрифт");
    if (ok) {
        textEdit->setFont(font);
    }
}

void MainWindow::zoomIn() {
    textEdit->zoomIn(2);
}

void MainWindow::zoomOut() {
    if (textEdit->font().pointSize() > 4) {
        textEdit->zoomOut(2);
    }
}