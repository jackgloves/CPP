#include "book/l3_book.h"
#include <stdexcept>

void L3Book::UncrossBookSide(bool is_buy) {
    // TODO
    std::list<int64_t> stale_orders;
    if (is_buy) {
        for (const auto& buyItr : buy_side_) {
            Price buy_price = buyItr.first;

            // Find sell orders with prices less than or equal to buy_price
            for (auto innerItr = sell_side_.rbegin(); innerItr != sell_side_.rend(); ++innerItr) {
                Price sell_price = innerItr->first;
                const Level& innerLevel = innerItr->second;

                if (sell_price <= buy_price) {
                    for (const auto& order : innerLevel.orders) {
                        stale_orders.push_back(order->order_id);
                    }
                }
            }
        }
    } else {
        for (const auto& sellItr : sell_side_) {
            Price sell_price = sellItr.first;

            // Find buy orders with prices greater than or equal to sell_price
            for (auto innerItr = buy_side_.begin(); innerItr != buy_side_.end(); ++innerItr) {
                Price buy_price = innerItr->first;
                const Level& innerLevel = innerItr->second;

                if (buy_price >= sell_price) {
                    for (const auto& order : innerLevel.orders) {
                        stale_orders.push_back(order->order_id);
                    }
                }
            }
        }
    }

    // Process delete for each order in the stale order list
    for (const auto& stale_order : stale_orders) {
        ProcessDelete(stale_order);
    }
}

void L3Book::ProcessAdd(int64_t order_id, bool is_buy, double price, int64_t qty) {
    auto add_to_side = [=](auto& side) {
        // find or create the price level  
        const auto [level_itr, new_level] = side.emplace(Price(price), Level(price));
        Level* level = &(level_itr->second);

        // save the order to orders_
        const auto [order_itr, new_order] = orders_.emplace(
            order_id, Order(order_id, is_buy, price, qty)
        );
        if (!new_order) {
            throw std::runtime_error("add order with duplicated order_id");
        }
        Order* order = &(order_itr->second);

        // update the level's summary data
        level->qty += qty;
        level->count += 1;

        // link Level with Order, note:
        // 1. list::push_front: O(1) operation while push_back is O(N)
        // 2. iterator invalidatio rule: 
        //       a) reference to element in std::unordered_map is stable, i.e. level
        //       b) reference to element in std::list is stable, i.e. order
        level->orders.push_front(&order_itr->second);  
        order->level = level;
        order->pos = level->orders.begin();
    };

    if (is_buy) {
        add_to_side(buy_side_);
    } else {
        add_to_side(sell_side_);
    }
    UncrossBookSide(!is_buy);
}

void L3Book::ProcessReplace(int64_t order_id, bool is_buy, double price, int64_t qty) {
    auto order_itr = orders_.find(order_id);
    if (order_itr == orders_.end()) {
        throw std::runtime_error("replace non-existing order");
    }
    auto& orig_order = order_itr->second;
    const bool inplace = Price(price) == Price(orig_order.price) && qty < orig_order.qty;
    if (inplace) {
        orig_order.level->qty -= (orig_order.qty - qty);
        orig_order.qty = qty;
    } else {
        ProcessDelete(order_id);
        ProcessAdd(order_id, is_buy, price, qty);
    }
}

void L3Book::ProcessDelete(int64_t order_id) {
    auto order_itr = orders_.find(order_id);
    if (order_itr == orders_.end()) {
        throw std::runtime_error("delete non-existing order");
    }
    const Order& order = order_itr->second;
    Level* level = order.level;

    // unlink Level with Order
    level->orders.erase(order.pos);

    // update the Level summary data
    level->qty -= order.qty;
    level->count -= 1;
    if (level->qty < 0 || level->count < 0) {
        throw std::runtime_error("delete more qty than available");
    }

    // delete the Level if no more orders
    auto delete_level = [] (auto& side, double price) {
        auto removed = side.erase(Price(price));
        if (removed == 0) {
            throw std::runtime_error("delete non-existing level");
        }
    };
    if (level->qty == 0) {
        if (order.is_buy) {
            delete_level(buy_side_, order.price);
        } else {
            delete_level(sell_side_, order.price);
        }
    }

    // delete the order
    orders_.erase(order_itr);
}

void L3Book::ProcessExec(int64_t order_id, int64_t exec_qty) {
    // TODO
    auto order_itr = orders_.find(order_id);
    if (order_itr == orders_.end()) {
        throw std::runtime_error("Execution for non-existing order");
    }

    Order& order = order_itr->second;
    int64_t order_qty = order.qty;

    if (exec_qty < order_qty) {
        int64_t remaining_qty = order_qty - exec_qty;
        ProcessReplace(order_id, order.is_buy, order.price, remaining_qty);
    } else if (exec_qty == order_qty) {
        ProcessDelete(order_id);
    } else {
        throw std::runtime_error("Execution quantity exceeds order quantity");
    }
}
