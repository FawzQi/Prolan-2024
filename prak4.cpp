#include <iostream>
#include <memory>
enum ProductId {
    MINE,
    YOURS
};
// Interface for objects created by the factory method.
class Product {
   public:
    virtual void print() = 0;
};
// Concrete implementations of the Product interface.
class ConcreteProductMINE : public Product {
   public:
    void print() override {
        std::cout << "this=" << this << " print MINE\n";
    }
};
class ConcreteProductYOURS : public Product {
   public:
    void print() override {
        std::cout << "this=" << this << " print YOURS\n";
    }
};
// Creator class with the factory method.
class Creator {
   public:
    virtual std::unique_ptr<Product> create(ProductId id) {
        if (ProductId::MINE == id)
            return std::make_unique<ConcreteProductMINE>();
        if (ProductId::YOURS == id)
            return std::make_unique<ConcreteProductYOURS>();
        // Add more cases for other product types...
        return nullptr;
    }
};
int main() {
    std::unique_ptr<Creator> creator = std::make_unique<Creator>();
    std::unique_ptr<Product> product = creator->create(ProductId::MINE);
    product->print();
    product->print();

    product = creator->create(ProductId::YOURS);
    product->print();
    product->print();
}