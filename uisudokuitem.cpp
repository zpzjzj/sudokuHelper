#include "uisudokuitem.h"
#include "ui_uisudokuitem.h"
#include "sudokuitem.hpp"
#include <QLabel>
#include <cassert>

using namespace sudoku;

UiSudokuItem::UiSudokuItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiSudokuItem)
{
    ui->setupUi(this);

    labels = new QLabel*[10]{ui->label_0, ui->label_1, ui->label_2, ui->label_3, ui->label_4,
            ui->label_5, ui->label_6, ui->label_7, ui->label_8, ui->label_9};

    // labels[0]->setVisible(false);
    // ui->container->setVisible(false);
}

void UiSudokuItem::setSudokuItem(UiSudokuItem::SudokuItemPtr&& item)
{
    item_ = std::move(item);
    render();
}

UiSudokuItem::~UiSudokuItem()
{
    delete ui;
}

void UiSudokuItem::render() {
    if (item_->status() == itemstate::pre_marked) {
        showPremarked();
    }
}

void UiSudokuItem::premark(int i) {
    item_->setPremark(i);
    showPremarked();
}

void UiSudokuItem::setAvaliable(int i)
{
    item_->setAvaliable(i);
    showCandidateItem(i);
}

void UiSudokuItem::setUnavaliable(int i)
{
    item_->auto_remove(i);
    showCandidateItem(i);
}

void UiSudokuItem::userRemove(int i)
{
    item_->user_remove(i);
    showCandidateItem(i);
}

void UiSudokuItem::set(int i)
{

}

void UiSudokuItem::unset()
{

}

///// why do I declare these functions inline? because we'll never, also can not use it outside the class!!!

inline void UiSudokuItem::showPremarked() {
    labels[0]->setVisible(true);
    hideCandidates();
    setSelected(item_->value());
    renderPremark();
}

inline void UiSudokuItem::showCandidateItem(int idx) {
    labels[idx]->show();
    renderCandidate(idx)
}

inline void UiSudokuItem::showCandidates() {

}

inline void UiSudokuItem::hideCandidates() {
    ui->container->setVisible(false);
}

inline void UiSudokuItem::hideSelected() {
    labels[0]->setVisible(false);
}

inline void UiSudokuItem::setSelected(int i) {
    labels[0]->setText(QString::number(i));
}

inline void UiSudokuItem::renderPremark() {
    QFont font = labels[0]->font();
    font.setPointSize(20);
    font.setBold(true);
    labels[0]->setFont(font);
}

inline void UiSudokuItem::renderCandidate(int i) {
    QFont font = labels[i]->font();
    switch (items_->get(i)) {
        case itemstate::auto_remove:
            labels[i]->hide();
            break;
        case itemstate::avaliable:
            labels[i]->show();
            font.setOverline(false);
            font.setItalic(false);
            break;
        case itemstate::user_remove:
            font.setOverline(true);
            font.setItalic(true);
            break;
        default:
            // is an error
            assert(false);
            break;
    }
    labels[i]->setFont(font);
}
