using System.Collections.Generic;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestPlatform.UnitTestFramework;

namespace LevelDBWinRT.Tests
{
    [TestClass]
    public class MultiThreadedTests
    {
        [TestMethod]
        public void TestMultipulteConnectionWrite()
        {
            var tasks = new List<Task>(10);
            for (int i = 0; i < 10; i++)
            {
                tasks.Add(WriteRangeInTask("mtest1.db", i * 100, 50));
            }

            Task.WaitAll(tasks.ToArray());
        }

        [TestMethod]
        public void TestMultipuleWriteAndWriteBatch()
        {
            var tasks = new List<Task>(20);
            for (int i = 0; i < 10; i++)
            {
                tasks.Add(WriteRangeInTask("mtest1.db", i * 100, 50));
            }

            for (int i = 0; i < 10; i++)
            {
                tasks.Add(BatchWriteRangeInTask("mtest1.db", i * 100, 50));
            }

            Task.WaitAll(tasks.ToArray());
        }


        [TestMethod]
        public void TestMultipule()
        {
            var tasks = new List<Task>(30);
            for (int i = 0; i < 10; i++)
            {
                tasks.Add(WriteRangeInTask("mtest1.db", i * 100, 50));
            }

            for (int i = 0; i < 10; i++)
            {
                tasks.Add(BatchWriteRangeInTask("mtest1.db", i * 100, 50));
            }


            for (int i = 0; i < 10; i++)
            {
                tasks.Add(ReadRangeInTask("mtest1.db", i * 100, 50));
            }

            Task.WaitAll(tasks.ToArray());
        }

        private static Task WriteRangeInTask(string dbName, int offset, int limit)
        {
            return Task.Run(() =>
            {
                var writeOpts = new WriteOptions();
                using (var db = new DB(new Options { CreateIfMissing = true }, dbName))
                {
                    for (int i = offset; i < limit; i++)
                    {
                        db.Put(writeOpts, Slice.FromString($"key::{i,20:D20}"), Slice.FromString($"{i,32}"));
                    }
                }
            });
        }

        private static Task BatchWriteRangeInTask(string dbName, int offset, int limit)
        {
            return Task.Run(() =>
            {
                using (var db = new DB(new Options { CreateIfMissing = true }, dbName))
                using (var batch = new WriteBatch())
                {
                    for (int i = offset; i < limit; i++)
                    {
                        batch.Put(Slice.FromString($"key::{i,20:D20}"), Slice.FromString($"{i,32}"));
                    }

                    db.Write(new WriteOptions { Sync = true }, batch);
                }
            });
        }

        private static Task ReadRangeInTask(string dbName, int offset, int limit)
        {
            return Task.Run(() =>
            {
                using (var db = new DB(new Options { CreateIfMissing = true }, dbName))
                {
                    using (var snapshot = db.GetSnapshot())
                    using (var itr = db.NewIterator(new ReadOptions { Snapshot = snapshot }))
                    {
                        itr.Seek(Slice.FromString($"key::{offset,20:D20}"));
                        while (itr.Valid())
                        {
                            itr.Next();
                        }
                    }
                }
            });
        }
    }
}