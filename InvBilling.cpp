#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTS 100

typedef struct {
    int id;
    char name[30];
    double price;
    int quantity;
} Product;

typedef struct {
    Product products[MAX_PRODUCTS];
    int count;
} Inventory;

void addProduct(Inventory *inventory, int id, const char *name, double price, int quantity) {
    if (inventory->count < MAX_PRODUCTS) {
        inventory->products[inventory->count].id = id;
        strncpy(inventory->products[inventory->count].name, name, 30);
        inventory->products[inventory->count].price = price;
        inventory->products[inventory->count].quantity = quantity;
        inventory->count++;
    } else {
        printf("Inventory is full!\n");
    }
}

void displayInventory(const Inventory *inventory) {
    printf("\n%-10s %-20s %-10s %-10s\n", "ID", "Name", "Price", "Quantity");
    printf("--------------------------------------------------------\n");
    for (int i = 0; i < inventory->count; i++) {
        printf("%-10d %-20s %-10.2f %-10d\n", 
               inventory->products[i].id, 
               inventory->products[i].name, 
               inventory->products[i].price, 
               inventory->products[i].quantity);
    }
}

Product* findProductById(Inventory *inventory, int id) {
    for (int i = 0; i < inventory->count; i++) {
        if (inventory->products[i].id == id) {
            return &inventory->products[i];
        }
    }
    return NULL;
}

int reduceProductQuantity(Product *product, int quantity) {
    if (product->quantity >= quantity) {
        product->quantity -= quantity;
        return 1;
    }
    return 0;
}

void processPurchase(Inventory *inventory) {
    int id, quantity;
    double totalCost = 0.0;

    printf("Enter product ID and quantity to purchase (0 to finish):\n");

    while (1) {
        printf("Product ID: ");
        scanf("%d", &id);
        if (id == 0) break;

        printf("Quantity: ");
        scanf("%d", &quantity);

        Product *product = findProductById(inventory, id);
        if (product) {
            if (reduceProductQuantity(product, quantity)) {
                double cost = product->price * quantity;
                totalCost += cost;
                printf("%d x %s @ %.2f each = %.2f\n", quantity, product->name, product->price, cost);
            } else {
                printf("Insufficient stock for %s!\n", product->name);
            }
        } else {
            printf("Product not found!\n");
        }
    }

    printf("Total Cost: $%.2f\n", totalCost);
}

int main() {
    Inventory inventory = { .count = 0 };
    int choice;

    // Adding some sample products to inventory
    addProduct(&inventory, 1, "Apple", 0.5, 100);
    addProduct(&inventory, 2, "Banana", 0.3, 120);
    addProduct(&inventory, 3, "Orange", 0.8, 80);
    addProduct(&inventory, 4, "Milk", 1.2, 50);

    do {
        printf("\nStore Billing and Inventory System\n");
        printf("1. Display Inventory\n");
        printf("2. Process Purchase\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            displayInventory(&inventory);
            break;
        case 2:
            processPurchase(&inventory);
            break;
        case 3:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice! Try again.\n");
            break;
        }
    } while (choice != 3);

    return 0;
}
