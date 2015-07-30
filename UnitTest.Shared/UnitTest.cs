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
        private DB levelDB;

        [AssemblyInitialize]
        public static void AssemblyInit(TestContext context)
        {
            
        }

        [TestInitialize]
        public void Initialize()
        {
            Options options = new Options
            {
                Filter = FilterType.BloomFilter,
                FilterParameters = new BloomFilterParams { BitsPerKey = 2048 }
            };

            this.levelDB = new DB(options, "foo.bar");
        }

        [TestCleanup]
        public void Cleanup()
        {
            this.levelDB.Dispose();
        }

        [TestMethod]
        public void TestPut()
        {
            Assert.IsTrue(this.levelDB.Put(new WriteOptions(), Slice.FromString("foo"), Slice.FromString("bar")));
            Assert.AreEqual(
                "bar",
                this.levelDB.Get(new ReadOptions(), Slice.FromString("foo")).AsString());
        }

        [TestMethod]
        public void TestDelete()
        {
            Assert.IsTrue(this.levelDB.Put(new WriteOptions(), Slice.FromString("foo"), Slice.FromString("bar")));
            Assert.IsTrue(this.levelDB.Delete(new WriteOptions(), Slice.FromString("foo")));
            Assert.AreEqual(
                null,
                this.levelDB.Get(new ReadOptions(), Slice.FromString("foo")));
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

            this.levelDB.Write(writeOptions, writeBatch);
            
            for (var j = 0; j < 10; j++)
            {
                Slice key = Slice.FromString("Key" + j);
                Slice readSlice = this.levelDB.Get(readOptions, key);
                Assert.IsNotNull(readSlice);
                Assert.AreEqual(j, BitConverter.ToInt32(readSlice.ToByteArray(), 0));
                Assert.IsTrue(this.levelDB.Delete(writeOptions, key));
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
                if (this.levelDB.Put(
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
