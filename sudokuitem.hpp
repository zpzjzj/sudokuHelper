#ifndef SUDOKUITEM_HPP
#define SUDOKUITEM_HPP
#include <cassert>

#define ITEM_ASSERT(v) assert(v > 0 && v < 10)

namespace sudoku {

/**
 * @brief The itemstate enum
 *      available: current valid alternative
 *      user_remove: removed by user
 *      auto_remove: removed by program
 *      pre_marked: init at start as the value of the cell
 *      selected: choose as the value of the cell by player
 */
enum class itemstate { avaliable, user_remove, auto_remove, pre_marked, pending, selected};

class SudokuItem
{
public:
    SudokuItem() :s(v_[0]) {
        for (auto& each : v_)
            each = itemstate::avaliable;
        s = itemstate::pending;
    }
    
    SudokuItem(int v) :SudokuItem() {
        ITEM_ASSERT(v);
        s = itemstate::pre_marked;
        ck = v;
    }

    ~SudokuItem() = default;
// modifier
public:
    void setPremark(int v) { ITEM_ASSERT(v); ck = v; s = itemstate::pre_marked; }
    void setAvaliable(int v) { ITEM_ASSERT(v); v_[v] = itemstate::avaliable; }
    void autoRemove(int v) { ITEM_ASSERT(v); v_[v] = itemstate::auto_remove; }
    void userRemove(int v) { ITEM_ASSERT(v); v_[v] = itemstate::user_remove; }
    void set(int v) { ITEM_ASSERT(v); ck = v; s = itemstate::selected; }
    void unset() {
        s = itemstate::pending;
        for (auto& each : v_) {
            each = itemstate::avaliable;
        }
    }

// accessor
public:
    itemstate operator[](int idx) const { ITEM_ASSERT(idx); return v_[idx]; }
    itemstate status() const { return s; }
    itemstate get(int idx) const { ITEM_ASSERT(idx); return v_[idx]; }
    const itemstate* begin() const { return &v_[1]; }
    const itemstate* end() const { return &v_[10]; }
    int value() const { assert(s==itemstate::pre_marked || s==itemstate::selected ); return ck; }
private:
    itemstate v_[10];
    itemstate& s; // = v_[0]
    int ck; // user's choice of value
};

} // sudoku

#undef ITEM_ASSERT

#endif // SUDOKUITEM_HPP
