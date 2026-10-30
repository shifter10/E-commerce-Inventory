# PhotoGear

## Brand-Affiliated E-Commerce Inventory & Search System

**Course:** Data Structure and Algorithm Analysis\
**Program:** BS in Computer Engineering\
**Project:** Final Laboratory Capstone\
**Language:** C++17\
**Domain:** Photography & Camera Equipment

------------------------------------------------------------------------

## 1. Project Overview

PhotoGear is an interactive, console-based e-commerce inventory and
search system for photography and camera equipment.

The project combines the major data structures and algorithms from Labs
1--8 into one C++ application. It manages a multi-brand product catalog,
filters products by partner brand, searches products, manages a shopping
cart and wishlist, processes customer orders, updates inventory, and
provides an admin undo feature.

### Partner Brands

-   Canon
-   Sony
-   Nikon
-   Fujifilm

------------------------------------------------------------------------

## 2. Product Model

Every product follows the required structure:

``` cpp
struct Product {
    int id;
    string name;
    string category;
    string brand;
    double price;
    int stockQuantity;
};
```

  Field             Purpose
  ----------------- -------------------------------
  `id`              Unique product identifier
  `name`            Product title
  `category`        Type of photography equipment
  `brand`           Official partner brand
  `price`           Price in Philippine pesos
  `stockQuantity`   Available inventory

------------------------------------------------------------------------

## 3. Lab 1--8 Integration

  -----------------------------------------------------------------------
  Lab                     Required Concept        PhotoGear
                                                  Implementation
  ----------------------- ----------------------- -----------------------
  Lab 1                   Dynamic Allocation      Dynamically allocated
                                                  and resizable product
                                                  catalog

  Lab 2                   Singly Linked List      Wishlist

  Lab 2                   Doubly Linked List      Shopping Cart

  Lab 3                   Circular Linked List    Partner Brand Showcase

  Lab 4                   Stack (LIFO)            Admin Inventory Undo

  Lab 5                   Queue (FIFO)            Customer Order
                                                  Processing

  Lab 6                   Binary Search Tree      Product ID Search

  Lab 7                   Searching Algorithms    Linear Keyword Search
                                                  and Binary Price Search

  Lab 8                   Sorting Algorithms      Quicksort and Insertion
                                                  Sort
  -----------------------------------------------------------------------

------------------------------------------------------------------------

## 4. Main Menu

``` text
[1] View Product Catalog
[2] Browse Products by Brand
[3] Search Products
[4] Search by Price Range
[5] Shopping Cart & Wishlist
[6] Checkout / Order Queue
[7] Admin: Undo Inventory Action
[8] Brand Showcase
[9] Sort Product Catalog
[10] Add Product (Dynamic Demo)
[0] Exit
```

### Feature Details

**1. View Product Catalog** --- Displays ID, product name, category,
brand, price, and stock.

**2. Browse Products by Brand** --- Filters products using the official
partner brand.

**3. Search Products** - Keyword Search = Linear Search - Product ID
Search = Binary Search Tree

**4. Search by Price Range** --- Makes a temporary copy, sorts it by
price using Quicksort, then uses binary search to find the requested
price boundaries.

**5. Shopping Cart & Wishlist** - Cart = Doubly Linked List - Wishlist =
Singly Linked List

**6. Checkout / Order Queue** - Orders use FIFO processing. - The oldest
pending order is processed first. - Inventory is changed only after the
complete order passes stock validation.

**7. Admin Undo** - Completed inventory actions are stored in a LIFO
stack. - The most recent completed action is undone first.

**8. Brand Showcase** - Uses a circular linked list. - Canon, Sony,
Nikon, and Fujifilm cycle repeatedly.

**9. Sort Product Catalog** - Price = Quicksort - Stock = Insertion
Sort - Brand = Insertion Sort

**10. Add Product** - Adds a product dynamically. - The catalog doubles
its capacity when full. - The BST is rebuilt after an addition because
catalog resizing can move product objects in memory.

------------------------------------------------------------------------

## 5. Data Structures

### Lab 1 --- Dynamic Array

`Catalog` owns a dynamically allocated `Product` array. When full, a
larger array is allocated, products are copied, and the old array is
released with `delete[]`.

### Lab 2 --- Singly Linked List

`Wishlist` uses nodes containing a product ID and a `next` pointer.

``` text
head -> node -> node -> node -> nullptr
```

### Lab 2 --- Doubly Linked List

`Cart` uses nodes containing product ID, quantity, `prev`, and `next`.

``` text
nullptr <- node <-> node <-> node -> nullptr
```

### Lab 3 --- Circular Linked List

`BrandCircle` connects the last brand back to the first brand.

``` text
Canon -> Sony -> Nikon -> Fujifilm
  ^                              |
  |______________________________|
```

### Lab 4 --- Stack

`UndoStack` follows **LIFO (Last In, First Out)**. The latest inventory
action is restored first.

### Lab 5 --- Queue

`OrderQueue` follows **FIFO (First In, First Out)**. The oldest pending
order is processed first.

### Lab 6 --- Binary Search Tree

`ProductBST` indexes products by unique ID. Average search is
approximately `O(log n)` for a reasonably balanced tree; a highly
unbalanced tree can become `O(n)`.

------------------------------------------------------------------------

## 6. Searching Algorithms --- Lab 7

### Linear Keyword Search

The program checks product name, category, and brand one product at a
time.

**Complexity:** `O(n)`

### Binary Price Search

The program sorts a temporary product array by price and uses binary
search to locate the lower and upper price boundaries.

**Boundary-search complexity:** `O(log n)`

------------------------------------------------------------------------

## 7. Sorting Algorithms --- Lab 8

### Quicksort

Used for price ordering and preparing the temporary array for binary
price searching.

-   Average: `O(n log n)`
-   Worst: `O(n²)`

### Insertion Sort

Used for stock and brand ordering.

-   Average/Worst: `O(n²)`

------------------------------------------------------------------------

## 8. Memory Management & Safety

Dynamic memory is used for the catalog and linked data structures.

The program releases allocated memory with: - `delete[]` for dynamic
arrays - `delete` for individual nodes - Destructors and cleanup
functions for data structures

The program handles: - Invalid numeric input - Negative
quantities/prices - Empty text - Duplicate product IDs - Duplicate
wishlist items - Missing product IDs - Empty cart/wishlist/queue -
Invalid price ranges - Insufficient stock

------------------------------------------------------------------------

## 9. Compilation

Using g++:

``` bash
g++ -std=c++17 -Wall -Wextra -pedantic ecommerce_inventory.cpp -o photogear
```

Windows:

``` text
photogear.exe
```

Linux/macOS:

``` text
./photogear
```

------------------------------------------------------------------------

## 10. Recommended Demonstration

For a short instructor demonstration:

1.  Choose **1** --- show the complete catalog.
2.  Choose **2** --- filter by `Canon`.
3.  Choose **3 → 1** --- search `lens` to demonstrate linear search.
4.  Choose **3 → 2** --- search ID `1004` to demonstrate the BST.
5.  Choose **4** --- enter `10000` to `30000` to demonstrate binary
    price search.
6.  Choose **5** --- add an item to the cart and another to the
    wishlist.
7.  Choose **6 → 1** --- submit the cart.
8.  Choose **6 → 2** --- show the FIFO order queue.
9.  Choose **6 → 3** --- process the order and show inventory
    decreasing.
10. Choose **7** --- undo the latest inventory action and show stock
    restored.
11. Choose **8** --- demonstrate the circular brand list.
12. Choose **9** --- demonstrate Quicksort or Insertion Sort.
13. Choose **10** --- add a product and explain dynamic catalog
    expansion/BST rebuilding.

------------------------------------------------------------------------

## 11. Submission Files

Recommended package:

``` text
PhotoGear_Project/
├── ecommerce_inventory.cpp
└── README.md
```

Optional:

``` text
└── photogear.exe
```

The `.cpp` file is the main source code. The README documents setup,
features, data structures, algorithms, Lab 1--8 integration, and the
recommended demonstration.

------------------------------------------------------------------------

## 12. Short Project Explanation

> PhotoGear is a console-based e-commerce inventory and search system
> focused on photography and camera equipment. It uses four partner
> brands: Canon, Sony, Nikon, and Fujifilm. The goal is to integrate the
> data structures and algorithms from Labs 1--8 into one working
> application. Dynamic allocation manages the catalog, linked lists
> manage the wishlist and cart, a circular list manages the brand
> showcase, a stack handles inventory undo, a queue processes customer
> orders, a BST searches product IDs, and the required searching and
> sorting algorithms are used for product lookup and organization.

------------------------------------------------------------------------

## 13. Conclusion

PhotoGear turns the laboratory concepts from Labs 1--8 into a practical
e-commerce scenario. Each required data structure has a specific role,
while input validation, inventory checks, pointer cleanup, and dynamic
memory management make the system safer and easier to demonstrate.
#   P h o t o g r a p h y - C a m e r a - E q u i p m e n t -  
 