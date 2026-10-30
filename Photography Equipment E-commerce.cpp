#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <utility>

using namespace std;

// ============================================================
// PHOTOGEAR - Brand-Affiliated E-Commerce Inventory System
// Domain: Photography & Camera Equipment
// C++17 | Data Structure and Algorithm Analysis Final Output
// ============================================================

// Required product model from the project specification.
struct Product {
    int id;                 // Unique identifier
    string name;            // Product title
    string category;        // Product category
    string brand;           // Official partner brand
    double price;           // Unit price in Philippine pesos
    int stockQuantity;      // Available inventory
};

// ============================================================
// LAB 1 - DYNAMIC ALLOCATION
// Dynamic, resizable array used as the product catalog.
// ============================================================
class Catalog {
private:
    Product* products;
    int count;
    int capacity;

    void grow() {
        int newCapacity = capacity * 2;
        Product* newProducts = new Product[newCapacity];

        for (int i = 0; i < count; ++i)
            newProducts[i] = products[i];

        delete[] products;
        products = newProducts;
        capacity = newCapacity;

        cout << "\n[Dynamic Memory] Catalog capacity is now "
             << capacity << ".\n";
    }

public:
    explicit Catalog(int initialCapacity = 16)
        : products(new Product[initialCapacity]), count(0), capacity(initialCapacity) {}

    ~Catalog() { delete[] products; }

    // Copying is disabled because Catalog owns dynamic memory.
    Catalog(const Catalog&) = delete;
    Catalog& operator=(const Catalog&) = delete;

    void add(const Product& product) {
        if (count == capacity)
            grow();
        products[count++] = product;
    }

    int size() const { return count; }
    int getCapacity() const { return capacity; }

    Product* data() { return products; }
    const Product* data() const { return products; }

    // Linear lookup used by several system features.
    Product* find(int id) {
        for (int i = 0; i < count; ++i)
            if (products[i].id == id)
                return &products[i];
        return nullptr;
    }

    const Product* find(int id) const {
        for (int i = 0; i < count; ++i)
            if (products[i].id == id)
                return &products[i];
        return nullptr;
    }

    void show() const {
        cout << "\n================ PRODUCT CATALOG ================\n";
        cout << left << setw(7) << "ID"
             << setw(27) << "Product"
             << setw(22) << "Category"
             << setw(14) << "Brand"
             << setw(14) << "Price"
             << setw(7) << "Stock" << '\n';
        cout << string(91, '-') << '\n';

        for (int i = 0; i < count; ++i) {
            cout << left << setw(7) << products[i].id
                 << setw(27) << products[i].name
                 << setw(22) << products[i].category
                 << setw(14) << products[i].brand
                 << "PHP " << right << setw(8) << fixed << setprecision(2)
                 << products[i].price << setw(6) << products[i].stockQuantity
                 << left << '\n';
        }

        cout << "Products: " << count << " | Capacity: " << capacity << '\n';
    }
};

// ============================================================
// LAB 2 - SINGLY LINKED LIST
// Wishlist implementation.
// ============================================================
struct WNode {
    int id;
    WNode* next;
    explicit WNode(int productId) : id(productId), next(nullptr) {}
};

class Wishlist {
private:
    WNode* head = nullptr;

public:
    ~Wishlist() { clear(); }

    void add(int id) {
        WNode* node = new WNode(id);
        node->next = head;
        head = node;
    }

    bool remove(int id) {
        WNode* previous = nullptr;
        WNode* current = head;

        while (current) {
            if (current->id == id) {
                if (previous)
                    previous->next = current->next;
                else
                    head = current->next;

                delete current;
                return true;
            }
            previous = current;
            current = current->next;
        }
        return false;
    }

    bool contains(int id) const {
        for (WNode* current = head; current; current = current->next)
            if (current->id == id)
                return true;
        return false;
    }

    void show(const Catalog& catalog) const {
        cout << "\n----- WISHLIST (Singly Linked List) -----\n";
        if (!head) {
            cout << "Wishlist is empty.\n";
            return;
        }

        for (WNode* current = head; current; current = current->next) {
            const Product* product = catalog.find(current->id);
            cout << "ID " << current->id << " - "
                 << (product ? product->name : "Unknown") << '\n';
        }
    }

    void clear() {
        while (head) {
            WNode* node = head;
            head = head->next;
            delete node;
        }
    }
};

// ============================================================
// LAB 2 - DOUBLY LINKED LIST
// Shopping cart implementation.
// ============================================================
struct CNode {
    int id;
    int quantity;
    CNode* prev;
    CNode* next;

    CNode(int productId, int qty)
        : id(productId), quantity(qty), prev(nullptr), next(nullptr) {}
};

class Cart {
private:
    CNode* head = nullptr;
    CNode* tail = nullptr;

public:
    ~Cart() { clear(); }

    CNode* find(int id) {
        for (CNode* current = head; current; current = current->next)
            if (current->id == id)
                return current;
        return nullptr;
    }

    bool add(int id, int quantity) {
        if (quantity <= 0)
            return false;

        CNode* existing = find(id);
        if (existing) {
            existing->quantity += quantity;
            return true;
        }

        CNode* node = new CNode(id, quantity);
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        return true;
    }

    bool remove(int id) {
        CNode* node = find(id);
        if (!node)
            return false;

        if (node->prev)
            node->prev->next = node->next;
        else
            head = node->next;

        if (node->next)
            node->next->prev = node->prev;
        else
            tail = node->prev;

        delete node;
        return true;
    }

    bool empty() const { return head == nullptr; }

    void clear() {
        while (head) {
            CNode* node = head;
            head = head->next;
            delete node;
        }
        tail = nullptr;
    }

    void show(const Catalog& catalog) const {
        cout << "\n----- SHOPPING CART (Doubly Linked List) -----\n";
        if (!head) {
            cout << "Cart is empty.\n";
            return;
        }

        double total = 0.0;
        for (CNode* current = head; current; current = current->next) {
            const Product* product = catalog.find(current->id);
            if (product) {
                double subtotal = product->price * current->quantity;
                cout << product->name << " x" << current->quantity
                     << " = PHP " << fixed << setprecision(2)
                     << subtotal << '\n';
                total += subtotal;
            }
        }
        cout << "Total: PHP " << fixed << setprecision(2) << total << '\n';
    }

    template <typename Function>
    void each(Function function) const {
        for (CNode* current = head; current; current = current->next)
            function(current->id, current->quantity);
    }
};

// ============================================================
// LAB 3 - CIRCULAR LINKED LIST
// Continuous partner-brand showcase.
// ============================================================
struct BNode {
    string brand;
    BNode* next;
    explicit BNode(string brandName)
        : brand(std::move(brandName)), next(nullptr) {}
};

class BrandCircle {
private:
    BNode* tail = nullptr;
    int brandCount = 0;

public:
    ~BrandCircle() { clear(); }

    void add(const string& brand) {
        BNode* node = new BNode(brand);

        if (!tail) {
            tail = node;
            node->next = node;
        } else {
            node->next = tail->next;
            tail->next = node;
            tail = node;
        }
        ++brandCount;
    }

    void show(int rounds = 2) const {
        if (!tail || brandCount == 0)
            return;

        cout << "\n----- BRAND SHOWCASE (Circular Linked List) -----\n";
        BNode* current = tail->next;
        int totalDisplays = brandCount * rounds;

        for (int i = 0; i < totalDisplays; ++i) {
            cout << "Featured Brand: " << current->brand << '\n';
            current = current->next;
        }
    }

    void clear() {
        if (!tail)
            return;

        BNode* head = tail->next;
        tail->next = nullptr; // Temporarily break the circle.

        while (head) {
            BNode* node = head;
            head = head->next;
            delete node;
        }

        tail = nullptr;
        brandCount = 0;
    }
};

// ============================================================
// LAB 5 - QUEUE (FIFO)
// Customer order processing.
// ============================================================
struct Order {
    int id;
    string customer;
    int productIds[10];
    int quantities[10];
    int itemCount;
    Order* next;

    Order(int orderId, string customerName)
        : id(orderId), customer(std::move(customerName)), itemCount(0), next(nullptr) {}
};

class OrderQueue {
private:
    Order* front = nullptr;
    Order* rear = nullptr;

public:
    ~OrderQueue() { clear(); }

    void push(Order* order) {
        if (!rear)
            front = rear = order;
        else {
            rear->next = order;
            rear = order;
        }
    }

    Order* pop() {
        if (!front)
            return nullptr;

        Order* order = front;
        front = front->next;
        order->next = nullptr;

        if (!front)
            rear = nullptr;

        return order;
    }

    bool empty() const { return front == nullptr; }

    void show() const {
        cout << "\n----- ORDER QUEUE (FIFO) -----\n";
        if (!front) {
            cout << "No pending orders.\n";
            return;
        }

        for (Order* current = front; current; current = current->next)
            cout << "Order #" << current->id << " | " << current->customer
                 << " | " << current->itemCount << " item(s)\n";
    }

    void clear() {
        while (front) {
            Order* order = front;
            front = front->next;
            delete order;
        }
        rear = nullptr;
    }
};

// ============================================================
// LAB 4 - STACK (LIFO)
// Stores previous inventory values for admin undo.
// ============================================================
struct UndoAction {
    int orderId;
    int itemCount;
    int productIds[10];
    int oldStock[10];
    UndoAction* next;

    UndoAction(int id, int count)
        : orderId(id), itemCount(count), next(nullptr) {}
};

class UndoStack {
private:
    UndoAction* top = nullptr;

public:
    ~UndoStack() { clear(); }

    void push(UndoAction* action) {
        action->next = top;
        top = action;
    }

    UndoAction* pop() {
        if (!top)
            return nullptr;

        UndoAction* action = top;
        top = top->next;
        action->next = nullptr;
        return action;
    }

    void clear() {
        while (top) {
            UndoAction* action = top;
            top = top->next;
            delete action;
        }
    }
};

// ============================================================
// LAB 6 - BINARY SEARCH TREE
// Product ID index for BST searching.
// ============================================================
struct TreeNode {
    int id;
    Product* product;
    TreeNode* left;
    TreeNode* right;

    explicit TreeNode(Product* productPointer)
        : id(productPointer->id), product(productPointer),
          left(nullptr), right(nullptr) {}
};

class ProductBST {
private:
    TreeNode* root = nullptr;

    TreeNode* insert(TreeNode* node, Product* product) {
        if (!node)
            return new TreeNode(product);

        if (product->id < node->id)
            node->left = insert(node->left, product);
        else if (product->id > node->id)
            node->right = insert(node->right, product);

        return node;
    }

    Product* search(TreeNode* node, int id) const {
        if (!node)
            return nullptr;
        if (id == node->id)
            return node->product;
        return (id < node->id) ? search(node->left, id)
                               : search(node->right, id);
    }

    void destroy(TreeNode* node) {
        if (!node)
            return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    ~ProductBST() { destroy(root); }

    /*
        Rebuild the tree after a catalog addition. The catalog uses
        a dynamic array, so a resize may move Product objects to a
        new memory address. Rebuilding refreshes all Product* links.
    */
    void rebuild(Catalog& catalog) {
        destroy(root);
        root = nullptr;

        for (int i = 0; i < catalog.size(); ++i)
            root = insert(root, &catalog.data()[i]);
    }

    Product* search(int id) const { return search(root, id); }
};

// ============================================================
// LAB 8 - SORTING ALGORITHMS
// Quicksort and Insertion Sort.
// ============================================================
bool priceCompare(const Product& a, const Product& b) {
    return (a.price == b.price) ? a.id < b.id : a.price < b.price;
}

bool stockCompare(const Product& a, const Product& b) {
    return (a.stockQuantity == b.stockQuantity)
               ? a.id < b.id
               : a.stockQuantity < b.stockQuantity;
}

bool brandCompare(const Product& a, const Product& b) {
    return (a.brand == b.brand) ? a.name < b.name : a.brand < b.brand;
}

void insertionSort(
    Product* array,
    int size,
    bool (*compare)(const Product&, const Product&)
) {
    for (int i = 1; i < size; ++i) {
        Product key = array[i];
        int j = i - 1;

        while (j >= 0 && compare(key, array[j])) {
            array[j + 1] = array[j];
            --j;
        }
        array[j + 1] = key;
    }
}

int partitionArray(
    Product* array,
    int low,
    int high,
    bool (*compare)(const Product&, const Product&)
) {
    Product pivot = array[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (compare(array[j], pivot)) {
            ++i;
            swap(array[i], array[j]);
        }
    }

    swap(array[i + 1], array[high]);
    return i + 1;
}

void quickSort(
    Product* array,
    int low,
    int high,
    bool (*compare)(const Product&, const Product&)
) {
    if (low >= high)
        return;

    int pivotIndex = partitionArray(array, low, high, compare);
    quickSort(array, low, pivotIndex - 1, compare);
    quickSort(array, pivotIndex + 1, high, compare);
}

// ============================================================
// INPUT AND DISPLAY HELPERS
// ============================================================
int readInt(const string& message, int minimum = 0) {
    int value;

    while (true) {
        cout << message;
        if (cin >> value && value >= minimum) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid input.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readDouble(const string& message, double minimum = 0.0) {
    double value;

    while (true) {
        cout << message;
        if (cin >> value && value >= minimum) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid amount.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readText(const string& message) {
    string text;
    do {
        cout << message;
        getline(cin, text);
        if (text.empty())
            cout << "Input cannot be empty.\n";
    } while (text.empty());
    return text;
}

string toLowerCase(string text) {
    transform(text.begin(), text.end(), text.begin(),
              [](unsigned char character) {
                  return static_cast<char>(tolower(character));
              });
    return text;
}

void printProduct(const Product& product) {
    cout << "ID " << product.id
         << " | " << product.name
         << " | " << product.category
         << " | " << product.brand
         << " | PHP " << fixed << setprecision(2) << product.price
         << " | Stock " << product.stockQuantity << '\n';
}

// ============================================================
// SEARCH AND FILTER FEATURES
// ============================================================
void filterByBrand(const Catalog& catalog) {
    string brand = toLowerCase(readText("Brand: "));
    bool found = false;

    for (int i = 0; i < catalog.size(); ++i) {
        if (toLowerCase(catalog.data()[i].brand) == brand) {
            printProduct(catalog.data()[i]);
            found = true;
        }
    }

    if (!found)
        cout << "No products found for that brand.\n";
}

void searchMenu(Catalog& catalog, ProductBST& bst) {
    cout << "\n1. Keyword Search (Linear)\n";
    cout << "2. Product ID Search (BST)\n";

    int choice = readInt("Choice: ", 1);

    if (choice == 1) {
        string keyword = toLowerCase(readText("Keyword: "));
        bool found = false;

        for (int i = 0; i < catalog.size(); ++i) {
            string searchableText = toLowerCase(
                catalog.data()[i].name + " " +
                catalog.data()[i].category + " " +
                catalog.data()[i].brand
            );

            if (searchableText.find(keyword) != string::npos) {
                printProduct(catalog.data()[i]);
                found = true;
            }
        }

        if (!found)
            cout << "No matches.\n";

    } else if (choice == 2) {
        int id = readInt("Product ID: ", 1);
        Product* product = bst.search(id);

        if (product)
            printProduct(*product);
        else
            cout << "Product not found.\n";

    } else {
        cout << "Invalid option.\n";
    }
}

// Sorts a temporary copy, then uses binary search for both boundaries.
void searchByPriceRange(const Catalog& catalog) {
    double minimumPrice = readDouble("Minimum price: PHP ");
    double maximumPrice = readDouble("Maximum price: PHP ");

    if (maximumPrice < minimumPrice) {
        cout << "Invalid price range.\n";
        return;
    }

    int size = catalog.size();
    Product* temporary = new Product[size];

    for (int i = 0; i < size; ++i)
        temporary[i] = catalog.data()[i];

    quickSort(temporary, 0, size - 1, priceCompare);

    // Lower bound: first price >= minimumPrice.
    int left = 0;
    int right = size;
    while (left < right) {
        int middle = (left + right) / 2;
        if (temporary[middle].price < minimumPrice)
            left = middle + 1;
        else
            right = middle;
    }
    int first = left;

    // Upper bound: first price > maximumPrice.
    left = 0;
    right = size;
    while (left < right) {
        int middle = (left + right) / 2;
        if (temporary[middle].price <= maximumPrice)
            left = middle + 1;
        else
            right = middle;
    }
    int last = left;

    cout << "\nPrice range results:\n";
    if (first == last)
        cout << "No products found.\n";
    else
        for (int i = first; i < last; ++i)
            printProduct(temporary[i]);

    delete[] temporary;
}

// ============================================================
// CART AND WISHLIST MENU
// ============================================================
void shopMenu(Catalog& catalog, Cart& cart, Wishlist& wishlist) {
    while (true) {
        cout << "\n1. Add Cart\n"
             << "2. Remove Cart\n"
             << "3. View Cart\n"
             << "4. Add Wishlist\n"
             << "5. Remove Wishlist\n"
             << "6. View Wishlist\n"
             << "7. Back\n";

        int choice = readInt("Choice: ", 1);

        if (choice == 1) {
            int id = readInt("Product ID: ", 1);
            Product* product = catalog.find(id);

            if (!product) {
                cout << "Product not found.\n";
                continue;
            }

            int quantity = readInt("Quantity: ", 1);
            if (quantity > product->stockQuantity)
                cout << "Insufficient stock.\n";
            else {
                cart.add(id, quantity);
                cout << "Added to cart.\n";
            }

        } else if (choice == 2) {
            int id = readInt("Product ID: ", 1);
            cout << (cart.remove(id) ? "Removed from cart.\n"
                                      : "Product is not in the cart.\n");

        } else if (choice == 3) {
            cart.show(catalog);

        } else if (choice == 4) {
            int id = readInt("Product ID: ", 1);
            if (!catalog.find(id))
                cout << "Product not found.\n";
            else if (wishlist.contains(id))
                cout << "Product is already in the wishlist.\n";
            else {
                wishlist.add(id);
                cout << "Added to wishlist.\n";
            }

        } else if (choice == 5) {
            int id = readInt("Product ID: ", 1);
            cout << (wishlist.remove(id) ? "Removed from wishlist.\n"
                                          : "Product is not in the wishlist.\n");

        } else if (choice == 6) {
            wishlist.show(catalog);

        } else if (choice == 7) {
            return;

        } else {
            cout << "Invalid option.\n";
        }
    }
}

// ============================================================
// CHECKOUT AND ORDER PROCESSING
// Lab 5: FIFO queue
// Lab 4: LIFO undo stack
// ============================================================
void checkoutMenu(
    Catalog& catalog,
    Cart& cart,
    OrderQueue& queue,
    UndoStack& undoStack,
    int& nextOrderId
) {
    while (true) {
        cout << "\n1. Submit Cart\n"
             << "2. View Queue\n"
             << "3. Process Next Order\n"
             << "4. Back\n";

        int choice = readInt("Choice: ", 1);

        if (choice == 1) {
            if (cart.empty()) {
                cout << "Cart is empty.\n";
                continue;
            }

            string customerName = readText("Customer name: ");
            Order* order = new Order(nextOrderId++, customerName);

            cart.each([&](int id, int quantity) {
                if (order->itemCount < 10) {
                    order->productIds[order->itemCount] = id;
                    order->quantities[order->itemCount] = quantity;
                    ++order->itemCount;
                }
            });

            queue.push(order);
            cart.clear();
            cout << "Order queued successfully.\n";

        } else if (choice == 2) {
            queue.show();

        } else if (choice == 3) {
            Order* order = queue.pop();
            if (!order) {
                cout << "No pending orders.\n";
                continue;
            }

            // Validate the entire order before changing inventory.
            bool valid = true;
            for (int i = 0; i < order->itemCount; ++i) {
                Product* product = catalog.find(order->productIds[i]);
                if (!product || order->quantities[i] > product->stockQuantity) {
                    valid = false;
                    break;
                }
            }

            if (!valid) {
                cout << "Order rejected: insufficient stock or missing product. "
                        "Stock unchanged.\n";
                delete order;
                continue;
            }

            // Save old stock values so the completed order can be undone.
            UndoAction* action = new UndoAction(order->id, order->itemCount);

            for (int i = 0; i < order->itemCount; ++i) {
                Product* product = catalog.find(order->productIds[i]);
                action->productIds[i] = product->id;
                action->oldStock[i] = product->stockQuantity;
                product->stockQuantity -= order->quantities[i];
            }

            undoStack.push(action);

            cout << "Order #" << order->id
                 << " completed for " << order->customer << ".\n";
            delete order;

        } else if (choice == 4) {
            return;

        } else {
            cout << "Invalid option.\n";
        }
    }
}

// Restores inventory from the most recently completed order.
void undoLastInventoryAction(Catalog& catalog, UndoStack& undoStack) {
    UndoAction* action = undoStack.pop();

    if (!action) {
        cout << "Nothing to undo.\n";
        return;
    }

    for (int i = 0; i < action->itemCount; ++i) {
        Product* product = catalog.find(action->productIds[i]);
        if (product)
            product->stockQuantity = action->oldStock[i];
    }

    cout << "Undo successful for Order #" << action->orderId << ".\n";
    delete action;
}

// ============================================================
// LAB 8 - SORT MENU
// ============================================================
void sortMenu(const Catalog& catalog) {
    int size = catalog.size();
    Product* temporary = new Product[size];

    for (int i = 0; i < size; ++i)
        temporary[i] = catalog.data()[i];

    cout << "\n1. Price - Quicksort\n"
         << "2. Stock - Insertion Sort\n"
         << "3. Brand - Insertion Sort\n";

    int choice = readInt("Choice: ", 1);

    if (choice == 1)
        quickSort(temporary, 0, size - 1, priceCompare);
    else if (choice == 2)
        insertionSort(temporary, size, stockCompare);
    else if (choice == 3)
        insertionSort(temporary, size, brandCompare);
    else {
        cout << "Invalid option.\n";
        delete[] temporary;
        return;
    }

    cout << "\n----- SORTED PRODUCTS -----\n";
    for (int i = 0; i < size; ++i)
        printProduct(temporary[i]);

    delete[] temporary;
}

// ============================================================
// INITIAL DATA
// Four partner brands: Canon, Sony, Nikon, Fujifilm.
// ============================================================
void seedCatalog(Catalog& catalog) {
    Product initialProducts[] = {
        {1001, "EOS R50", "Mirrorless Camera", "Canon", 42999, 8},
        {1002, "Alpha a6400", "Mirrorless Camera", "Sony", 49999, 6},
        {1003, "Z50", "Mirrorless Camera", "Nikon", 46999, 7},
        {1004, "X-S20", "Mirrorless Camera", "Fujifilm", 79999, 5},
        {1005, "RF 50mm F1.8", "Camera Lens", "Canon", 12999, 10},
        {1006, "FE 50mm F1.8", "Camera Lens", "Sony", 14999, 9},
        {1007, "NIKKOR Z 50mm", "Camera Lens", "Nikon", 16999, 7},
        {1008, "XF 35mm F2", "Camera Lens", "Fujifilm", 21999, 6},
        {1009, "Speedlite EL-100", "Flash", "Canon", 11999, 5},
        {1010, "GP-VPT2BT", "Camera Grip", "Sony", 8999, 8},
        {1011, "NIKKOR Z Battery", "Camera Battery", "Nikon", 4999, 12},
        {1012, "Instax Camera Bag", "Camera Bag", "Fujifilm", 3999, 15}
    };

    const int productCount =
        static_cast<int>(sizeof(initialProducts) / sizeof(initialProducts[0]));

    for (int i = 0; i < productCount; ++i)
        catalog.add(initialProducts[i]);
}

// ============================================================
// MAIN PROGRAM
// ============================================================
int main() {
    Catalog catalog;
    Wishlist wishlist;
    Cart cart;
    OrderQueue orderQueue;
    UndoStack undoStack;
    ProductBST productBST;
    BrandCircle brandShowcase;

    seedCatalog(catalog);

    brandShowcase.add("Canon");
    brandShowcase.add("Sony");
    brandShowcase.add("Nikon");
    brandShowcase.add("Fujifilm");

    productBST.rebuild(catalog);

    int nextOrderId = 5001;

    cout << "\nWelcome to PhotoGear!\n";

    while (true) {
        cout << "\n================================================\n"
             << "          PHOTOGEAR E-COMMERCE SYSTEM\n"
             << "       Photography & Camera Equipment\n"
             << "================================================\n"
             << "[1] View Product Catalog\n"
             << "[2] Browse Products by Brand\n"
             << "[3] Search Products\n"
             << "[4] Search by Price Range\n"
             << "[5] Shopping Cart & Wishlist\n"
             << "[6] Checkout / Order Queue\n"
             << "[7] Admin: Undo Inventory Action\n"
             << "[8] Brand Showcase\n"
             << "[9] Sort Product Catalog\n"
             << "[10] Add Product (Dynamic Demo)\n"
             << "[0] Exit\n"
             << "================================================\n";

        int choice = readInt("Enter choice: ", 0);

        if (choice == 1) {
            catalog.show();
        } else if (choice == 2) {
            filterByBrand(catalog);
        } else if (choice == 3) {
            searchMenu(catalog, productBST);
        } else if (choice == 4) {
            searchByPriceRange(catalog);
        } else if (choice == 5) {
            shopMenu(catalog, cart, wishlist);
        } else if (choice == 6) {
            checkoutMenu(catalog, cart, orderQueue, undoStack, nextOrderId);
        } else if (choice == 7) {
            undoLastInventoryAction(catalog, undoStack);
        } else if (choice == 8) {
            brandShowcase.show();
        } else if (choice == 9) {
            sortMenu(catalog);
        } else if (choice == 10) {
            int id = readInt("ID: ", 1);

            if (catalog.find(id)) {
                cout << "ID already exists.\n";
                continue;
            }

            string name = readText("Name: ");
            string category = readText("Category: ");
            string brand = readText("Brand: ");
            double price = readDouble("Price: PHP ");
            int stock = readInt("Stock: ", 0);

            catalog.add({id, name, category, brand, price, stock});

            // Rebuild BST because a catalog resize may move Product objects.
            productBST.rebuild(catalog);

            cout << "Product added successfully.\n";
        } else {
            cout << "Thank you for using PhotoGear.\n";
            return 0;
        }
    }
}
