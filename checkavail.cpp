#include <array>
#include <set>
#include <functional>
#include <cstdlib>
#include <algorithm>
#include <QDebug>
#include <iostream>
#include "mainwindow.h"
#include "uisudokuitem.h"
#include "sudokuitem.hpp"

typedef std::set<int> Avails;
//typedef std::array<Avails, 3> BlockRow;
//typedef std::array<BlockRow, 3> Block;
typedef std::array<std::array<Avails, 9>, 9> Table;
Table table;

typedef std::vector<Avails*> Area;
typedef std::function<void(Area&)> Functor;
void foreach_block(Functor functor) {
    for(int k = 0; k < 9; ++k) {
        Area area;
        auto div = std::div(k, 3);

        for(int i = 0; i < 3; ++i) {
            for(int j = 0; j < 3; ++j) {
                area.push_back(&table[div.quot * 3 + i][div.rem * 3 + j]);
            }
        }
        functor(area);
    }
}

void foreach_row(Functor functor) {
    for(int i = 0; i < 9; ++i) {
        // row
        Area area;
        for(int j = 0; j < 9; ++j) {
            area.push_back(&table[i][j]);
        }
        functor(area);
    }
}

void foreach_col(Functor functor) {
    for(int j = 0; j < 9; ++j) {
        Area area;
        for(int i = 0; i < 9; ++i) {
            area.push_back(&table[i][j]);
        }
        functor(area);
    }
}

void foreach_area(Functor functor) {
    foreach_block(functor);
    foreach_row(functor);
    foreach_col(functor);
}


std::set<int> get_settled(const Area& area) {
    std::set<int> settled;
    for(Avails* avails : area) {
        if(avails->size() == 1) {
            settled.insert(*avails->begin());
        }
    }
    return settled;
}

void remove_avail(Area& area) {
    std::set<int> settled = get_settled(area);
    for(Avails* avails : area) {
        if(avails->size() > 1) {
            std::set<int> result;
            std::set_difference(avails->begin(), avails->end(),
                                settled.begin(), settled.end(),
                                std::inserter(result, result.begin()));
            avails->swap(result);
        }
    }
}

std::set<int> find_closure(size_t level, std::vector<Avails*> lst, std::vector<Avails*> remain) {
    if(lst.size() == level) { // n sets union with n elements
        std::set<int> res;
        for(Avails* elem : lst) {
            std::set_union(elem->begin(), elem->end(),
                           res.begin(), res.end(),
                           std::inserter(res, res.begin()));
        }
        if(res.size() == level) {
            return res;
        } else {
            return {};
        }
    } else {
        remain.erase(std::remove_if(remain.begin(), remain.end(),
                       [&lst](Avails* elem){
            return std::any_of(lst.begin(), lst.end(), [&elem](Avails* elem_) {
                std::set<int> res;
                std::set_intersection(elem->begin(), elem->end(),
                                      elem_->begin(), elem_->end(),
                                      std::inserter(res, res.begin()));
                return res.size() == 0; // not intersect
            });
            }), remain.end());

        while(remain.size() > 0) { // try with remaining elements
            auto lst_new = lst;
            lst_new.push_back(std::move(remain.back()));
            remain.pop_back();
            auto res = find_closure(level, lst_new, remain);
            if(res.size() > 0) {
                return res;
            }
        }
    }
    return {};
}

void do_partition(int level, Area& area) {
    std::vector<Avails*> remain = area;
    while(remain.size() > 0) {
        std::vector<Avails*> lst = {remain.back()};
        remain.pop_back();
        std::set<int> partition = find_closure(level, lst, remain);
        if(partition.size() > 0) {
            qDebug() << "partition found";
            for(auto val : partition) {
                qDebug() << val << "";
            }
            remain.erase(std::remove_if(remain.begin(), remain.end(), [&partition](Avails* elem){
                return std::includes(partition.begin(), partition.end(), elem->begin(), elem->end());
            }), remain.end());
            for(Avails* elem_ptr : remain) {
                std::set<int> res;
                std::set_difference(elem_ptr->begin(), elem_ptr->end(), partition.begin(), partition.end(),
                                    std::inserter(res, res.begin()));
                elem_ptr->swap(res);
            }
        }
    }
}

void partition_avail(Area& area) {
    for(size_t i = 2; i <= 5; ++i) {
        do_partition(i, area);
    }
}

void unique_avail(Area& area) {
    std::set<int> settled = get_settled(area);
    for(int i = 1; i < 9; ++i) {
        int count = 0;
        Avails* last = nullptr;
        if(std::find(settled.begin(), settled.end(), i) == settled.end()) {
            // outside of settled
            for(Avails* avails : area) {
                if(std::find(avails->begin(), avails->end(), i) != avails->end()) {
                    ++count;
                    last = avails;
                }
            }
        }
        if(count == 1) { // unique
            last->clear();
            last->insert(i);
        }
    }
}

void eval_block() {
    foreach_area(remove_avail);
    foreach_area(unique_avail);
    foreach_area(partition_avail);
}


void set_table(MainWindow::UiSudokuBlock blocks[3][3]) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for(int bi = 0; bi < 3; ++bi)
                for(int bj = 0; bj < 3; ++bj) {
                    UiSudokuItem* ui_item = blocks[i][j][bi][bj];
                    using namespace sudoku;
                    const SudokuItem& item = ui_item->get();
                    itemstate status = item.status();
                    if(status == itemstate::pre_marked || status == itemstate::selected) {
                        table[i*3+bi][j*3+bj].insert(item.value());
                    } else {
                        for(int v = 1; v < 10; ++v) {
                            sudoku::itemstate state = item[v];
                            if(state != itemstate::user_remove) {
                                table[i*3+bi][j*3+bj].insert(v);
                            }
                        }
                    }
                }
}

void set_block(MainWindow::UiSudokuBlock blocks[3][3]) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for(int bi = 0; bi < 3; ++bi)
                for(int bj = 0; bj < 3; ++bj) {
                    UiSudokuItem* ui_item = blocks[i][j][bi][bj];
                    using namespace sudoku;
                    const SudokuItem& item = ui_item->get();
                    itemstate status = item.status();
                    if(status == itemstate::pre_marked || status == itemstate::selected) {
                        // do nothing
                    } else {
                        const Avails& avails = table[i*3+bi][j*3+bj];
                        for(int v = 1; v < 10; ++v) {
                            if(avails.find(v) != avails.end()) {
                                ui_item->setAvaliable(v);
                            } else {
                                ui_item->setUnavaliable(v);
                            }
                        }
                    }
                }
    for(int i = 0; i < 9; ++i) {
        for(int j = 0; j < 9; ++j) {
            table[i][j].clear();
        }
    }
}

void main_eval(MainWindow::UiSudokuBlock blocks[3][3]) {
    set_table(blocks);
    Table old;
    do {
        old = table;
        eval_block();
    }while(old != table);
    set_block(blocks);
}
