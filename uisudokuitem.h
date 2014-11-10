#ifndef UISUDOKUITEM_H
#define UISUDOKUITEM_H

#include <QWidget>
#include <QLabel>
#include <memory>

namespace sudoku {
class SudokuItem;
}

class ClickableLabel;

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
public:
    const sudoku::SudokuItem& get() const { return *item_; }
public slots:
    void premark(int i);
    void setAvaliable(int i);
    void setUnavaliable(int i);
    void userRemove(int i);
    void set(int i);
    void unset();
signals:
    void valueSetted(int v);
    void valueUnsetted(int v);
    void valuePremarked(int v);
private slots:
    void toggle_remove(int idx);
    void toggle_premark();
// avaliable ui operations
private:
    void render();
    void showSelected();
    void showPremarked();
    void showCandidates();
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
    ClickableLabel** labels;
    SudokuItemPtr item_;
};

#endif // UISUDOKUITEM_H
