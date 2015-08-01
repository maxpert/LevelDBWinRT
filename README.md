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
  * Builtin compression support with Snappy.
  * Custom comparator support.
  * ARM or x86 architecture supported.

# Examples

The library contains basic constructs that serve as a wrapper around C++ LevelDB API. Most of them are aligned close to the native API but some changes have been made here and there to comply .NET and future support. You can create a database simply by creating a LevelDBWinRT.DB object:

```C#
LevelDB = new DB(options, "test.ordinal.snappy");
```

__NOTE:__ if you don't specify an absolute path to the database it's created under ApplicationData.Current.LocalFolder. If a complete path is specified the directory will be created under the specified path. If the database is nested inside folders make sure the parent directory tree has been created before opening the database.
Options object can be created specifying various parameters:

```C#
Options options = new Options
{
    CreateIfMissing = true,
    Compressor = CompressorType.Snappy,
    Filter = FilterType.BloomFilter,
    FilterParameters = new BloomFilterParams { BitsPerKey = 32 }
};
```

Once you have used database object you can simply close it by calling Dispose method on database object:

```C#
LevelDB.Dispose();
```

The database object supports all basic operations:

```C#
ReadOptions readOptions = new ReadOptions();
WriteOptions writeOptions = new WriteOptions();

bool putSuccess = LevelDB.Put(writeOptions, Slice.FromString("foo"), Slice.FromString("bar"));
Slice value = LevelDB.Get(readOptions, Slice.FromString("foo"));

if (value == null)
{
  Debug.WriteLine("Key not found");
}
else
{
  Debug.WriteLine("Key value = {0}", value.AsString());
}

LevelDB.Delete(new WriteOptions(), Slice.FromString("foo"))

WriteBatch writeBatch = new WriteBatch();
for (var i = 0; i < 10; i++)
{
   writeBatch.Put(Slice.FromString("Key"+i), Slice.FromByteArray(BitConverter.GetBytes(i)));
}

writeBatch.Delete(Slice.FromString("Key5"));

bool writeBatch = LevelDB.Write(writeOptions, writeBatch);
```

## Key comparators

A key comparator can be implemented by implementing a class from IKeyComparator, here is a example of case insensitive key comparator:

```C#
internal class OrdinalStringComparator : IKeyComparator
{
    public int Compare(Slice a, Slice b)
    {
        return string.Compare(a.AsString(), b.AsString(), StringComparison.OrdinalIgnoreCase);
    }

    public string Name
    {
        get { return "OrdinalStringComparator"; }
    }
}
```

This comparator can then be passed in to Options object:

```C#
Options options = new Options
{
    CreateIfMissing = true,
    Comparator = new OrdinalStringComparator(),
    Compressor = CompressorType.Snappy,
    Filter = FilterType.BloomFilter,
    FilterParameters = new BloomFilterParams { BitsPerKey = 32 }
};
```

_Once a database has been created with a specific comparator it must be always opened with same comparator (Atleast name should match)_. Make sure your Compare method never throws an exception otherwise the behavior is undefined.

# TODO:

  * Use native Windows API instead of fstream to ensure better Flush and Sync.

This is an unoffical port of LevelDB for Windows Runtime (forked and modified from another Windows port that was only working on .NET https://leveldb.angeloflogic.com/downloads/).
