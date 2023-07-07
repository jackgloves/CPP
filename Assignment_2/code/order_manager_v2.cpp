#include "order_manager.h"
#include <algorithm>
#include <iostream>

void OrderManager::Add(int price, int qty, const std::string& order_id, bool is_sell) {
    Order newOrder(price, qty, order_id, is_sell);

    auto it = std::lower_bound(orders_.begin(), orders_.end(), newOrder, [](const Order& a, const Order& b) {
        if (a.price != b.price)
            return a.price < b.price;
        return a.order_id < b.order_id;
    });

    orders_.insert(it, newOrder);
}

void OrderManager::Update(const std::string& order_id, int updated_open_qty) {
    auto it = std::find_if(orders_.begin(), orders_.end(), [&](const Order& order) {
        return order.order_id == order_id;
    });

    if (it != orders_.end()) {
        it->open_qty = updated_open_qty;

        if (it->open_qty == 0) {
            orders_.erase(it);
        }
    }
}

void OrderManager::PrintAllOrders() const {
    for (const auto& order : orders_) {
        std::cout << order << std::endl;
    }
}
