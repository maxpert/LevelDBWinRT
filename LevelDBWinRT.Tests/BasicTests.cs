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
