#include "order_manager.h"
#include <algorithm>
#include <iostream>

void OrderManager::Add(int price, int qty, const std::string& order_id, bool is_sell) {
    orders_.push_back(Order{price, qty, order_id, is_sell});
}

void OrderManager::Update(const std::string& order_id, int updated_open_qty) {
    auto it = std::find_if(orders_.begin(), orders_.end(), [&](const Order& order) {
        return order.order_id == order_id;
    });

    if (it != orders_.end()) {
        it->open_qty = updated_open_qty;
        if (it->open_qty == 0)
            orders_.erase(it);
    } else {
        std::cout << "Order not found." << std::endl;
    }
}

void OrderManager::PrintAllOrders() const {
    std::vector<Order> sortedOrders = orders_;
    std::sort(sortedOrders.begin(), sortedOrders.end(), [](const Order& a, const Order& b) {
        if (a.price != b.price)
            return a.price < b.price;
        return a.order_id < b.order_id;
    });

    for (const Order& order : sortedOrders) {
        std::cout << order << std::endl;
    }
}
