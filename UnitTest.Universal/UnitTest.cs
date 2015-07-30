namespace UnitTest.Universal
{
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
            this.levelDB = new DB("foo.bar");
        }

        [TestCleanup]
        public void Cleanup()
        {
            this.levelDB.Dispose();
        }

        [TestMethod]
        public void TestPut()
        {
            this.levelDB.Put(new WriteOptions(), Slice.FromString("foo"), Slice.FromString("bar"));
        }
    }
}
