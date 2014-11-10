#include "uisudokuitem.h"
#include "ui_uisudokuitem.h"
#include "sudokuitem.hpp"
#include "clickablelabel.h"
#include <QLabel>
#include <cassert>
#include <QDebug>
#include <algorithm>
#include <functional>

using namespace sudoku;

UiSudokuItem::UiSudokuItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiSudokuItem), item_(new SudokuItem)
{
    ui->setupUi(this);

    labels = new ClickableLabel*[10]{ui->label_0, ui->label_1, ui->label_2, ui->label_3, ui->label_4,
            ui->label_5, ui->label_6, ui->label_7, ui->label_8, ui->label_9};

    for (int i = 1; i < 10; ++i) {
        connect(labels[i], &ClickableLabel::clicked, [=](){set(i);} );
        connect(labels[i], &ClickableLabel::rclicked, [=](){toggle_remove(i);} );
    };

    connect(labels[0], &ClickableLabel::clicked, this, &UiSudokuItem::toggle_premark);
    connect(labels[0], &ClickableLabel::rclicked, this, &UiSudokuItem::unset);

    render();
}

void UiSudokuItem::toggle_premark() {
    if (item_->status() == itemstate::pre_marked)
        showSelected();
    else
        showPremarked();
}

void UiSudokuItem::toggle_remove(int i) {
    if (item_->get(i) == itemstate::user_remove) {
        item_->setAvaliable(i);
    } else {
        item_->userRemove(i);
    }
    renderCandidate(i);
}

void UiSudokuItem::setSudokuItem(UiSudokuItem::SudokuItemPtr&& item)
{
    item_ = std::move(item);
    render();
}

UiSudokuItem::~UiSudokuItem()
{
    delete ui;
    delete [] labels;
}

void UiSudokuItem::render() {
    if (item_->status() == itemstate::pre_marked) {
        showPremarked();
    } else if (item_->status() == itemstate::pending) {
        showCandidates();
    } else if (item_->status() == itemstate::selected) {
        showSelected();
    }
}

void UiSudokuItem::premark(int i) {
    item_->setPremark(i);
    showPremarked();
    emit valuePremarked(i);
}

void UiSudokuItem::setAvaliable(int i)
{
    item_->setAvaliable(i);
    showCandidateItem(i);
}

void UiSudokuItem::setUnavaliable(int i)
{
    item_->autoRemove(i);
    showCandidateItem(i);
}

void UiSudokuItem::userRemove(int i)
{
    item_->userRemove(i);
    showCandidateItem(i);
}

void UiSudokuItem::set(int i)
{
    item_->set(i);
    showSelected();
    emit valueSetted(i);
}

void UiSudokuItem::unset()
{
    int i = item_->value();
    item_->unset();
    showCandidates();
    emit valueUnsetted(i);
}

///// why do I declare these functions inline? It is because that we'll never, also not able to, use it outside the class!!!

inline void UiSudokuItem::showPremarked() {
    labels[0]->setVisible(true);
    hideCandidates();
    setSelected(item_->value());
    renderPremark();
    qDebug() << "nogood";
}

inline void UiSudokuItem::showCandidates() {
    ui->container->setVisible(true);
    hideSelected();

    for (int i = 1; i < 10; ++i)
        renderCandidate(i);
}

inline void UiSudokuItem::showSelected() {
    labels[0]->setVisible(true);
    hideCandidates();
    setSelected(item_->value());
    renderSelected();
}

inline void UiSudokuItem::showCandidateItem(int idx) {
    labels[idx]->show();
    renderCandidate(idx);
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
    QFont font; // = labels[0]->font();
    font.setPointSize(20);
    font.setBold(true);
    labels[0]->setFont(font);
    labels[0]->setStyleSheet("QLabel { background-color : #999999 } ");
}

inline void UiSudokuItem::renderCandidate(int i) {
    QFont font; // = labels[i]->font();
    switch (item_->get(i)) {
        case itemstate::auto_remove:
            labels[i]->hide();
            break;
        case itemstate::avaliable:
            labels[i]->show();
            font.setBold(true);
            labels[i]->setStyleSheet("");
            break;
        case itemstate::user_remove:
            labels[i]->show();
            font.setBold(true);
            labels[i]->setStyleSheet("QLabel { color: #aaaaaa }");
            break;
        default:
            // is an error
            assert(false);
            break;
    }
    labels[i]->setFont(font);
}

inline void UiSudokuItem::renderSelected() {
    QFont font;
    font.setPointSize(20);
    labels[0]->setFont(font);
    labels[0]->setStyleSheet("");
}
