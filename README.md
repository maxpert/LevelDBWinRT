# LevelDB Windows Runtime Component

 This project aims to enable usage of LevelDB on Windows 10 UWP (Windows Phone 8.1+, Windows 8.1+ platforms have been deprecated). LevelDB stores keys and values in arbitrary byte arrays, and data is sorted by key. It supports batching writes, forward and backward iteration, and compression of the data via Google's Snappy compression library.

LevelDB is a fast key-value storage library written at Google that provides an ordered mapping from string keys to string values.
LevelDB Authors: Sanjay Ghemawat (sanjay@google.com) and Jeff Dean (jeff@google.com)

# Is it production ready?

 Yes! It is production ready.
 
# Project status 

 > The original goal of this project was to port LevelDB to UWP platform. Project has been stablized and is being used by various applications and projects for past few years now. With dismissal of UWP from phones and lesser tracktion I've decided to code-freeze the project. Any security patches or fixes that won't change the API are welcome, but no major updates will be released. With my limited time I try my best to people opening issues or send out emails.

# Where to download?

### [Download](https://www.nuget.org/packages/LevelDB.UWP) Nuget

# Features
  * Keys and values can be arbitrary byte arrays (Slice.FromArray)
  * Data is stored and sorted by key.
  * Basic Get(key)/Put(key, value)/Delete(key) support.
  * Atomic batch operations can make multiple changes.
  * Users can create transient snapshot to get a consistent view of data.
  * Iterator support, with forward and backward iteration.
  * Basic wrappers for WriteOptions, ReadOptions, Options.
  * Builtin compression support with Snappy.
  * Custom comparator support (experimental).
  * ARM, x86, and x64 architecture support.

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

__Once a database has been created with a specific comparator it must be always opened with same comparator (Atleast name should match)__. Make sure your Compare method never throws an exception otherwise the behavior is undefined.

# Documentation

 Checkout project wiki for extended and detailed documentation
