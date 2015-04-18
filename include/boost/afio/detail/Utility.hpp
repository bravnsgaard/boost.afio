/*
 * File:   Utility.hpp
 * Author: ned14
 *
 * Created on June 25, 2013, 12:43 PM


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/


#if !ASIO_STANDALONE
# include "boost/exception/diagnostic_information.hpp"
#endif  /*_ !ASIO_STANDALONE _*/

//! \def BOOST_AFIO_TYPEALIGNMENT(bytes) The markup this compiler uses to mark a type as having some given alignment
#ifndef BOOST_AFIO_TYPEALIGNMENT
# ifdef __cpp_alignas
#  define BOOST_AFIO_TYPEALIGNMENT(bytes) alignas(bytes)
# else  /*_ defined(__cpp_alignas) _*/
#  ifdef _MSC_VER
#   define BOOST_AFIO_TYPEALIGNMENT(bytes) __declspec(align(bytes))
#  elif defined(__GNUC__)
#   define BOOST_AFIO_TYPEALIGNMENT(bytes) __attribute__((aligned(bytes)))
#  else  /*_ defined(__GNUC__) _*/
#   define BOOST_AFIO_TYPEALIGNMENT(bytes) unknown_type_alignment_markup_for_this_compiler
#  endif  /*_ defined(__GNUC__) _*/
# endif  /*_ defined(__cpp_alignas) _*/
#endif  /*_ !defined(BOOST_AFIO_TYPEALIGNMENT) _*/

BOOST_AFIO_V1_NAMESPACE_BEGIN
namespace detail
{
#ifdef _MSC_VER
  static inline int win32_exception_filter() { return EXCEPTION_EXECUTE_HANDLER; }
  static inline void set_threadname(const char *threadName)
  {
    const DWORD MS_VC_EXCEPTION = 0x406D1388;

# pragma pack(push, 8)
    typedef struct tagTHREADNAME_INFO
    {
      DWORD dwType;      // Must be 0x1000.
      LPCSTR szName;     // Pointer to name (in user addr space).
      DWORD dwThreadID;  // Thread ID (-1=caller thread).
      DWORD dwFlags;     // Reserved for future use, must be zero.
    } THREADNAME_INFO;
# pragma pack(pop)
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadName;
    info.dwThreadID = (DWORD) -1;
    info.dwFlags = 0;

    __try { RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR *) &info); }
    __except(win32_exception_filter()) { int a = 1; }
  }
#elif defined(__linux__)
  static inline void set_threadname(const char *threadName) { pthread_setname_np(pthread_self(), threadName); }
#else  /*_ defined(__linux__) _*/
  static inline void set_threadname(const char *threadName) {}
#endif  /*_ defined(__linux__) _*/

  BOOST_AFIO_HEADERS_ONLY_FUNC_SPEC void print_fatal_exception_message_to_stderr(const char *msg);
}
BOOST_AFIO_V1_NAMESPACE_END

#ifndef BOOST_AFIO_THROW_FATAL
// Need some portable way of throwing a really absolutely definitely fatal exception
// If we guaranteed had noexcept, this would be easy, but for compilers without noexcept
// we'll bounce through extern "C" as well just to be sure
# ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4297)  // function assumed not to throw an exception but does __declspec(nothrow) or throw() was specified on the function
# endif  /*_ defined(_MSC_VER) _*/
# ifdef BOOST_AFIO_COMPILING_FOR_GCOV
#  define BOOST_AFIO_THROW_FATAL(x) std::terminate()
# else  /*_ defined(BOOST_AFIO_COMPILING_FOR_GCOV) _*/
namespace boost
{
  namespace afio
  {
    namespace fatal_exception_throw
    {
      template <class T> inline void do_throw_fatal_exception(const T &v) BOOST_NOEXCEPT_OR_NOTHROW
      {
        BOOST_AFIO_V1_NAMESPACE::detail::print_fatal_exception_message_to_stderr(v.what());
        throw v;
      }
      extern "C" inline void boost_afio_do_throw_fatal_exception(std::function<void()> impl) BOOST_NOEXCEPT_OR_NOTHROW { impl(); }
      template <class T> inline void throw_fatal_exception(const T &v) BOOST_NOEXCEPT_OR_NOTHROW
      {
        // In case the extern "C" fails to terminate, trap and terminate here
        try
        {
          std::function<void()> doer = std::bind(&do_throw_fatal_exception<T>, std::ref(v));
          boost_afio_do_throw_fatal_exception(doer);
        }
        catch(...)
        {
          std::terminate();  // Sadly won't produce much of a useful error message
        }
      }
    }
  }
}
#  define BOOST_AFIO_THROW_FATAL(x) boost::afio::fatal_exception_throw::throw_fatal_exception(x)
# endif  /*_ defined(BOOST_AFIO_COMPILING_FOR_GCOV) _*/
# ifdef _MSC_VER
#  pragma warning(pop)
# endif  /*_ defined(_MSC_VER) _*/
# define BOOST_AFIO_THROW(x) throw x
# define BOOST_AFIO_RETHROW throw
#endif  // BOOST_AFIO_THROW_FATAL


BOOST_AFIO_V1_NAMESPACE_BEGIN

namespace detail
{
  // Support for make_unique. I keep wishing it was already here!
  template <class T, class... Args> std::unique_ptr<T> make_unique(Args &&... args) { return std::unique_ptr<T>(new T(std::forward<Args>(args)...)); }

  // Support for combining hashes.
  template <class T> inline void hash_combine(std::size_t &seed, const T &v)
  {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }

  template <class T> struct decay_preserving_cv
  {
    typedef typename std::remove_reference<T>::type U;
    typedef typename std::conditional<std::is_array<U>::value, typename std::remove_extent<U>::type *, typename std::conditional<std::is_function<U>::value, typename std::add_pointer<U>::type, U>::type>::type type;
  };
  // Support for SFINAE detection of iterator/pointer ranges (Can it dereference? Can it increment?)
  //    template<class T, typename = void> struct is_rangeable : std::false_type { };
  //    template<class T> struct is_rangeable<T, decltype(*std::declval<T&>(), ++std::declval<T&>(), void())> : std::true_type { };
  // Support for SFINAE detection of containers (does it have begin() and end()?), made considerably more complex by needing MSVC to work.
  template <class T> inline auto is_container_impl(T) -> decltype(*std::begin(std::declval<T>()), *std::end(std::declval<T>()), bool()) { return true; }
  inline int is_container_impl(...) { return 0; }
  template <class T, typename = decltype(is_container_impl(std::declval<T>()))> struct is_container : std::false_type
  {
  };
  template <class T> struct is_container<T, bool> : std::true_type
  {
    typedef decltype(*std::begin(*((typename std::remove_reference<T>::type *) nullptr))) raw_type;  //!< The raw type (probably a (const) lvalue ref) returned by *it
    typedef typename decay_preserving_cv<raw_type>::type type;                                       //!< The type held by the container, still potentially const if container does not permit write access
  };

  // Debug printing of exception info
  inline std::ostream &output_exception_info(std::ostream &os, const std::exception &e) { return os << "Exception: '" << e.what() << "'"; }
#if !ASIO_STANDALONE
  inline std::ostream &output_exception_info(std::ostream &os, const boost::exception &e) { return os << "Exception: '" << boost::current_exception_diagnostic_information() << "'"; }
#endif  /*_ !ASIO_STANDALONE _*/
  inline std::ostream &output_exception_info(std::ostream &os)
  {
    try
    {
      throw;
    }
    catch(const std::exception &e)
    {
      return output_exception_info(os, e);
    }
#if !ASIO_STANDALONE
    catch(const boost::exception &e)
    {
      return output_exception_info(os, e);
    }
#endif  /*_ !ASIO_STANDALONE _*/
    catch(...)
    {
      return os << "Exception : 'unknown type'";
    }
  }

}  // namespace

#if BOOST_AFIO_USE_BOOST_THREAD
typedef boost::exception_ptr exception_ptr;
using boost::current_exception;
using boost::rethrow_exception;
#else  /*_ BOOST_AFIO_USE_BOOST_THREAD _*/
typedef std::exception_ptr exception_ptr;
using std::current_exception;
using std::rethrow_exception;
#endif  /*_ BOOST_AFIO_USE_BOOST_THREAD _*/
// Get an exception ptr from a future
template <typename T> inline exception_ptr get_exception_ptr(future<T> &f)
{
#if BOOST_AFIO_USE_BOOST_THREAD && BOOST_VERSION >= 105500
  // Thanks to Vicente for adding this to Boost.Thread
  return f.get_exception_ptr();
#else  /*_ BOOST_AFIO_USE_BOOST_THREAD && BOOST_VERSION >= 105500 _*/
  // This seems excessive but I don't see any other legal way to extract the exception ...
  bool success = false;
  try
  {
    f.get();
    success = true;
  }
  catch(...)
  {
    exception_ptr e(current_exception());
    assert(e);
    return e;
  }
  return exception_ptr();
#endif  /*_ BOOST_AFIO_USE_BOOST_THREAD && BOOST_VERSION >= 105500 _*/
}
template <typename T> inline exception_ptr get_exception_ptr(const shared_future<T> &f)
{
#if BOOST_AFIO_USE_BOOST_THREAD && BOOST_VERSION >= 105500
  // Thanks to Vicente for adding this to Boost.Thread
  return const_cast<shared_future<T> &>(f).get_exception_ptr();
#else  /*_ BOOST_AFIO_USE_BOOST_THREAD && BOOST_VERSION >= 105500 _*/
  // This seems excessive but I don't see any other legal way to extract the exception ...
  bool success = false;
  try
  {
    // std::shared_future in older libstdc++ does not have a const get().
    const_cast<shared_future<T> &>(f).get();
    success = true;
  }
  catch(...)
  {
    exception_ptr e(current_exception());
    assert(e);
    return e;
  }
  return exception_ptr();
#endif  /*_ BOOST_AFIO_USE_BOOST_THREAD && BOOST_VERSION >= 105500 _*/
}
// Is a future ready?
template <typename T> inline bool is_ready(const future<T> &f)
{
#if BOOST_AFIO_USE_BOOST_THREAD
  return f.is_ready();
#else  /*_ BOOST_AFIO_USE_BOOST_THREAD _*/
  return f.wait_for(chrono::seconds(0)) == future_status::ready;
#endif  /*_ BOOST_AFIO_USE_BOOST_THREAD _*/
}
template <typename T> inline bool is_ready(const shared_future<T> &f)
{
#if BOOST_AFIO_USE_BOOST_THREAD
  return f.is_ready();
#else  /*_ BOOST_AFIO_USE_BOOST_THREAD _*/
  return f.wait_for(chrono::seconds(0)) == future_status::ready;
#endif  /*_ BOOST_AFIO_USE_BOOST_THREAD _*/
}

struct filesystem_hash
{
  std::hash<filesystem::path::string_type> hasher;

public:
  size_t operator()(const filesystem::path &p) const { return hasher(p.native()); }
};

BOOST_AFIO_V1_NAMESPACE_END
