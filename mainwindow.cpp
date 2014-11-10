#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sudokuitem.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    using namespace sudoku;
    typedef UiSudokuItem::SudokuItemPtr SudokuItemPtr;

    SudokuItemPtr item(new SudokuItem(7));

    ui->widget->setSudokuItem(std::move(item));
}

MainWindow::~MainWindow()
{
    delete ui;
}
