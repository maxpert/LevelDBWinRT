using Microsoft.VisualStudio.TestPlatform.UnitTestFramework;

namespace LevelDBWinRT.Tests
{
    [TestClass]
    public class BasicTests
    {
        [TestMethod]
        public void TestCreateDisposeDatabase()
        {
            using (var db = new DB(new Options{ CreateIfMissing = true }, "test1.db"))
            {
                Assert.IsNotNull(db);
            }
        }

        [TestMethod]
        public void TestCreatePutDisposeDatabase()
        {
            using (var db = new DB(new Options { CreateIfMissing = true }, "test1.db"))
            {
                var saved = db.Put(new WriteOptions { Sync = true }, Slice.FromString("Foo"), Slice.FromString("Bar"));
                Assert.IsTrue(saved);
            }
        }

        [TestMethod]
        public void TestReadWriteDatabase()
        {
            using (var db = new DB(new Options { CreateIfMissing = true }, "test1.db"))
            {
                db.Put(new WriteOptions(), Slice.FromString("Foo"), Slice.FromString("Bar"));
                Assert.IsNotNull(db.Get(new ReadOptions(), Slice.FromString("Foo")));
            }
        }

        [TestMethod]
        public void TestReadMissingEntryDatabase()
        {
            using (var db = new DB(new Options { CreateIfMissing = true }, "test1.db"))
            {
                Assert.IsNull(db.Get(new ReadOptions(), Slice.FromString("boo")));
            }
        }

        [TestMethod]
        public void TestReadFromSnapshotOfDatabase()
        {
            using (var db = new DB(new Options { CreateIfMissing = true }, "test1.db"))
            {
                db.Put(new WriteOptions(), Slice.FromString("Foo"), Slice.FromString("Bar"));
                using (var snapshot = db.GetSnapshot())
                {
                    var opts = new ReadOptions{ Snapshot = snapshot };
                    Assert.IsNotNull(db.Get(opts, Slice.FromString("Foo")));
                    Assert.IsTrue(db.Delete(new WriteOptions(), Slice.FromString("Foo")));
                    Assert.IsNotNull(db.Get(opts, Slice.FromString("Foo")));
                }
            }
        }

        [TestMethod]
        public void TestIteratorWithSnapshot()
        {
            using (var db = new DB(new Options { CreateIfMissing = true }, "test2.db"))
            {
                using (var batch = new WriteBatch())
                {
                    for (int i = 0; i < 100; i++)
                    {
                        batch.Put(Slice.FromString($"key::{i,20:D20}"), Slice.FromString($"{i,32}"));
                    }

                    db.Write(new WriteOptions { Sync = true }, batch);
                }

                using (var snapshot = db.GetSnapshot())
                using (var batch = new WriteBatch())
                using (var itr = db.NewIterator(new ReadOptions { Snapshot = snapshot }))
                {
                    itr.Seek(Slice.FromString("key::"));
                    Assert.IsTrue(itr.Valid());
                    int entriesDeleted = 0;
                    while (itr.Valid())
                    {
                        batch.Delete(itr.Key());
                        itr.Next();
                        entriesDeleted++;
                    }

                    db.Write(new WriteOptions(), batch);
                    Assert.AreEqual(100, entriesDeleted);
                }
            }
        }

        [TestMethod]
        public void TestDeleteEntryFromDatabase()
        {
            using (var db = new DB(new Options { CreateIfMissing = true }, "test1.db"))
            {
                db.Put(new WriteOptions(), Slice.FromString("Foo"), Slice.FromString("Bar"));
                Assert.IsNotNull(db.Get(new ReadOptions(), Slice.FromString("Foo")));
                Assert.IsTrue(db.Delete(new WriteOptions(), Slice.FromString("Foo")));
                Assert.IsNull(db.Get(new ReadOptions(), Slice.FromString("Foo")));
            }
        }
    }
}
