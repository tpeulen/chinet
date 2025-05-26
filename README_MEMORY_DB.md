# Using Chinet Without MongoDB

This document explains how to build and use Chinet without MongoDB, using the in-memory database implementation instead.

## Overview

Chinet now supports two database backends:
1. **MongoDB**: The original implementation using MongoDB for persistent storage
2. **In-Memory**(default): A new implementation that stores data in memory, without requiring MongoDB

The in-memory implementation provides the same interface as the MongoDB implementation, allowing existing code to work without modification. However, data is not persisted between program runs when using the in-memory implementation.

## Building Without MongoDB

To build Chinet with MongoDB, set the `WITH_MONGODB` CMake option to `ON`:

```bash
cmake -DWITH_MONGODB=ON ..
```

Alternatively, if MongoDB is not found on your system, the build will automatically fall back to the in-memory implementation.

## Implementation Details

The implementation uses a type alias `DatabaseObject` that points to either `MongoObject` (when MongoDB is enabled) or `MemoryObject` (when MongoDB is disabled). All classes that previously inherited from `MongoObject` now inherit from `DatabaseObject` instead.

The `MemoryObject` class provides the same interface as `MongoObject` but stores data in memory using a static `std::unordered_map` to simulate a database. This allows existing code to work without modification, but data is not persisted between program runs.

## Limitations

When using the in-memory implementation:

1. **Data Persistence**: Data is not persisted between program runs. All data is lost when the program exits.
2. **Distributed Access**: The in-memory implementation does not support distributed access to the database.
3. **Query Capabilities**: The in-memory implementation provides basic storage and retrieval functionality but does not support advanced query capabilities that MongoDB offers.

## Future Work

Future improvements to the in-memory implementation could include:

1. **File-based Persistence**: Adding the ability to save and load the in-memory database to/from a file.
2. **SQLite Backend**: Implementing a SQLite backend as an alternative to MongoDB for persistent storage.
3. **Enhanced Query Capabilities**: Adding more advanced query capabilities to the in-memory implementation.