#include <iostream>
#include <string>

using namespace std;

// ==========================================
// 1. DATA STRUCTURES
// ==========================================

// Node for our Inventory Linked List
struct ProductNode {
    int id;
    string name;
    double price;
    int quantity;
    int threshold;
    ProductNode* next; // Pointer to the next item
};

// Node for our Undo Stack
struct TransactionNode {
    int productId;
    int quantitySold;
    TransactionNode* next; // Pointer to the previous sale in the stack
};

// ==========================================
// 2. HELPER FUNCTIONS & OPERATIONS
// ==========================================

// Function to add a product to the front of the Linked List
void addProduct(ProductNode*& head, int id, string name, double price, int qty, int threshold) {
    ProductNode* newNode = new ProductNode();
    newNode->id = id;
    newNode->name = name;
    newNode->price = price;
    newNode->quantity = qty;
    newNode->threshold = threshold;
    newNode->next = head;
    
    head = newNode; // New node becomes the head
    cout << "Success: Product added to inventory.\n";
}

// Function to display all products in the Linked List
void displayInventory(ProductNode* head) {
    if (head == nullptr) {
        cout << "\nInventory is empty.\n";
        return;
    }

    cout << "\n--- INVENTORY LIST ---\n";
    ProductNode* current = head;
    while (current != nullptr) {
        cout << "ID: " << current->id 
             << " | Name: " << current->name 
             << " | Price: $" << current->price 
             << " | Stock: " << current->quantity 
             << " | Threshold: " << current->threshold << "\n";
        current = current->next;
    }
}

// Function to record a sale and push it onto the Stack
void recordSale(ProductNode* head, TransactionNode*& topStack, int prodId, int qtySold) {
    // Find the product in the linked list
    ProductNode* current = head;
    while (current != nullptr && current->id != prodId) {
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Error: Product ID not found!\n";
        return;
    }

    if (current->quantity < qtySold) {
        cout << "Error: Not enough stock available!\n";
        return;
    }

    // Reduce the stock quantity
    current->quantity -= qtySold;

    // Push the sale onto the stack (LIFO)
    TransactionNode* newTx = new TransactionNode();
    newTx->productId = prodId;
    newTx->quantitySold = qtySold;
    newTx->next = topStack;
    topStack = newTx;

    cout << "Success: Sale recorded. Stock updated.\n";
}

// Function to undo the last sale using the Stack (Pop)
void undoLastSale(ProductNode* head, TransactionNode*& topStack) {
    if (topStack == nullptr) {
        cout << "\nNo sales to undo!\n";
        return;
    }

    // Get the top transaction from the stack
    TransactionNode* temp = topStack;
    topStack = topStack->next; // Move top pointer down

    // Find the product and give the stock back
    ProductNode* current = head;
    while (current != nullptr && current->id != temp->productId) {
        current = current->next;
    }

    if (current != nullptr) {
        current->quantity += temp->quantitySold;
        cout << "\nSuccess: Last sale undone. Restored " << temp->quantitySold << " units to " << current->name << ".\n";
    }

    delete temp; // Clean up memory
}

// Function to generate a low-stock report using Insertion Sort
void generateLowStockReport(ProductNode* head) {
    if (head == nullptr) {
        cout << "\nInventory is empty.\n";
        return;
    }

    // Step 1: Count how many items we have
    int count = 0;
    ProductNode* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }

    // Step 2: Create a simple array to copy our items into for sorting
    ProductNode* arr = new ProductNode[count];
    current = head;
    for (int i = 0; i < count; i++) {
        arr[i] = *current;
        current = current->next;
    }

    // Step 3: Insertion Sort (sorting by stock quantity, lowest first)
    for (int i = 1; i < count; i++) {
        ProductNode key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].quantity > key.quantity) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }

    // Step 4: Display the sorted report
    cout << "\n--- LOW-STOCK REPORT (Sorted) ---\n";
    for (int i = 0; i < count; i++) {
        cout << "ID: " << arr[i].id << " | Name: " << arr[i].name << " | Stock: " << arr[i].quantity;
        if (arr[i].quantity <= arr[i].threshold) {
            cout << " --> [LOW STOCK!]";
        }
        cout << "\n";
    }

    delete[] arr; // Clean up array memory
}

// ==========================================
// 3. MAIN PROGRAM LOOP
// ==========================================
int main() {
    ProductNode* inventoryHead = nullptr;
    TransactionNode* undoStackTop = nullptr;

    // Add some starter sample data
    addProduct(inventoryHead, 101, "Flour", 15.00, 25, 5);
    addProduct(inventoryHead, 102, "Sugar", 8.50, 4, 10);
    addProduct(inventoryHead, 103, "Cooking Oil", 22.00, 3, 8);

    int choice;
    do {
        cout << "\n=== SMALL BUSINESS INVENTORY MENU ===\n";
        cout << "1. View Inventory\n";
        cout << "2. Add Product\n";
        cout << "3. Record a Sale\n";
        cout << "4. Undo Last Sale\n";
        cout << "5. Low-Stock Report\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        // Guard against invalid menu input (like letters)
        if (cin.fail()) {
            cout << "Invalid choice! Please enter a number from 1 to 6.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) {
            displayInventory(inventoryHead);
        } 
        else if (choice == 2) {
            int id, qty, threshold;
            string name;
            double price;
            
            cout << "Enter ID: "; cin >> id;
            cout << "Enter Name: "; cin >> name;
            cout << "Enter Price ($): "; cin >> price;
            cout << "Enter Quantity: "; cin >> qty;
            cout << "Enter Threshold: "; cin >> threshold;
            
            // Check if the user typed something invalid (like letters instead of numbers)
            if (cin.fail()) {
                cout << "Invalid input! Please use numbers for ID, price, quantity, and threshold.\n";
                cin.clear();               // Resets the input error state
                cin.ignore(1000, '\n');    // Clears the bad typing out of the system buffer
            } else {
                addProduct(inventoryHead, id, name, price, qty, threshold);
            }
        }
        else if (choice == 3) {
            int id, qty;
            cout << "Enter Product ID sold: "; cin >> id;
            cout << "Enter quantity sold: "; cin >> qty;
            recordSale(inventoryHead, undoStackTop, id, qty);
        } 
        else if (choice == 4) {
            undoLastSale(inventoryHead, undoStackTop);
        } 
        else if (choice == 5) {
            generateLowStockReport(inventoryHead);
        }
    } while (choice != 6);

    return 0;
}