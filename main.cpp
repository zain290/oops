#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
// Base abstract class
class User
{
protected:
    string username;

public:
    User(string uname) : username(uname) {}
    virtual void dashboard() = 0;
};
// Product class
class Product
{
private:
    int id;
    string name;
    double price;
    int quantity;

public:
    Product() {}
    Product(int id, string name, double price, int quantity)
        : id(id), name(name), price(price), quantity(quantity) {}

    // Friend function to display product (operator overloading)
    friend ostream &operator<<(ostream &out, const Product &p)
    {
        out << "ID: " << p.id << ", Name: " << p.name
            << ", Price: " << p.price << ", Quantity: " << p.quantity;
        return out;
    }
    // save product to CSV format
    string toCSV() const
    {
        return to_string(id) + "," + name + "," + to_string(price) + "," + to_string(quantity);
    }
    // get product ID
    int getId() const
    {
        return id;
    }
    // get product name
    string getName() const
    {
        return name;
    }
    // get product price
    double getPrice() const
    {
        return price;
    }
    // get product quantity
    int getQuantity() const
    {
        return quantity;
    }
    void setId(int id);
    void setName(string name);
    void setPrice(float price);
    void setQuantity(int qty);

    // update product details
    void updateDetails()
    {
        cout << "\nEnter new name: ";
        cin >> name;

        cout << "Enter new price: ";
        cin >> price;

        cout << "Enter new quantity: ";
        cin >> quantity;
    }

    // Save product to file
    void saveToFile()
    {
        ofstream file("products.txt", ios::app);
        if (file.is_open())
        {
            file << id << "," << name << "," << price << "," << quantity << "\n";
            file.close();
        }
    }
    // Load products from file
    static Product fromCSV(const string &line)
    {
        stringstream ss(line);
        string idStr, name, priceStr, quantityStr;
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, priceStr, ',');
        getline(ss, quantityStr, ',');

        int id = stoi(idStr);
        double price = stod(priceStr);
        int quantity = stoi(quantityStr);

        return Product(id, name, price, quantity);
    }
};
void Product::setQuantity(int qty)
{
    quantity = qty;
}
void Product::setId(int id)
{
    id = id;
}
void Product::setName(string name)
{
    name = name;
}
void Product::setPrice(float price)
{
    price = price;
}
// transaction class

// Admin class
class Admin : public User
{
public:
    Admin(string uname) : User(uname) {}
    bool isIdUnique(int newId)
    {
        ifstream file("products.txt");
        string line;

        while (getline(file, line))
        {
            Product p = Product::fromCSV(line);
            if (p.getId() == newId)
            {
                return false;
            }
        }

        return true;
    }
    // Add new product
    void addProduct()
    {
        cout << "\n🆕 === Add New Product ===\n";

        int id;
        string name;
        float price;
        int quantity;

        // Loop to ensure unique ID
        while (true)
        {
            cout << "Enter Product ID: ";
            cin >> id;

            if (isIdUnique(id))
                break; // If unique, continue
            else
                cout << "❌ ID already exists. Please enter a unique Product ID.\n";
        }

        cout << "Enter Product Name: ";
        cin.ignore(); // flush newline
        getline(cin, name);

        cout << "Enter Product Price: ";
        cin >> price;

        cout << "Enter Product Quantity: ";
        cin >> quantity;

        Product p(id, name, price, quantity);

        ofstream outFile("products.txt", ios::app);
        if (outFile)
        {
            outFile << p.toCSV() << "\n";
            cout << "✅ Product added successfully!\n";
        }
        else
        {
            cout << "❌ Error writing to file.\n";
        }

        outFile.close();
    }
    // Display all products

    void displayAllProducts()
    {
        cout << "\n📦 === Product Inventory ===\n";

        ifstream file("products.txt");
        string line;
        bool found = false;

        while (getline(file, line))
        {
            Product p = Product::fromCSV(line);
            cout << p << endl; // uses the overloaded << operator
            found = true;
        }

        file.close();

        if (!found)
        {
            cout << "⚠️ No products found in inventory.\n";
        }
    }
    // update product details
    void updateProduct()
    {
        int searchID;
        cout << "\n🔧 === Update Product Details ===\n";
        cout << "Enter Product ID to update: ";
        cin >> searchID;

        ifstream file("products.txt");
        ofstream tempFile("temp.txt");

        string line;
        bool found = false;

        while (getline(file, line))
        {
            Product p = Product::fromCSV(line);
            if (p.getId() == searchID)
            {
                found = true;
                cout << "Current Details:\n"
                     << p << endl;

                p.updateDetails();             // Update in memory
                tempFile << p.toCSV() << "\n"; // Write updated product
            }
            else
            {
                tempFile << line << "\n"; // Keep original
            }
        }

        file.close();
        tempFile.close();

        remove("products.txt");
        rename("temp.txt", "products.txt");

        if (found)
        {
            cout << "\n✅ Product updated and saved successfully!\n";
        }
        else
        {
            cout << "\n❌ Product ID not found.\n";
        }
    }
    // delete product

    void deleteProduct()
    {
        int deleteID;
        cout << "\n🗑️ === Delete Product ===\n";
        cout << "Enter Product ID to delete: ";
        cin >> deleteID;

        ifstream file("products.txt");
        ofstream tempFile("temp.txt");

        string line;
        bool found = false;

        while (getline(file, line))
        {
            Product p = Product::fromCSV(line);
            if (p.getId() == deleteID)
            {
                found = true;
                cout << "✅ Product with ID " << deleteID << " deleted.\n";
                // Skip writing this product (delete)
            }
            else
            {
                tempFile << line << "\n"; // Keep all other products
            }
        }

        file.close();
        tempFile.close();

        remove("products.txt");
        rename("temp.txt", "products.txt");

        if (!found)
        {
            cout << "❌ Product ID not found.\n";
        }
    }
    // search product
    void searchProduct()
    {
        int searchChoice;
        cout << "\n🔍 === Search Product ===\n";
        cout << "1. Search by ID\n";
        cout << "2. Search by Name\n";
        cout << "Choose an option: ";
        cin >> searchChoice;

        ifstream file("products.txt");
        if (!file)
        {
            cout << "❌ Failed to open products file.\n";
            return;
        }

        string line;
        bool found = false;

        if (searchChoice == 1)
        {
            int id;
            cout << "Enter Product ID: ";
            cin >> id;

            while (getline(file, line))
            {
                Product p = Product::fromCSV(line);
                if (p.getId() == id)
                {
                    cout << "\n✅ Product Found:\n"
                         << p << endl;
                    found = true;
                    break;
                }
            }
        }
        else if (searchChoice == 2)
        {
            string name;
            cout << "Enter Product Name: ";
            cin.ignore(); // flush newline
            getline(cin, name);

            while (getline(file, line))
            {
                Product p = Product::fromCSV(line);
                if (p.getName() == name)
                {
                    cout << "\n✅ Product Found:\n"
                         << p << endl;
                    found = true;
                    break;
                }
            }
        }
        else
        {
            cout << "❌ Invalid choice.\n";
        }

        if (!found)
        {
            cout << "❌ No product matched your search.\n";
        }

        file.close();
    }

    void dashboard() override
    {
        while (true)
        {
            int choice;
            cout << "\n========== Admin Dashboard ==========\n";
            cout << "1. Add New Products\n";
            cout << "2. Update Product Details\n";
            cout << "3. Delete product\n";
            cout << "4. Search Product by Name or ID\n";
            cout << "5. Display All Products\n";

            cout << "6. Exit to Main Menu\n";
            cout << "Choose an option: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                addProduct();
                break;
            case 2:
                updateProduct();
                break;
            case 3:
                deleteProduct();
                break;
            case 4:
                searchProduct();
                break;
            case 5:
                displayAllProducts();
                break;
            case 6:
                cout << "Exiting Admin Dashboard...\n";
                cout << "Returning to Main Menu...\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
    }
};

// Customer class
class Customer : public User
{
public:
    Customer(string uname) : User(uname) {}
    void displayAllProducts()
    {
        cout << "\n📦 === Product Inventory ===\n";

        ifstream file("products.txt");
        string line;
        bool found = false;

        while (getline(file, line))
        {
            Product p = Product::fromCSV(line);
            cout << p << endl; // uses the overloaded << operator
            found = true;
        }

        file.close();

        if (!found)
        {
            cout << "⚠️ No products found in inventory.\n";
        }
    }
    // Add products to cart
    void displayCart()
    {
        ifstream cart("cart.txt");
        string line;

        cout << "\n🛒 Your Cart:\n";
        cout << "ID\tName\t\tPrice\tQty\n";
        cout << "----------------------------------\n";

        while (getline(cart, line))
        {
            Product p = Product::fromCSV(line);
            cout << p.getId() << "\t" << p.getName() << "\t\t" << p.getPrice() << "\t" << p.getQuantity() << "\n";
        }

        cart.close();
    }
    // cart functionality
    void addToCart()
    {
        int searchID;
        cout << "\nEnter Product ID to add to cart: ";
        cin >> searchID;

        ifstream productFile("products.txt");
        ofstream cartFile("cart.txt", ios::app); // append mode
        string line;
        bool found = false;

        while (getline(productFile, line))
        {
            Product p = Product::fromCSV(line);
            if (p.getId() == searchID)
            {
                found = true;
                cout << "\nProduct found: " << p.getName()
                     << " (Available: " << p.getQuantity() << ")\n";

                int desiredQty = 0;
                // 🔁 Keep asking until valid quantity
                do
                {
                    cout << "Enter quantity to add: ";
                    cin >> desiredQty;

                    if (desiredQty <= 0 || desiredQty > p.getQuantity())
                    {
                        cout << "❌ Invalid quantity. Please enter a quantity between 1 and "
                             << p.getQuantity() << ".\n";
                    }
                } while (desiredQty <= 0 || desiredQty > p.getQuantity());

                // ✅ Valid quantity — Add to cart
                cartFile << p.getId() << ","
                         << p.getName() << ","
                         << p.getPrice() << ","
                         << desiredQty << "\n";
                cout << "✅ Product added to cart!\n";
                break;
            }
        }

        if (!found)
        {
            cout << "❌ Product with ID " << searchID << " not found.\n";
        }

        productFile.close();
        cartFile.close();
        displayCart(); // Show updated cart
    }

    // search product
    void searchProduct()
    {
        int searchChoice;
        cout << "\n🔍 === Search Product ===\n";
        cout << "1. Search by ID\n";
        cout << "2. Search by Name\n";
        cout << "Choose an option: ";
        cin >> searchChoice;

        ifstream file("products.txt");
        if (!file)
        {
            cout << "❌ Failed to open products file.\n";
            return;
        }

        string line;
        bool found = false;

        if (searchChoice == 1)
        {
            int id;
            cout << "Enter Product ID: ";
            cin >> id;

            while (getline(file, line))
            {
                Product p = Product::fromCSV(line);
                if (p.getId() == id)
                {
                    cout << "\n✅ Product Found:\n"
                         << p << endl;
                    found = true;
                    break;
                }
            }
        }
        else if (searchChoice == 2)
        {
            string name;
            cout << "Enter Product Name: ";
            cin.ignore(); // flush newline
            getline(cin, name);

            while (getline(file, line))
            {
                Product p = Product::fromCSV(line);
                if (p.getName() == name)
                {
                    cout << "\n✅ Product Found:\n"
                         << p << endl;
                    found = true;
                    break;
                }
            }
        }
        else
        {
            cout << "❌ Invalid choice.\n";
        }

        if (!found)
        {
            cout << "❌ No product matched your search.\n";
        }

        file.close();
    }
    // remove item from cart
    void removeFromCart()
    {
        int removeID;
        cout << "\nEnter Product ID to remove from cart: ";
        cin >> removeID;

        ifstream cartFile("cart.txt");
        ofstream tempFile("temp_cart.txt");
        bool found = false;

        string line;
        while (getline(cartFile, line))
        {
            Product p = Product::fromCSV(line);
            if (p.getId() == removeID)
            {
                found = true;
                cout << "🗑️ Product with ID " << removeID << " removed from cart.\n";
                continue; // skip writing this product
            }
            tempFile << line << "\n";
        }

        cartFile.close();
        tempFile.close();

        // Replace old cart file with updated one
        remove("cart.txt");
        rename("temp_cart.txt", "cart.txt");

        if (!found)
        {
            cout << "❌ Product with ID " << removeID << " not found in cart.\n";
        }
        displayCart(); // Show updated cart
    }
    // calculate bill
    void printBill()
    {
        const int MAX_CART = 100;
        int ids[MAX_CART], quantities[MAX_CART];
        string names[MAX_CART];
        double prices[MAX_CART];
        int count = 0;
        double subtotal = 0.0;
        const double taxRate = 0.15;

        ifstream cartFile("cart.txt");
        if (!cartFile)
        {
            cout << "🛒 Cart is empty or missing.\n";
            return;
        }

        string line;
        while (getline(cartFile, line) && count < MAX_CART)
        {
            stringstream ss(line);
            string idStr, name, priceStr, qtyStr;

            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, priceStr, ',');
            getline(ss, qtyStr, ',');

            ids[count] = stoi(idStr);
            names[count] = name;
            prices[count] = stod(priceStr);
            quantities[count] = stoi(qtyStr);

            subtotal += prices[count] * quantities[count];
            count++;
        }
        cartFile.close();

        if (count == 0)
        {
            cout << "🛒 Your cart is empty!\n";
            return;
        }

        // Show cart
        cout << "\n🧾 Your Cart:\n";
        cout << "----------------------------------------------\n";
        cout << "ID\tName\tPrice\tQty\tTotal\n";
        cout << "----------------------------------------------\n";
        for (int i = 0; i < count; ++i)
        {
            cout << ids[i] << "\t" << names[i] << "\t" << prices[i] << "\t" << quantities[i]
                 << "\t" << prices[i] * quantities[i] << " Rs\n";
        }
        cout << "----------------------------------------------\n";

        // Show totals
        double taxAmount = subtotal * taxRate;
        double total = subtotal + taxAmount;

        cout << "Subtotal       : " << subtotal << " Rs\n";
        cout << "Sales Tax (15%): " << taxAmount << " Rs\n";
        cout << "Total Amount   : " << total << " Rs\n";
    }

    // // payment method
    void confirmPayment()
    {
        cout << "\n📄 Your Bill:\n";
        printBill(); // Display cart and calculate total

        string choice;
        cout << "\nDo you want to proceed with payment? (yes/no): ";
        cin >> choice;

        if (choice != "yes")
        {
            cout << "❌ Payment cancelled.\n";
            return;
        }

        cout << "\nSelect Payment Method:\n";
        cout << "1. Credit Card\n";
        cout << "2. Cash on Delivery\n";
        int method;
        cin >> method;

        // First check all items in cart against current stock
        ifstream cartFile("cart.txt");
        ifstream productFile("products.txt");
        bool stockIssue = false;
        vector<Product> products;
        vector<Product> cartItems;

        // Read all products
        string productLine;
        while (getline(productFile, productLine))
        {
            products.push_back(Product::fromCSV(productLine));
        }
        productFile.close();

        // Read all cart items
        string cartLine;
        while (getline(cartFile, cartLine))
        {
            cartItems.push_back(Product::fromCSV(cartLine));
        }
        cartFile.close();

        // Check stock availability
        for (auto &cartItem : cartItems)
        {
            for (auto &product : products)
            {
                if (product.getId() == cartItem.getId())
                {
                    if (cartItem.getQuantity() > product.getQuantity())
                    {
                        cout << "❌ Not enough stock for: " << product.getName()
                             << " (Available: " << product.getQuantity()
                             << ", Requested: " << cartItem.getQuantity() << ")\n";
                        stockIssue = true;
                    }
                    break;
                }
            }
        }

        if (stockIssue)
        {
            cout << "\n❌ Payment failed due to stock issues.\n";
            return;
        }

        // Update quantities in products
        for (auto &cartItem : cartItems)
        {
            for (auto &product : products)
            {
                if (product.getId() == cartItem.getId())
                {
                    product.setQuantity(product.getQuantity() - cartItem.getQuantity());
                    break;
                }
            }
        }

        // Write updated products back to file
        ofstream outFile("products.txt");
        for (auto &product : products)
        {
            outFile << product.toCSV() << "\n";
        }
        outFile.close();

        // Clear the cart
        ofstream clearCart("cart.txt", ios::trunc);
        clearCart.close();

        cout << "\n✅ Payment Successful! Product quantities updated.\n";
    }

    // dashboard function
    void dashboard() override
    {
        while (true)
        {
            int choice;
            cout << "\n========== Customer Dashboard ==========\n";
            cout << "1. Display Available Products\n";
            cout << "2. Add Products to Cart\n";
            cout << "3. Search Product by Name or ID\n";
            cout << "4. Remove Item from Cart\n";
            cout << "5. Calculate Bill (with Tax%)\n";
            cout << "6. Choose Payment Method & Confirm\n";
            cout << "7. Rate the Product\n";
            cout << "8. display Cart\n";
            cout << "9. Exit to Main Menu\n";
            cout << "Choose an option: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                displayAllProducts();
                break;
            case 2:
                addToCart();
                break;
            case 3:
                searchProduct();
                break;
            case 4:
                removeFromCart();
                break;
            case 5:
                printBill();
                break;
            case 6:
                confirmPayment();
                break;
            case 7:
                cout << "[Customer] Rate Product - (Functionality coming soon)\n";
                break;
            case 8:
                displayCart();
                break;
            case 9:
                cout << "Returning to Main Menu...\n";
                return;
            default:
                cout << "Invalid option. Try again.\n";
            }
        }
    }
};
// Authentication system
class AuthSystem
{
public:
    static bool signup()
    {
        string username, password, role;

        cout << "\n=== Signup ===\n";
        cout << "Enter username: ";
        cin >> username;

        // Check for unique username
        ifstream infile("users.txt");
        string line, fileUser;
        bool exists = false;

        while (getline(infile, line))
        {
            stringstream ss(line);
            getline(ss, fileUser, ',');
            if (fileUser == username)
            {
                exists = true;
                break;
            }
        }
        infile.close();

        if (exists)
        {
            cout << "\n🔴 Username already taken. Please choose a different one.\n";
            return false;
        }

        cout << "Enter password: ";
        cin >> password;

        // Validate role input
        while (true)
        {
            cout << "Enter role (Admin/Customer): ";
            cin >> role;

            // Convert to lowercase for comparison
            string roleLower = role;
            for (char &c : roleLower)
                c = tolower(c);

            if (roleLower == "admin" || roleLower == "customer")
            {
                break; // valid input
            }
            else
            {
                cout << "\n🔴 Invalid role. Please enter 'Admin' or 'Customer'.\n";
            }
        }

        ofstream file("users.txt", ios::app);
        if (file.is_open())
        {
            file << username << "," << password << "," << role << "\n";
            file.close();
            cout << "\n✅ Account created successfully!\n";
            return true;
        }
        else
        {
            cout << "Error opening file.\n";
            return false;
        }
    }

    static bool login()
    {
        string username, password, line, fileUser, filePass, fileRole;
        cout << "\n=== Login ===\n";
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        ifstream file("users.txt");
        if (file.is_open())
        {
            while (getline(file, line))
            {
                stringstream ss(line);
                getline(ss, fileUser, ',');
                getline(ss, filePass, ',');
                getline(ss, fileRole, ',');

                if (fileUser == username && filePass == password)
                {
                    cout << "\nLogin successful as " << fileRole << "!\n";
                    file.close();

                    if (fileRole == "Admin" || fileRole == "admin")
                    {
                        Admin admin(username);
                        admin.dashboard();
                    }
                    else if (fileRole == "Customer" || fileRole == "customer")
                    {
                        Customer customer(username);
                        customer.dashboard();
                    }
                    return true;
                }
            }
            file.close();
            cout << "Invalid credentials.\n";
        }
        else
        {
            cout << "Error opening file.\n";
        }
        return false;
    }
};

// Main program loop
int main()
{
    int choice;

    while (true)
    {
        cout << "\n========= Grocery Management System =========\n";
        cout << "1. Signup\n";
        cout << "2. Login\n";
        cout << "3. Exit Program\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (AuthSystem::signup())
            {
                AuthSystem::login(); // Auto login after signup
            }
            break;
        case 2:
            AuthSystem::login();
            break;
        case 3:
            cout << "Exiting program. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid option. Try again.\n";
        }
    }
}
// End of code