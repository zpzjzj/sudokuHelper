#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sudokuitem.hpp"
#include "uisudokuitem.h"
#include <QGridLayout>
#include <QFrame>
#include "checkavail.hpp"

static void initLayout(QGridLayout* layout) {
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    using namespace sudoku;
    typedef UiSudokuItem::SudokuItemPtr SudokuItemPtr;

    QFrame* w = new QFrame(ui->centralWidget);
    QGridLayout* glayout = new QGridLayout;
    glayout->setSpacing(0);
    glayout->setContentsMargins(0, 0, 0, 0);
    w->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    w->setFrameShape(QFrame::Box);
    w->setFrameShadow(QFrame::Plain);
    w->setLineWidth(3);
    for (int bi = 0; bi < 3; ++bi) {
        for (int bj = 0; bj < 3; ++bj) {
            QWidget* ret = initBlock(bi, bj, blocks[bi][bj] );
            glayout->addWidget(ret, bi, bj);
        } // for bj
    } // for bi
    w->setLayout(glayout);
}

QWidget* MainWindow::initBlock(int row, int col, UiSudokuBlock& block) {
    QFrame* widget = new QFrame(this);
    widget->setFrameShape(QFrame::Box);
    widget->setFrameShadow(QFrame::Plain);
    widget->setFrameStyle(2);
    widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QGridLayout* glayout = new QGridLayout(widget);
    glayout->setSpacing(1);
    glayout->setContentsMargins(0, 0, 0, 0);
    initLayout(glayout);

    for (int si = 0; si < 3; ++si) {
        for (int sj = 0; sj < 3; ++sj) {
            block[si][sj] = new UiSudokuItem(widget);
            auto& ref = block[si][sj];
            ref->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            connect(ref, &UiSudokuItem::valueSetted, [=](int v){ update_set(row, col, si, sj, v);});
            connect(ref, &UiSudokuItem::valueUnsetted, [=](int v){ update_unset(row, col, si, sj, v);});
            glayout->addWidget(ref, si, sj);
        }
    }

    return widget;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::update_set
 * @param bi block row
 * @param bj block col
 * @param si row in block
 * @param sj col in block
 * @param v
 */
void MainWindow::update_set(int bi, int bj, int si, int sj, int v)
{
    main_eval(blocks);
}

void MainWindow::update_unset(int bi, int bj, int si, int sj, int v)
{
    main_eval(blocks);
}
