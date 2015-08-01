namespace UnitTest.Shared
{
    using System;
    using LevelDBWinRT;
    using Microsoft.VisualStudio.TestPlatform.UnitTestFramework;

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

    [TestClass]
    public class ComparatorTest
    {
        private static DB LevelDB;

        [ClassInitialize]
        public static void Initialize(TestContext context)
        {
            Options options = new Options
            {
                CreateIfMissing = true,
                Comparator = new OrdinalStringComparator(),
                Compressor = CompressorType.Snappy,
                Filter = FilterType.BloomFilter,
                FilterParameters = new BloomFilterParams { BitsPerKey = 32 }
            };

            LevelDB = new DB(options, "test.ordinal.snappy");
        }

        [ClassCleanup]
        public static void Cleanup()
        {
            LevelDB.Dispose();
        }

        [TestMethod]
        public void TestPutAndOrdinalGet()
        {
            var ro = new ReadOptions();
            Assert.IsTrue(LevelDB.Put(new WriteOptions(), Slice.FromString("foo"), Slice.FromString("bar")));
            Assert.IsNotNull(LevelDB.Get(ro, Slice.FromString("Foo")));
            Assert.IsNotNull(LevelDB.Get(ro, Slice.FromString("foo")));
            Assert.AreEqual(
                "bar",
                LevelDB.Get(ro, Slice.FromString("Foo")).AsString());
        }


        [TestMethod]
        public void TestOrdinalPutReplaceValue()
        {
            var ro = new ReadOptions();
            Assert.IsTrue(LevelDB.Put(new WriteOptions(), Slice.FromString("foo"), Slice.FromString("bar")));
            Assert.IsTrue(LevelDB.Put(new WriteOptions(), Slice.FromString("Foo"), Slice.FromString("test")));
            Assert.AreEqual(
                "test",
                LevelDB.Get(ro, Slice.FromString("foo")).AsString());
        }
    }
}
