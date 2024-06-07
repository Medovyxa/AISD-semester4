#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <set>

size_t lcg() {
    static size_t x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}

class BinarySearchTree {
public:
    struct Node {
        int key;
        Node* left;
        Node* right;
        Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

private:
    Node* root;

public:
    BinarySearchTree() : root(nullptr) {}

    BinarySearchTree(const BinarySearchTree& other) {
        root = copyHelper(other.root);
    }

    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (this != &other) {
            clear();
            root = copyHelper(other.root);
        }
        return *this;
    }

    ~BinarySearchTree() {
        clear();
    }

    Node* getRoot() const {
        return root;
    }

    Node* end() const {
        static Node endNode(INT_MAX);
        return &endNode;
    }

    void insert(int key) {
        insertHelper(root, key);
    }

    bool contains(int key) const {
        return containsHelper(root, key);
    }

    bool erase(int key) {
        return eraseHelper(root, key);
    }

    void print() const {
        printHelper(root);
        std::cout << std::endl;
    }

    void clear() {
        deleteHelper(root);
        root = nullptr;
    }

    std::set<int> toSet() const {
        std::set<int> result;
        toSetHelper(root, result);
        return result;
    }

private:
    Node* copyHelper(Node* root) {
        if (!root)
            return nullptr;
        Node* newNode = new Node(root->key);
        newNode->left = copyHelper(root->left);
        newNode->right = copyHelper(root->right);
        return newNode;
    }

    void deleteHelper(Node* root) {
        if (!root)
            return;
        deleteHelper(root->left);
        deleteHelper(root->right);
        delete root;
    }

    void printHelper(Node* root) const {
        if (!root)
            return;
        printHelper(root->left);
        std::cout << root->key << " ";
        printHelper(root->right);
    }

    bool insertHelper(Node*& root, int key) {
        if (!root) {
            root = new Node(key);
            return true;
        }
        if (key < root->key)
            return insertHelper(root->left, key);
        else if (key > root->key)
            return insertHelper(root->right, key);
        return false;
    }

    bool containsHelper(Node* root, int key) const {
        if (!root)
            return false;
        if (root->key == key)
            return true;
        if (key < root->key)
            return containsHelper(root->left, key);
        else
            return containsHelper(root->right, key);
    }

    bool eraseHelper(Node*& root, int key) {
        if (!root)
            return false;
        if (key < root->key)
            return eraseHelper(root->left, key);
        else if (key > root->key)
            return eraseHelper(root->right, key);
        else {
            if (!root->left && !root->right) {
                delete root;
                root = nullptr;
            }
            else if (!root->left) {
                Node* temp = root;
                root = root->right;
                delete temp;
            }
            else if (!root->right) {
                Node* temp = root;
                root = root->left;
                delete temp;
            }
            else {
                Node* temp = findMin(root->right);
                root->key = temp->key;
                eraseHelper(root->right, temp->key);
            }
            return true;
        }
    }

    Node* findMin(Node* node) const {
        while (node->left)
            node = node->left;
        return node;
    }

    void toSetHelper(Node* root, std::set<int>& result) const {
        if (!root)
            return;
        toSetHelper(root->left, result);
        result.insert(root->key);
        toSetHelper(root->right, result);
    }
};

std::vector<int> generateRandomNumbers(int count, int min, int max) {
    std::vector<int> numbers;
    numbers.reserve(count);

    for (int i = 0; i < count; ++i)
        numbers.push_back(min + lcg() % (max - min + 1));

    return numbers;
}

double measureInsertionTime(BinarySearchTree& set, const std::vector<int>& numbers) {
    auto start = std::chrono::steady_clock::now();
    for (int num : numbers) {
        set.insert(num);
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

double measureSearchTime(const BinarySearchTree& set, const std::vector<int>& numbers) {
    auto start = std::chrono::steady_clock::now();
    for (int num : numbers) {
        set.contains(num);
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

double measureAdditionAndDeletionTime(BinarySearchTree& set, const std::vector<int>& numbers) {
    auto start = std::chrono::steady_clock::now();
    for (int num : numbers) {
        set.insert(num);
    }
    for (int num : numbers) {
        set.erase(num);
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

double measureVectorInsertionTime(std::vector<int>& vec, const std::vector<int>& numbers) {
    auto start = std::chrono::steady_clock::now();
    for (int num : numbers) {
        vec.push_back(num);
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

double measureVectorSearchTime(const std::vector<int>& vec, const std::vector<int>& numbers) {
    auto start = std::chrono::steady_clock::now();
    for (int num : numbers) {
        std::find(vec.begin(), vec.end(), num);
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

double measureVectorAdditionAndDeletionTime(std::vector<int>& vec, const std::vector<int>& numbers) {
    auto start = std::chrono::steady_clock::now();
    for (int num : numbers) {
        vec.push_back(num);
    }
    for (int num : numbers) {
        vec.erase(std::remove(vec.begin(), vec.end(), num), vec.end());
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

std::set<int> setUnion(const std::set<int>& set1, const std::set<int>& set2) {
    std::set<int> result;
    std::set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(result, result.begin()));
    return result;
}

std::set<int> setSymmetricDifference(const std::set<int>& set1, const std::set<int>& set2) {
    std::set<int> result;
    std::set_symmetric_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(result, result.begin()));
    return result;
}

int main() {
    const int trials = 100; // Количество попыток для каждого эксперимента

    // Среднее время заполнения BinarySearchTree уникальными случайными числами
    for (int n : {1000, 10000, 12500}) {
        double totalTime = 0.0;
        for (int i = 0; i < trials; ++i) {
            BinarySearchTree set;
            auto numbers = generateRandomNumbers(n, 1, n * 10);
            totalTime += measureInsertionTime(set, numbers);
        }
        std::cout << "Average time to fill BinarySearchTree with " << n << " elements: " << totalTime / trials << " seconds\n";
    }

    // Среднее время поиска случайных чисел в BinarySearchTree
    for (int n : {1000, 10000, 12500}) {
        double totalTime = 0.0;
        BinarySearchTree set;
        auto numbers = generateRandomNumbers(n, 1, n * 10);
        for (int i = 0; i < trials; ++i) {
            totalTime += measureSearchTime(set, numbers);
        }
        std::cout << "Average time to search for a random number in BinarySearchTree with " << n << " elements: " << totalTime / trials << " seconds\n";
    }

    // Среднее время добавления и удаления случайных чисел в BinarySearchTree
    for (int n : {1000, 10000, 12500}) {
        double totalTime = 0.0;
        for (int i = 0; i < trials; ++i) {
            BinarySearchTree set;
            auto numbers = generateRandomNumbers(n, 1, n * 10);
            totalTime += measureAdditionAndDeletionTime(set, numbers);
        }
        std::cout << "Average time to add and erase " << n << " random numbers from BinarySearchTree: " << totalTime / trials << " seconds\n";
    }

    // Среднее время заполнения std::vector<int> уникальными случайными числами
    for (int n : {1000, 10000, 12500}) {
        double totalTime = 0.0;
        for (int i = 0; i < trials; ++i) {
            std::vector<int> vec;
            auto numbers = generateRandomNumbers(n, 1, n * 10);
            totalTime += measureVectorInsertionTime(vec, numbers);
        }
        std::cout << "Average time to fill std::vector<int> with " << n << " elements: " << totalTime / trials << " seconds\n";
    }

    // Среднее время поиска случайных чисел в std::vector<int>
    for (int n : {1000, 10000, 12500}) {
        double totalTime = 0.0;
        std::vector<int> vec;
        auto numbers = generateRandomNumbers(n, 1, n * 10);
        for (int i = 0; i < trials; ++i) {
            totalTime += measureVectorSearchTime(vec, numbers);
        }
        std::cout << "Average time to search for a random number in std::vector<int> with " << n << " elements: " << totalTime / trials << " seconds\n";
    }

    // Среднее время добавления и удаления случайных чисел в std::vector<int>
    for (int n : {1000, 10000, 12500}) {
        double totalTime = 0.0;
        for (int i = 0; i < trials; ++i) {
            std::vector<int> vec;
            auto numbers = generateRandomNumbers(n, 1, n * 10);
            totalTime += measureVectorAdditionAndDeletionTime(vec, numbers);
        }
        std::cout << "Average time to add and erase " << n << " random numbers from std::vector<int>: " << totalTime / trials << " seconds\n";
    }

    // Выполнение функций объединения и симметрической разности двух множеств
    BinarySearchTree set1;
    set1.insert(1);
    set1.insert(2);
    set1.insert(3);
    set1.insert(4);
    set1.insert(5);

    BinarySearchTree set2;
    set2.insert(4);
    set2.insert(5);
    set2.insert(6);
    set2.insert(7);
    set2.insert(8);

    std::set<int> unionSet = setUnion(set1.toSet(), set2.toSet());
    std::cout << "Union of sets: ";
    for (int num : unionSet) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::set<int> symmetricDiffSet = setSymmetricDifference(set1.toSet(), set2.toSet());
    std::cout << "Symmetric difference of sets: ";
    for (int num : symmetricDiffSet) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
