#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PRODUCTS 1000
#define MAX_NAME_LENGTH 50
#define MAX_CUSTOMERS 100
#define MAX_BILLS 1000

// Product structure
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    int stock;
} Product;

// Customer structure
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char phone[15];
} Customer;

// Bill item structure
typedef struct {
    int product_id;
    int quantity;
    float total_price;
} BillItem;

// Bill structure
typedef struct {
    int id;
    int customer_id;
    time_t date;
    BillItem items[MAX_PRODUCTS];
    int item_count;
    float total_amount;
} Bill;

// Global variables
Product products[MAX_PRODUCTS];
int product_count = 0;
Customer customers[MAX_CUSTOMERS];
int customer_count = 0;
Bill bills[MAX_BILLS];
int bill_count = 0;

// Function prototypes
void load_data();
void save_data();
void add_product();
void update_product();
void delete_product();
void list_products();
void add_customer();
void update_customer();
void delete_customer();
void list_customers();
void create_bill();
void view_bill();
void generate_report();
int find_product(int id);
int find_customer(int id);
void print_menu();
void print_bill(Bill *bill);

int main() {
    load_data();
    int choice;
    
    do {
        print_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                add_product();
                break;
            case 2:
                update_product();
                break;
            case 3:
                delete_product();
                break;
            case 4:
                list_products();
                break;
            case 5:
                add_customer();
                break;
            case 6:
                update_customer();
                break;
            case 7:
                delete_customer();
                break;
            case 8:
                list_customers();
                break;
            case 9:
                create_bill();
                break;
            case 10:
                view_bill();
                break;
            case 11:
                generate_report();
                break;
            case 0:
                save_data();
                printf("Thank you for using the Supermarket Billing System. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
    
    return 0;
}

void load_data() {
    FILE *product_file = fopen("products.dat", "rb");
    FILE *customer_file = fopen("customers.dat", "rb");
    FILE *bill_file = fopen("bills.dat", "rb");
    
    if (product_file) {
        fread(&product_count, sizeof(int), 1, product_file);
        fread(products, sizeof(Product), product_count, product_file);
        fclose(product_file);
    }
    
    if (customer_file) {
        fread(&customer_count, sizeof(int), 1, customer_file);
        fread(customers, sizeof(Customer), customer_count, customer_file);
        fclose(customer_file);
    }
    
    if (bill_file) {
        fread(&bill_count, sizeof(int), 1, bill_file);
        fread(bills, sizeof(Bill), bill_count, bill_file);
        fclose(bill_file);
    }
}

void save_data() {
    FILE *product_file = fopen("products.dat", "wb");
    FILE *customer_file = fopen("customers.dat", "wb");
    FILE *bill_file = fopen("bills.dat", "wb");
    
    if (product_file) {
        fwrite(&product_count, sizeof(int), 1, product_file);
        fwrite(products, sizeof(Product), product_count, product_file);
        fclose(product_file);
    }
    
    if (customer_file) {
        fwrite(&customer_count, sizeof(int), 1, customer_file);
        fwrite(customers, sizeof(Customer), customer_count, customer_file);
        fclose(customer_file);
    }
    
    if (bill_file) {
        fwrite(&bill_count, sizeof(int), 1, bill_file);
        fwrite(bills, sizeof(Bill), bill_count, bill_file);
        fclose(bill_file);
    }
}

void add_product() {
    if (product_count >= MAX_PRODUCTS) {
        printf("Maximum number of products reached.\n");
        return;
    }
    
    Product new_product;
    printf("Enter product ID: ");
    scanf("%d", &new_product.id);
    
    if (find_product(new_product.id) != -1) {
        printf("Product with this ID already exists.\n");
        return;
    }
    
    printf("Enter product name: ");
    scanf(" %[^\n]", new_product.name);
    printf("Enter product price: ");
    scanf("%f", &new_product.price);
    printf("Enter product stock: ");
    scanf("%d", &new_product.stock);
    
    products[product_count++] = new_product;
    printf("Product added successfully.\n");
}

void update_product() {
    int id, index;
    printf("Enter product ID to update: ");
    scanf("%d", &id);
    
    index = find_product(id);
    if (index == -1) {
        printf("Product not found.\n");
        return;
    }
    
    printf("Enter new product name (or press enter to keep current): ");
    char name[MAX_NAME_LENGTH];
    getchar();
    fgets(name, MAX_NAME_LENGTH, stdin);
    if (name[0] != '\n') {
        name[strcspn(name, "\n")] = 0;
        strcpy(products[index].name, name);
    }
    
    printf("Enter new product price (or -1 to keep current): ");
    float price;
    scanf("%f", &price);
    if (price != -1) {
        products[index].price = price;
    }
    
    printf("Enter new product stock (or -1 to keep current): ");
    int stock;
    scanf("%d", &stock);
    if (stock != -1) {
        products[index].stock = stock;
    }
    
    printf("Product updated successfully.\n");
}

void delete_product() {
    int id, index;
    printf("Enter product ID to delete: ");
    scanf("%d", &id);
    
    index = find_product(id);
    if (index == -1) {
        printf("Product not found.\n");
        return;
    }
    
    for (int i = index; i < product_count - 1; i++) {
        products[i] = products[i + 1];
    }
    product_count--;
    
    printf("Product deleted successfully.\n");
}

void list_products() {
    printf("Product List:\n");
    printf("ID\tName\t\t\tPrice\tStock\n");
    for (int i = 0; i < product_count; i++) {
        printf("%d\t%-20s\t%.2f\t%d\n", products[i].id, products[i].name, products[i].price, products[i].stock);
    }
}

void add_customer() {
    if (customer_count >= MAX_CUSTOMERS) {
        printf("Maximum number of customers reached.\n");
        return;
    }
    
    Customer new_customer;
    printf("Enter customer ID: ");
    scanf("%d", &new_customer.id);
    
    if (find_customer(new_customer.id) != -1) {
        printf("Customer with this ID already exists.\n");
        return;
    }
    
    printf("Enter customer name: ");
    scanf(" %[^\n]", new_customer.name);
    printf("Enter customer phone: ");
    scanf("%s", new_customer.phone);
    
    customers[customer_count++] = new_customer;
    printf("Customer added successfully.\n");
}

void update_customer() {
    int id, index;
    printf("Enter customer ID to update: ");
    scanf("%d", &id);
    
    index = find_customer(id);
    if (index == -1) {
        printf("Customer not found.\n");
        return;
    }
    
    printf("Enter new customer name (or press enter to keep current): ");
    char name[MAX_NAME_LENGTH];
    getchar();
    fgets(name, MAX_NAME_LENGTH, stdin);
    if (name[0] != '\n') {
        name[strcspn(name, "\n")] = 0;
        strcpy(customers[index].name, name);
    }
    
    printf("Enter new customer phone (or press enter to keep current): ");
    char phone[15];
    fgets(phone, 15, stdin);
    if (phone[0] != '\n') {
        phone[strcspn(phone, "\n")] = 0;
        strcpy(customers[index].phone, phone);
    }
    
    printf("Customer updated successfully.\n");
}

void delete_customer() {
    int id, index;
    printf("Enter customer ID to delete: ");
    scanf("%d", &id);
    
    index = find_customer(id);
    if (index == -1) {
        printf("Customer not found.\n");
        return;
    }
    
    for (int i = index; i < customer_count - 1; i++) {
        customers[i] = customers[i + 1];
    }
    customer_count--;
    
    printf("Customer deleted successfully.\n");
}

void list_customers() {
    printf("Customer List:\n");
    printf("ID\tName\t\t\tPhone\n");
    for (int i = 0; i < customer_count; i++) {
        printf("%d\t%-20s\t%s\n", customers[i].id, customers[i].name, customers[i].phone);
    }
}

void create_bill() {
    if (bill_count >= MAX_BILLS) {
        printf("Maximum number of bills reached.\n");
        return;
    }
    
    Bill new_bill;
    new_bill.id = bill_count + 1;
    new_bill.date = time(NULL);
    new_bill.item_count = 0;
    new_bill.total_amount = 0;
    
    printf("Enter customer ID: ");
    scanf("%d", &new_bill.customer_id);
    
    if (find_customer(new_bill.customer_id) == -1) {
        printf("Customer not found. Please add the customer first.\n");
        return;
    }
    
    while (1) {
        int product_id, quantity;
        printf("Enter product ID (or 0 to finish): ");
        scanf("%d", &product_id);
        
        if (product_id == 0) {
            break;
        }
        
        int product_index = find_product(product_id);
        if (product_index == -1) {
            printf("Product not found.\n");
            continue;
        }
        
        printf("Enter quantity: ");
        scanf("%d", &quantity);
        
        if (quantity > products[product_index].stock) {
            printf("Insufficient stock.\n");
            continue;
        }
        
        new_bill.items[new_bill.item_count].product_id = product_id;
        new_bill.items[new_bill.item_count].quantity = quantity;
        new_bill.items[new_bill.item_count].total_price = quantity * products[product_index].price;
        new_bill.total_amount += new_bill.items[new_bill.item_count].total_price;
        new_bill.item_count++;
        
        products[product_index].stock -= quantity;
    }
    
    bills[bill_count++] = new_bill;
    printf("Bill created successfully.\n");
    print_bill(&new_bill);
}

void view_bill() {
    int bill_id;
    printf("Enter bill ID: ");
    scanf("%d", &bill_id);
    
    for (int i = 0; i < bill_count; i++) {
        if (bills[i].id == bill_id) {
            print_bill(&bills[i]);
            return;
        }
    }
    
    printf("Bill not found.\n");
}

void generate_report() {
    float total_sales = 0;
    int total_items_sold = 0;
    
    printf("Sales Report:\n");
    printf("Bill ID\tCustomer ID\tDate\t\t\tTotal Amount\n");
    
    for (int i = 0; i < bill_count; i++) {
        char date_str[26];
        ctime_r(&bills[i].date, date_str);
        date_str[24] = '\0';  // Remove newline character
        
        printf("%d\t%d\t\t%s\t%.2f\n", bills[i].id, bills[i].customer_id, date_str, bills[i].total_amount);
        
        total_sales += bills[i].total_amount;
        for (int j = 0; j < bills[i].item_count; j++) {
            total_items_sold += bills[i].items[j].quantity;
        }
    }
    
    printf("\nTotal Sales: %.2f\n", total_sales);
    printf("Total Items Sold: %d\n", total_items_sold);
    printf("Average Sale per Bill: %.2f\n", total_sales / bill_count);
}

int find_product(int id) {
    for (int i = 0; i < product_count; i++) {
        if (products[i].id == id) {
            return i;
        }
    }
    return -1;
}

int find_customer(int id) {
    for (int i = 0; i < customer_count; i++) {
        if (customers[i].id == id) {
            return i;
        }
    }
    return -1;
}

void print_menu() {
    printf("\nSupermarket Billing System\n");
    printf("1. Add Product\n");
    printf("2. Update Product\n");
    printf("3. Delete Product\n");
    printf("4. List Products\n");
    printf("5. Add Customer\n");
    printf("6. Update Customer\n");
    printf("7. Delete Customer\n");
    printf("8. List Customers\n");
    printf("9. Create Bill\n");
    printf("10. View Bill\n");
    printf("11. Generate Report\n");
    printf("0. Exit\n");
}

void print_bill(Bill *bill) {
    printf("\nBill ID: %d\n", bill->id);
    printf("Customer ID: %d\n", bill->customer_id);
    
    char date_str[26];
    ctime_r(&bill->date, date_str);
