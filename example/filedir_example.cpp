#include "afio_pch.hpp"

int main(void)
{
  //[filedir_example
  using namespace BOOST_AFIO_V2_NAMESPACE;
  using BOOST_AFIO_V2_NAMESPACE::rmdir;
  std::shared_ptr<boost::afio::dispatcher> dispatcher =
    boost::afio::make_dispatcher().get();
  current_dispatcher_guard h(dispatcher);

  // Free function
  try
  {
    // Schedule creating a directory called testdir
    auto mkdir(async_dir("testdir", boost::afio::file_flags::create)());
    // Schedule creating a file called testfile in testdir only when testdir has been created
    auto mkfile(mkdir.then(async_file("testfile", boost::afio::file_flags::create)));
    // Schedule creating a symbolic link called linktodir to the item referred to by the precondition
    // i.e. testdir. Note that on Windows you can only symbolic link directories. Note that creating
    // symlinks must *always* be as an absolute path, as that is how they are stored.
    auto mklink(mkdir.then(async_symlink(path::make_absolute("testdir/linktodir"),
      boost::afio::file_flags::create)));

    // Schedule deleting the symbolic link only after when it has been created
    auto rmlink(mklink.then(async_rmsymlink()));
    // Schedule deleting the file only after when it has been created
    auto rmfile(mkfile.then(async_rmfile()).then(async_close()));
    // Schedule waiting until both the preceding operations have finished
    auto barrier(dispatcher->barrier({ rmlink, rmfile }));
    // Schedule deleting the directory only after the barrier completes
    auto rmdir(dispatcher->depends(barrier.front(), mkdir).then(async_rmdir()));
    // Check ops for errors
    boost::afio::when_all(mkdir, mkfile, mklink, rmlink, rmfile, rmdir).wait();
  }
  catch (...)
  {
    std::cerr << boost::current_exception_diagnostic_information(true) << std::endl;
    throw;
  }

  // Batch
  try
  {
      // Schedule creating a directory called testdir
      auto mkdir(dispatcher->dir(std::vector<boost::afio::async_path_op_req>(1,
        boost::afio::async_path_op_req("testdir", boost::afio::file_flags::create))).front());
      // Schedule creating a file called testfile in testdir only when testdir has been created
      auto mkfile(dispatcher->file(std::vector<boost::afio::async_path_op_req>(1,
        boost::afio::async_path_op_req::relative(mkdir, "testfile",
          boost::afio::file_flags::create))).front());
      // Schedule creating a symbolic link called linktodir to the item referred to by the precondition
      // i.e. testdir. Note that on Windows you can only symbolic link directories. Note that creating
      // symlinks must *always* be as an absolute path, as that is how they are stored.
      auto mklink(dispatcher->symlink(std::vector<boost::afio::async_path_op_req>(1, 
        boost::afio::async_path_op_req::absolute(mkdir, "testdir/linktodir",
          boost::afio::file_flags::create))).front());

      // Schedule deleting the symbolic link only after when it has been created
      auto rmlink(dispatcher->close(std::vector<future<>>(1, dispatcher->rmsymlink(mklink))).front());
      // Schedule deleting the file only after when it has been created
      auto rmfile(dispatcher->close(std::vector<future<>>(1, dispatcher->rmfile(mkfile))).front());
      // Schedule waiting until both the preceding operations have finished
      auto barrier(dispatcher->barrier({rmlink, rmfile}));
      // Schedule deleting the directory only after the barrier completes
      auto rmdir(dispatcher->rmdir(std::vector<async_path_op_req>(1, dispatcher->depends(barrier.front(), mkdir))).front());
      // Check ops for errors
      boost::afio::when_all(mkdir, mkfile, mklink, rmlink, rmfile, rmdir).wait();
  }
  catch(...)
  {
      std::cerr << boost::current_exception_diagnostic_information(true) << std::endl;
      throw;
  }
  //]
}
