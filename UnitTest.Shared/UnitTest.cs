namespace UnitTest.Universal
{
    using System;
    using System.Collections.Generic;
    using System.Threading.Tasks;
    using LevelDBWinRT;
    using Microsoft.VisualStudio.TestPlatform.UnitTestFramework;

    [TestClass]
    public class BasicAPITests
    {
        private static DB LevelDB;

        [AssemblyInitialize]
        public static void AssemblyInit(TestContext context)
        {
            
        }

        [ClassInitialize]
        public static void Initialize(TestContext context)
        {
            Options options = new Options
            {
                CreateIfMissing = true,
                Compressor = CompressorType.Snappy,
                Filter = FilterType.BloomFilter,
                FilterParameters = new BloomFilterParams { BitsPerKey = 64 }
            };

            LevelDB = new DB(options, "test.snappy");
        }

        [ClassCleanup]
        public static void Cleanup()
        {
            LevelDB.Dispose();
        }

        [TestMethod]
        public void TestPut()
        {
            Assert.IsTrue(LevelDB.Put(new WriteOptions(), Slice.FromString("foo"), Slice.FromString("bar")));
            Assert.AreEqual(
                "bar",
                LevelDB.Get(new ReadOptions(), Slice.FromString("foo")).AsString());
        }


        [TestMethod]
        public void TestExactGetPut()
        {
            var readOptions = new ReadOptions();
            Assert.IsTrue(LevelDB.Put(new WriteOptions(), Slice.FromString("foo"), Slice.FromString("bar")));

            Assert.IsNotNull(LevelDB.Get(readOptions, Slice.FromString("foo")));
            Assert.IsNull(LevelDB.Get(readOptions, Slice.FromString("f")));
            Assert.IsNull(LevelDB.Get(readOptions, Slice.FromString("g")));
            Assert.AreEqual(
                "bar",
                LevelDB.Get(readOptions, Slice.FromString("foo")).AsString());
        }

        [TestMethod]
        public void TestDelete()
        {
            Assert.IsTrue(LevelDB.Put(new WriteOptions(), Slice.FromString("foo"), Slice.FromString("bar")));
            Assert.IsTrue(LevelDB.Delete(new WriteOptions(), Slice.FromString("foo")));
            Assert.AreEqual(
                null,
                LevelDB.Get(new ReadOptions(), Slice.FromString("foo")));
        }


        [TestMethod]
        public void TestBatchWrite()
        {
            WriteBatch writeBatch = new WriteBatch();
            ReadOptions readOptions = new ReadOptions();
            WriteOptions writeOptions = new WriteOptions();
            for (var i = 0; i < 10; i++)
            {
                writeBatch.Put(Slice.FromString("Key"+i), Slice.FromByteArray(BitConverter.GetBytes(i)));
            }

            LevelDB.Write(writeOptions, writeBatch);
            
            for (var j = 0; j < 10; j++)
            {
                Slice key = Slice.FromString("Key" + j);
                Slice readSlice = LevelDB.Get(readOptions, key);
                Assert.IsNotNull(readSlice);
                Assert.AreEqual(j, BitConverter.ToInt32(readSlice.ToByteArray(), 0));
                Assert.IsTrue(LevelDB.Delete(writeOptions, key));
            }
        }

        [TestMethod]
        public void TestParallelWrites()
        {
            const int totalInserts = 100;
            var tasks = new List<Task<int>>();
            for (var i = 0; i < 10; i++)
            {
               tasks.Add(this.ParallelWriter(totalInserts)); 
            }

            Task.WaitAll(tasks.ToArray());

            foreach (var task in tasks)
            {
                Assert.AreEqual(totalInserts, task.Result);
            }
        }

        private async Task<int> ParallelWriter(int totalInserts)
        {
            WriteOptions writeOptions = new WriteOptions();
            int counter = 0;
            for (var i = 0; i < totalInserts; i++)
            {
                if (LevelDB.Put(
                    writeOptions, 
                    Slice.FromByteArray(BitConverter.GetBytes(i)),
                    Slice.FromString(DateTime.Now.ToString())))
                {
                    counter++;
                }

                await Task.Delay(10).ConfigureAwait(false);
            }

            return counter;
        }
    }
}
