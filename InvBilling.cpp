#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

typedef struct Purchase {
    int id;
    int quantity;
    double cost;
    struct Purchase *next;
} Purchase;

typedef struct {
    Purchase *front;
    Purchase *rear;
} PurchaseQueue;

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
    printf("\n%-10s %-20s %-10s %-10s\n", "ID", "Name", "Price (₹)", "Quantity");
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

void initializeQueue(PurchaseQueue *queue) {
    queue->front = queue->rear = NULL;
}

void enqueue(PurchaseQueue *queue, int id, int quantity, double cost) {
    Purchase *newPurchase = (Purchase*)malloc(sizeof(Purchase));
    if (!newPurchase) {
        printf("Memory allocation failed\n");
        return;
    }
    newPurchase->id = id;
    newPurchase->quantity = quantity;
    newPurchase->cost = cost;
    newPurchase->next = NULL;

    if (queue->rear) {
        queue->rear->next = newPurchase;
    } else {
        queue->front = newPurchase;
    }
    queue->rear = newPurchase;
}

void dequeue(PurchaseQueue *queue) {
    if (!queue->front) {
        printf("Queue is empty\n");
        return;
    }
    Purchase *temp = queue->front;
    queue->front = queue->front->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(temp);
}

void displayQueue(PurchaseQueue *queue) {
    Purchase *current = queue->front;
    printf("\nRecent Purchases (FIFO Queue):\n");
    printf("%-10s %-10s %-10s\n", "Product ID", "Quantity", "Cost (₹)");
    printf("-----------------------------------\n");
    while (current) {
        printf("%-10d %-10d %-10.2f\n", current->id, current->quantity, current->cost);
        current = current->next;
    }
}

void processPurchase(Inventory *inventory, PurchaseQueue *queue) {
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
                printf("%d x %s @ ₹%.2f each = ₹%.2f\n", quantity, product->name, product->price, cost);
                enqueue(queue, id, quantity, cost);
            } else {
                printf("Insufficient stock for %s!\n", product->name);
            }
        } else {
            printf("Product not found!\n");
        }
    }

    printf("Total Cost: ₹%.2f\n", totalCost);
}

void restockProduct(Inventory *inventory) {
    int id, quantity;
    printf("Enter product ID to restock: ");
    scanf("%d", &id);

    Product *product = findProductById(inventory, id);
    if (product) {
        printf("Enter quantity to add: ");
        scanf("%d", &quantity);
        product->quantity += quantity;
        printf("%s restocked successfully. New quantity: %d\n", product->name, product->quantity);
    } else {
        printf("Product not found!\n");
    }
}

int main() {
    Inventory inventory = { .count = 0 };
    PurchaseQueue queue;
    initializeQueue(&queue);
    int choice;

    addProduct(&inventory, 1, "Apple", 50.0, 100);
    addProduct(&inventory, 2, "Banana", 30.0, 120);
    addProduct(&inventory, 3, "Orange", 80.0, 80);
    addProduct(&inventory, 4, "Milk", 120.0, 50);

    do {
        printf("\nStore Billing and Inventory System\n");
        printf("1. Display Inventory\n");
        printf("2. Process Purchase\n");
        printf("3. Purchase History\n");
        printf("4. Restock Product\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            displayInventory(&inventory);
            break;
        case 2:
            processPurchase(&inventory, &queue);
            break;
        case 3:
            displayQueue(&queue);
            break;
        case 4:
            restockProduct(&inventory);
            break;
        case 5:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice! Try again.\n");
            break;
        }
    } while (choice != 5);

    return 0;
}
