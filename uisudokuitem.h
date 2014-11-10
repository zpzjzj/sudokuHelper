#ifndef UISUDOKUITEM_H
#define UISUDOKUITEM_H

#include <QWidget>
#include <QLabel>
#include <memory>

namespace sudoku {
class SudokuItem;
}

namespace Ui {
class UiSudokuItem;
}

class UiSudokuItem : public QWidget
{
    Q_OBJECT

public:
    typedef std::unique_ptr<sudoku::SudokuItem> SudokuItemPtr;
    explicit UiSudokuItem(QWidget *parent = 0);
    void setSudokuItem(SudokuItemPtr&& item);

    ~UiSudokuItem();
public slots:
    void premark(int i);
    void setAvaliable(int i);
    void setUnavaliable(int i);
    void userRemove(int i);
    void set(int i);
    void unset();
// avaliable ui operations
private:
    void render();
    void showCandidate();
    void showSelected();
    void showPremarked();
    void hideCandidateItem(int idx);
    void showCandidateItem(int idx);
    void highlightCandicateItem(int idx);

    void renderPremark();
    void renderCandidate(int i);
    void renderSelected();
    void hideCandidates();
    void hideSelected();
    void setSelected(int i);
private:
    Ui::UiSudokuItem *ui;
    QLabel** labels;
    SudokuItemPtr item_;
};

#endif // UISUDOKUITEM_H
