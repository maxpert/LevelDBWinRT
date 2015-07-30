# LevelDB Windows Runtime Component

 This project aims to enable usage of LevelDB on Windows Phone 8+ and Windows 8+ platforms. This includes Windows 10 Universal Platform. LevelDB stores keys and values in arbitrary byte arrays, and data is sorted by key. It supports batching writes, forward and backward iteration, and compression of the data via Google's Snappy compression library.

LevelDB is a fast key-value storage library written at Google that provides an ordered mapping from string keys to string values.
LevelDB Authors: Sanjay Ghemawat (sanjay@google.com) and Jeff Dean (jeff@google.com)

# Features
  * Keys and values can be arbitrary byte arrays (Slice.FromArray)
  * Data is stored and sorted by key.
  * Callers can provide custom comparison function to override the sort order (Not implemented yet).
  * Basic Get(key)/Put(key, value)/Delete(key) support.
  * Atomic batch operations can make multiple changes.
  * Users can create transient snapshot to get a consistent view of data.
  * Iterator support, with forward and backward iteration.
  * Basic wrappers for WriteOptions, ReadOptions, Options.
  * Snappy, and LZ4 compression (Not implemented yet).
  * ARM or x86 architecture supported.

# TODO:

  * Use native Windows API instead of fstream to ensure better Flush and Sync.
  * Compression support.
  * Comparators support.

This is an unoffical port of LevelDB for Windows Runtime (forked and modified from another Windows port that was only working on .NET https://leveldb.angeloflogic.com/downloads/).
