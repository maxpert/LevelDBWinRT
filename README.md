**LevelDB is a fast key-value storage library written at Google that provides an ordered mapping from string keys to string values.**

LevelDB Authors: Sanjay Ghemawat (sanjay@google.com) and Jeff Dean (jeff@google.com)

# Features
  * Keys and values are arbitrary byte arrays.
  * Data is stored sorted by key.
  * Callers can provide a custom comparison function to override the sort order.
  * The basic operations are `Put(key,value)`, `Get(key)`, `Delete(key)`.
  * Multiple changes can be made in one atomic batch.
  * Users can create a transient snapshot to get a consistent view of data.
  * Forward and backward iteration is supported over the data.
  * Data is automatically compressed using the [Snappy compression library](http://code.google.com/p/snappy).
  * External activity (file system operations etc.) is relayed through a virtual interface so users can customize the operating system interactions.
  * [Detailed documentation](http://htmlpreview.github.io/?https://github.com/google/leveldb/blob/master/doc/index.html) about how to use the library is included with the source code.


# Limitations
  * This is not a SQL database.  It does not have a relational data model, it does not support SQL queries, and it has no support for indexes.
  * Only a single process (possibly multi-threaded) can access a particular database at a time.
  * There is no client-server support builtin to the library.  An application that needs such support will have to wrap their own server around the library.

# Performance

Here is a performance report (with explanations) from the run of the
included db_bench program.  The results are somewhat noisy, but should
be enough to get a ballpark performance estimate.

# WinRT Port Notes

  * Basic Get/Put/Delete support (work in progress).
  * To use the library either add the corresponding project to your solution and refer it in your dependencies.
  * Be sure to choose ARM or x86 architecture for your project.
  * Snappy compression not enabled yet.

# TODO:

  * Introduce Batch write operation support.
  * Compression support.
  * Comparators support.
  
