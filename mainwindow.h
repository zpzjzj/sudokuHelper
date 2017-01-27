#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class UiSudokuItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    typedef UiSudokuItem* UiSudokuBlock[3][3]; // 3 * 3 block restrict

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void update_set(int bi, int bj, int si, int sj, int v);
    void update_unset(int bi, int bj, int si, int sj, int v);
private:
    QWidget* initBlock(int row, int col, UiSudokuBlock& block);
private:
    Ui::MainWindow *ui;
    UiSudokuBlock blocks[3][3];
};

#endif // MAINWINDOW_H
