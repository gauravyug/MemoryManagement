# MemoryManagement
Operating System Memory Management

### 🔍 What is a Buddy Bitmap?
The Buddy Memory Allocation system:

# Divides memory into partitions of sizes that are powers of two.

# When a request comes in, it allocates the smallest possible block (power of two) that can satisfy the request.

# If no exact-size block is available, it splits a larger block and marks the "buddies" in a bitmap.

# On deallocation, it merges free buddy blocks if both are free.

#### ✅ Goals of Buddy Bitmap Implementation
We'll implement:

Memory initialization with total size (e.g. 1024 bytes).

Allocation requests of any size.

Deallocation using the address.

Bitmap to track which blocks are free or used.

Splitting and merging logic.

#### 🧠 Key Concepts
## Terminology:
Block: A chunk of memory of size 2k
Buddy: Two blocks of the same size resulting from splitting a block of  2𝑘+1

 

## Bitmap Representation:
Tree-structured bitmap (like a segment tree):

Each node in the bitmap represents a memory block.

Root represents the entire memory.

Leaves represent the smallest unit block (e.g. 1 KB).

0 = free, 1 = occupied

📦 Example:
Memory Size = 16 bytes
Minimum Block = 1 byte
Levels = 4 (2⁴ = 16)