#include <iostream>
#include <unordered_map>
#include <stack>
#include <stdexcept>

using namespace std;

class InMemory {
private:
    unordered_map<string, int> data;
    unordered_map<string, int> tempData; // Used to store transaction changes
    stack <unordered_map<string, int > > transactions; // Stack to manage transactions

    public:
    void begin_transaction() {
        transactions.push(tempData); // Save the current state to the stack
        tempData.clear(); // Clear the temporary data for the new transaction
    }

    void put(const string& key, int value) {
        if (transactions.empty()) {
            throw runtime_error("Transaction not in progress");
        }
        tempData[key] = value; // Store changes in temporary data
    }

    int get(const string& key) {
        if (data.find(key) != data.end()) {
            return data[key];
        }
        return 0; // Return 0 if key doesn't exist
    }

    void commit() {
        if (transactions.empty()) {
            throw runtime_error("No transaction to commit");
        }
        data.insert(tempData.begin(), tempData.end()); // Apply changes to main data
        tempData.clear(); // Clear temporary data
        transactions.pop(); // Remove the committed transaction from the stack
    }

    void rollback() {
        if (transactions.empty()) {
            throw runtime_error("No transaction to rollback");
        }
        tempData.clear(); // Clear temporary data
        tempData = transactions.top(); // Restore previous transaction state
        transactions.pop(); // Remove the rolled back transaction from the stack
    }

    int main() {
        InMemory db;
        db.begin_transaction();
        db.put("A", 100);
        db.put("B", 200);
        db.commit();

        db.begin_transaction();
        db.put("A", 50);
        db.rollback();

        cout << "A: " << db.get("A") << endl;
        cout << "B: " << db.get("B") << endl;

        return 0;
    }
};