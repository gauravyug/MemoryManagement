#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
using namespace std;

class BuddyAllocator {
    int totalSize;               // Total memory size
    int minBlockSize;            // Minimum block size
    int levels;                  // Levels in the tree (log2(totalSize/minBlockSize))
    vector<int> bitmap;          // Bitmap representing tree (0: free, 1: used/split)
    unordered_map<void*, int> allocations; // Address -> level

public:
    BuddyAllocator(int total, int minBlock = 1) {
        totalSize = total;
        minBlockSize = minBlock;
        levels = log2(totalSize / minBlockSize) + 1;
        bitmap.resize((1 << levels) - 1, 0); // Full binary tree
    }

    // Allocate memory of size `size`
    void* allocate(int size) {
        int requiredLevel = getRequiredLevel(size);
        int index = allocateHelper(0, 0, requiredLevel);
        if (index == -1) return nullptr;

        void* ptr = indexToAddress(index, requiredLevel);
        allocations[ptr] = requiredLevel;
        return ptr;
    }

    // Deallocate memory from address
    void deallocate(void* addr) {
        if (allocations.find(addr) == allocations.end()) return;

        int level = allocations[addr];
        int index = addressToIndex(addr, level);
        deallocateHelper(index, level);
        allocations.erase(addr);
    }

private:
    // Get the level required for allocation
    int getRequiredLevel(int size) {
        int blockSize = totalSize;
        for (int level = 0; level < levels; ++level) {
            if (size <= blockSize) return level;
            blockSize /= 2;
        }
        return -1;
    }

    // Try to allocate a block at or below current level
    int allocateHelper(int index, int level, int targetLevel) {
        if (level >= levels) return -1;

        if (bitmap[index] == 1) return -1; // Used/split

        if (level == targetLevel) {
            bitmap[index] = 1;
            return index;
        }

        // Split node
        bitmap[index] = 1;
        int left = leftChild(index);
        int result = allocateHelper(left, level + 1, targetLevel);
        if (result != -1) return result;

        int right = rightChild(index);
        result = allocateHelper(right, level + 1, targetLevel);
        if (result != -1) return result;

        // Allocation failed, mark this node as free again
        bitmap[index] = 0;
        return -1;
    }

    // Deallocate helper: free and merge buddies if possible
    void deallocateHelper(int index, int level) {
        bitmap[index] = 0;

        while (index != 0) {
            int parent = parentIndex(index);
            int sibling = siblingIndex(index);

            if (bitmap[sibling] != 0) break;

            bitmap[parent] = 0;
            index = parent;
        }
    }

    // Get the memory address from bitmap index
    void* indexToAddress(int index, int level) {
        int blockSize = totalSize >> level;
        int offset = (index - ((1 << level) - 1)) * blockSize;
        return reinterpret_cast<void*>(offset);
    }

    // Get bitmap index from memory address
    int addressToIndex(void* addr, int level) {
        int blockSize = totalSize >> level;
        int offset = reinterpret_cast<uintptr_t>(addr);
        int position = offset / blockSize;
        return (1 << level) - 1 + position;
    }

    // Helpers for binary tree indexing
    int leftChild(int index) { return 2 * index + 1; }
    int rightChild(int index) { return 2 * index + 2; }
    int parentIndex(int index) { return (index - 1) / 2; }
    int siblingIndex(int index) {
        if (index % 2 == 0) return index - 1;
        return index + 1;
    }

    // Debug: Print bitmap state
public:
    void printBitmap() {
        for (int i = 0; i < bitmap.size(); ++i) {
            cout << bitmap[i] << " ";
        }
        cout << endl;
    }
};
