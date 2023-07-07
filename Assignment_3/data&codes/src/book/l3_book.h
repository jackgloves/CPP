#pragma once

#include <cstdint>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>

#include "util/price.h"

class L3Book {
 public:
    void ProcessAdd(int64_t order_id, bool is_buy, double price, int64_t qty);
    void ProcessReplace(int64_t order_id, bool is_buy, double price, int64_t qty);
    void ProcessDelete(int64_t order_id);
    void ProcessExec(int64_t order_id, int64_t exec_qty);

 public:
    template <typename F>
    void ForEachLevel(F&& f) const {
        // TODO
        auto buyItr = buy_side_.begin();
        auto sellItr = sell_side_.begin();

        while (buyItr != buy_side_.end() || sellItr != sell_side_.end()) {
            const double bid_price = (buyItr != buy_side_.end()) ? buyItr->second.price : 0.0;
            const int64_t bid_qty = (buyItr != buy_side_.end()) ? buyItr->second.qty : 0;
            const int64_t bid_count = (buyItr != buy_side_.end()) ? buyItr->second.count : 0;

            const double ask_price = (sellItr != sell_side_.end()) ? sellItr->second.price : 0.0;
            const int64_t ask_qty = (sellItr != sell_side_.end()) ? sellItr->second.qty : 0;
            const int64_t ask_count = (sellItr != sell_side_.end()) ? sellItr->second.count : 0;

            if (!f(bid_price, bid_qty, bid_count, ask_price, ask_qty, ask_count)) {
                break;
            }

            if (buyItr != buy_side_.end()) {
                ++buyItr;
            }
            if (sellItr != sell_side_.end()) {
                ++sellItr;
            }
        }
    }

    template <typename F>
    void ForEachOrder(bool is_buy, bool inner_to_outer, F&& f) const {
        // TODO
        if (is_buy && inner_to_outer) {
            for (auto buyItr = buy_side_.begin(); buyItr != buy_side_.end(); ++buyItr) {
                for (auto pos = buyItr->second.orders.rbegin(); pos != buyItr->second.orders.rend(); ++pos) {
                    f(is_buy, static_cast<double>(buyItr->first), buyItr->second.qty, (*pos)->qty, (*pos)->order_id);
                }
            }
        } else if (!is_buy && !inner_to_outer) {
            for (auto sellItr = sell_side_.rbegin(); sellItr != sell_side_.rend(); ++sellItr) {
                for (auto revItr = sellItr->second.orders.rbegin(); revItr != sellItr->second.orders.rend(); ++revItr) {
                    f(is_buy, static_cast<double>(sellItr->first), sellItr->second.qty, (*revItr)->qty, (*revItr)->order_id);
                }
            }
        }
    }

    // to uncross the book by removing any offensive orders from the given book side
    // - is_buy: true. If the sell side is newer, and any offensive orders on the buy side should be removed
    void UncrossBookSide(bool is_buy);

 private:
    struct Order;
    struct Level {
        using Orders = std::list<Order*>;

        Level(double price) 
            : price(price), qty(0), count(0) {}

        Orders orders;
        double price;
        int64_t qty;
        int64_t count;
    };
    struct Order {
        Order(int64_t order_id, bool is_buy, double price, int64_t qty)
            : order_id(order_id), is_buy(is_buy), price(price), qty(qty) {} 

        int64_t order_id;
        bool is_buy;
        double price;
        int64_t qty;

        Level::Orders::iterator pos;        
        Level* level;
    };

    using BuySide = std::map<Price, Level, std::greater<Price>>;
    using SellSide = std::map<Price, Level, std::less<Price>>;
    using Orders = std::unordered_map<int64_t, Order>;

 private:
    Orders orders_;
    BuySide buy_side_;
    SellSide sell_side_;
};

// the requirement, the ops time complexity should be less than O(N), i.e. either O(1) or O(logN)

// OrderBook Update operation
// - add order: find the level and append to the level end 
// - update order: 
//   1) if priority is unchanged, update the qty inplace
//   2) if priority is changed, remove(order) and add(order)
// - delete order: find the level and remove from the level 

// OrderBook Query operation
// - iterate the levels up to some provide level count, to get the price, qty, ocount for each level.
// 
