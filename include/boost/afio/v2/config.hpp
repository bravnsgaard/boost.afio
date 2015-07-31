/*
* File:   config.hpp
* Author: Paul Kirth
*
* Created on June 18, 2013, 7:30 PM


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

#if !defined(BOOST_AFIO_HEADERS_ONLY) && !defined(BOOST_ALL_DYN_LINK)
#define BOOST_AFIO_HEADERS_ONLY 1
#endif

// Fix up mingw weirdness
#if !defined(WIN32) && defined(_WIN32)
# define WIN32 1
#endif
// Boost ASIO needs this
#if !defined(_WIN32_WINNT) && defined(WIN32)
# define _WIN32_WINNT 0x0501
#endif
#if defined(WIN32) && _WIN32_WINNT<0x0501
# error _WIN32_WINNT must at least be set to Windows XP for Boost ASIO to compile
#endif
// Get Mingw to assume we are on at least Windows 2000
#if __MSVCRT_VERSION__ < 0x601
# undef __MSVCRT_VERSION__
# define __MSVCRT_VERSION__ 0x601
#endif
// Pull in detection of __MINGW64_VERSION_MAJOR
#ifdef __MINGW32__
# include <_mingw.h>
#endif

#include "../bindlib/include/import.h"

#ifndef __cpp_exceptions
# error Boost.AFIO needs C++ exceptions to be turned on
#endif
#ifndef __cpp_alias_templates
# error Boost.AFIO needs template alias support in the compiler
#endif
#ifndef __cpp_variadic_templates
# error Boost.AFIO needs variadic template support in the compiler
#endif
#ifndef __cpp_noexcept
# error Boost.AFIO needs noexcept support in the compiler
#endif
#if !defined(__cpp_constexpr) && _MSC_VER < 1900
# error Boost.AFIO needs constexpr (C++ 11) support in the compiler
#endif
#ifndef __cpp_thread_local
# error Boost.AFIO needs thread_local support in the compiler
#endif

#if defined(BOOST_AFIO_LATEST_VERSION) && BOOST_AFIO_LATEST_VERSION < 2
# error You need to include the latest version of Boost.AFIO before any earlier versions within the same translation unit
#endif
#ifndef BOOST_AFIO_LATEST_VERSION
# define BOOST_AFIO_LATEST_VERSION 2
#endif

#undef BOOST_AFIO_V2_STL11_IMPL
#undef BOOST_AFIO_V2_FILESYSTEM_IMPL
#undef BOOST_AFIO_V2_ASIO_IMPL
#undef BOOST_AFIO_V2
#undef BOOST_AFIO_V2_NAMESPACE
#undef BOOST_AFIO_V2_NAMESPACE_BEGIN
#undef BOOST_AFIO_V2_NAMESPACE_END
#undef BOOST_SPINLOCK_V1_STL11_IMPL

// Default to the C++ 11 STL for atomic, chrono, mutex and thread except on Mingw32
#if (defined(BOOST_AFIO_USE_BOOST_THREAD) && BOOST_AFIO_USE_BOOST_THREAD) || (defined(__MINGW32__) && !defined(__MINGW64__) && !defined(__MINGW64_VERSION_MAJOR))
# define BOOST_AFIO_V2_STL11_IMPL boost
# define BOOST_SPINLOCK_V1_STL11_IMPL boost
# ifndef BOOST_THREAD_VERSION
#  define BOOST_THREAD_VERSION 3
# endif
# if BOOST_THREAD_VERSION < 3
#  error Boost.AFIO requires that Boost.Thread be configured to v3 or later
# endif
#else
# define BOOST_AFIO_V2_STL11_IMPL std
# ifndef BOOST_AFIO_USE_BOOST_THREAD
#  define BOOST_AFIO_USE_BOOST_THREAD 0
# endif
#endif
// Default to the C++ 11 STL if on MSVC (Dinkumware ships a copy), else Boost
#ifndef BOOST_AFIO_USE_BOOST_FILESYSTEM
# if _MSC_VER >= 1900  // >= VS 14
#  define BOOST_AFIO_USE_BOOST_FILESYSTEM 0
# endif
#endif
#ifndef BOOST_AFIO_USE_BOOST_FILESYSTEM
# define BOOST_AFIO_USE_BOOST_FILESYSTEM 1
#endif
#if BOOST_AFIO_USE_BOOST_FILESYSTEM
# define BOOST_AFIO_V2_FILESYSTEM_IMPL boost
# define BOOST_AFIO_USE_LEGACY_FILESYSTEM_SEMANTICS 1
#else
# define BOOST_AFIO_V2_FILESYSTEM_IMPL std
#endif
// If building standalone, use a local asio, else Boost
#ifndef BOOST_AFIO_V2_ASIO_IMPL
# if ASIO_STANDALONE
#  define BOOST_AFIO_V2_ASIO_IMPL asio
# else
#  define BOOST_AFIO_V2_ASIO_IMPL boost
# endif
#endif
#if BOOST_AFIO_LATEST_VERSION == 2
# define BOOST_AFIO_V2 (boost), (afio), (BOOST_BINDLIB_NAMESPACE_VERSION(v2, BOOST_AFIO_V2_STL11_IMPL, BOOST_AFIO_V2_FILESYSTEM_IMPL, BOOST_AFIO_V2_ASIO_IMPL), inline)
#else
# define BOOST_AFIO_V2 (boost), (afio), (BOOST_BINDLIB_NAMESPACE_VERSION(v2, BOOST_AFIO_V2_STL11_IMPL, BOOST_AFIO_V2_FILESYSTEM_IMPL, BOOST_AFIO_V2_ASIO_IMPL))
#endif
#define BOOST_AFIO_V2_NAMESPACE       BOOST_BINDLIB_NAMESPACE      (BOOST_AFIO_V2)
#define BOOST_AFIO_V2_NAMESPACE_BEGIN BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_AFIO_V2)
#define BOOST_AFIO_V2_NAMESPACE_END   BOOST_BINDLIB_NAMESPACE_END  (BOOST_AFIO_V2)

// From automated matrix generator
#undef BOOST_AFIO_NEED_DEFINE
#undef BOOST_AFIO_NEED_DEFINE_DESCRIPTION
#if !BOOST_AFIO_USE_BOOST_THREAD && !BOOST_AFIO_USE_BOOST_FILESYSTEM && !ASIO_STANDALONE
# ifndef BOOST_AFIO_NEED_DEFINE_000
#  define BOOST_AFIO_NEED_DEFINE_DESCRIPTION "BOOST_AFIO_USE_BOOST_THREAD=0 BOOST_AFIO_USE_BOOST_FILESYSTEM=0 ASIO_STANDALONE=0"
#  define BOOST_AFIO_NEED_DEFINE_000
#  define BOOST_AFIO_NEED_DEFINE 1
# endif
#elif BOOST_AFIO_USE_BOOST_THREAD && !BOOST_AFIO_USE_BOOST_FILESYSTEM && !ASIO_STANDALONE
# ifndef BOOST_AFIO_NEED_DEFINE_100
#  define BOOST_AFIO_NEED_DEFINE_DESCRIPTION "BOOST_AFIO_USE_BOOST_THREAD=1 BOOST_AFIO_USE_BOOST_FILESYSTEM=0 ASIO_STANDALONE=0"
#  define BOOST_AFIO_NEED_DEFINE_100
#  define BOOST_AFIO_NEED_DEFINE 1
# endif
#elif !BOOST_AFIO_USE_BOOST_THREAD && BOOST_AFIO_USE_BOOST_FILESYSTEM && !ASIO_STANDALONE
# ifndef BOOST_AFIO_NEED_DEFINE_010
#  define BOOST_AFIO_NEED_DEFINE_DESCRIPTION "BOOST_AFIO_USE_BOOST_THREAD=0 BOOST_AFIO_USE_BOOST_FILESYSTEM=1 ASIO_STANDALONE=0"
#  define BOOST_AFIO_NEED_DEFINE_010
#  define BOOST_AFIO_NEED_DEFINE 1
# endif
#elif BOOST_AFIO_USE_BOOST_THREAD && BOOST_AFIO_USE_BOOST_FILESYSTEM && !ASIO_STANDALONE
# ifndef BOOST_AFIO_NEED_DEFINE_110
#  define BOOST_AFIO_NEED_DEFINE_DESCRIPTION "BOOST_AFIO_USE_BOOST_THREAD=1 BOOST_AFIO_USE_BOOST_FILESYSTEM=1 ASIO_STANDALONE=0"
#  define BOOST_AFIO_NEED_DEFINE_110
#  define BOOST_AFIO_NEED_DEFINE 1
# endif
#elif !BOOST_AFIO_USE_BOOST_THREAD && !BOOST_AFIO_USE_BOOST_FILESYSTEM && ASIO_STANDALONE
# ifndef BOOST_AFIO_NEED_DEFINE_001
#  define BOOST_AFIO_NEED_DEFINE_DESCRIPTION "BOOST_AFIO_USE_BOOST_THREAD=0 BOOST_AFIO_USE_BOOST_FILESYSTEM=0 ASIO_STANDALONE=1"
#  define BOOST_AFIO_NEED_DEFINE_001
#  define BOOST_AFIO_NEED_DEFINE 1
# endif
#elif BOOST_AFIO_USE_BOOST_THREAD && !BOOST_AFIO_USE_BOOST_FILESYSTEM && ASIO_STANDALONE
# ifndef BOOST_AFIO_NEED_DEFINE_101
#  define BOOST_AFIO_NEED_DEFINE_DESCRIPTION "BOOST_AFIO_USE_BOOST_THREAD=1 BOOST_AFIO_USE_BOOST_FILESYSTEM=0 ASIO_STANDALONE=1"
#  define BOOST_AFIO_NEED_DEFINE_101
#  define BOOST_AFIO_NEED_DEFINE 1
# endif
#elif !BOOST_AFIO_USE_BOOST_THREAD && BOOST_AFIO_USE_BOOST_FILESYSTEM && ASIO_STANDALONE
# ifndef BOOST_AFIO_NEED_DEFINE_011
#  define BOOST_AFIO_NEED_DEFINE_DESCRIPTION "BOOST_AFIO_USE_BOOST_THREAD=0 BOOST_AFIO_USE_BOOST_FILESYSTEM=1 ASIO_STANDALONE=1"
#  define BOOST_AFIO_NEED_DEFINE_011
#  define BOOST_AFIO_NEED_DEFINE 1
# endif
#elif BOOST_AFIO_USE_BOOST_THREAD && BOOST_AFIO_USE_BOOST_FILESYSTEM && ASIO_STANDALONE
# ifndef BOOST_AFIO_NEED_DEFINE_111
#  define BOOST_AFIO_NEED_DEFINE_DESCRIPTION "BOOST_AFIO_USE_BOOST_THREAD=1 BOOST_AFIO_USE_BOOST_FILESYSTEM=1 ASIO_STANDALONE=1"
#  define BOOST_AFIO_NEED_DEFINE_111
#  define BOOST_AFIO_NEED_DEFINE 1
# endif
#endif

#ifdef BOOST_AFIO_NEED_DEFINE

#define BOOST_STL11_ATOMIC_MAP_NAMESPACE_BEGIN        BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_AFIO_V2, (stl11, inline))
#define BOOST_STL11_ATOMIC_MAP_NAMESPACE_END          BOOST_BINDLIB_NAMESPACE_END  (BOOST_AFIO_V2, (stl11, inline))
#define BOOST_STL11_ATOMIC_MAP_NO_ATOMIC_CHAR32_T // missing VS14
#define BOOST_STL11_ATOMIC_MAP_NO_ATOMIC_CHAR16_T // missing VS14
#define BOOST_STL11_CHRONO_MAP_NAMESPACE_BEGIN        BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_AFIO_V2, (stl11, inline), (chrono))
#define BOOST_STL11_CHRONO_MAP_NAMESPACE_END          BOOST_BINDLIB_NAMESPACE_END  (BOOST_AFIO_V2, (stl11, inline), (chrono))
#define BOOST_STL11_CONDITION_VARIABLE_MAP_NAMESPACE_BEGIN BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_AFIO_V2, (stl11, inline))
#define BOOST_STL11_CONDITION_VARIABLE_MAP_NAMESPACE_END   BOOST_BINDLIB_NAMESPACE_END  (BOOST_AFIO_V2, (stl11, inline))
#define BOOST_STL1z_FILESYSTEM_MAP_NAMESPACE_BEGIN    BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_AFIO_V2, (stl1z, inline), (filesystem))
#define BOOST_STL1z_FILESYSTEM_MAP_NAMESPACE_END      BOOST_BINDLIB_NAMESPACE_END  (BOOST_AFIO_V2, (stl1z, inline), (filesystem))
// Match Dinkumware's TR2 implementation
#define BOOST_STL1z_FILESYSTEM_MAP_NO_SYMLINK_OPTION
#define BOOST_STL1z_FILESYSTEM_MAP_NO_COPY_OPTION
#define BOOST_STL1z_FILESYSTEM_MAP_NO_CHANGE_EXTENSION
#define BOOST_STL1z_FILESYSTEM_MAP_NO_WRECURSIVE_DIRECTORY_ITERATOR
#define BOOST_STL1z_FILESYSTEM_MAP_NO_EXTENSION
#define BOOST_STL1z_FILESYSTEM_MAP_NO_TYPE_PRESENT
#define BOOST_STL1z_FILESYSTEM_MAP_NO_PORTABLE_FILE_NAME
#define BOOST_STL1z_FILESYSTEM_MAP_NO_PORTABLE_DIRECTORY_NAME
#define BOOST_STL1z_FILESYSTEM_MAP_NO_PORTABLE_POSIX_NAME
#define BOOST_STL1z_FILESYSTEM_MAP_NO_LEXICOGRAPHICAL_COMPARE
#define BOOST_STL1z_FILESYSTEM_MAP_NO_WINDOWS_NAME
#define BOOST_STL1z_FILESYSTEM_MAP_NO_PORTABLE_NAME
#define BOOST_STL1z_FILESYSTEM_MAP_NO_BASENAME
#define BOOST_STL1z_FILESYSTEM_MAP_NO_COMPLETE
#define BOOST_STL1z_FILESYSTEM_MAP_NO_IS_REGULAR
#define BOOST_STL1z_FILESYSTEM_MAP_NO_INITIAL_PATH
#define BOOST_STL1z_FILESYSTEM_MAP_NO_PERMISSIONS_PRESENT
#define BOOST_STL1z_FILESYSTEM_MAP_NO_CODECVT_ERROR_CATEGORY
#define BOOST_STL1z_FILESYSTEM_MAP_NO_WPATH
#define BOOST_STL1z_FILESYSTEM_MAP_NO_SYMBOLIC_LINK_EXISTS
#define BOOST_STL1z_FILESYSTEM_MAP_NO_COPY_DIRECTORY
#define BOOST_STL1z_FILESYSTEM_MAP_NO_NATIVE
#define BOOST_STL1z_FILESYSTEM_MAP_NO_UNIQUE_PATH
#define BOOST_STL11_FUTURE_MAP_NAMESPACE_BEGIN        BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_AFIO_V2, (stl11, inline))
#define BOOST_STL11_FUTURE_MAP_NAMESPACE_END          BOOST_BINDLIB_NAMESPACE_END  (BOOST_AFIO_V2, (stl11, inline))
#define BOOST_STL11_FUTURE_MAP_NO_FUTURE
#define BOOST_STL11_MUTEX_MAP_NAMESPACE_BEGIN         BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_AFIO_V2, (stl11, inline))
#define BOOST_STL11_MUTEX_MAP_NAMESPACE_END           BOOST_BINDLIB_NAMESPACE_END  (BOOST_AFIO_V2, (stl11, inline))
#define BOOST_STL1z_NETWORKING_MAP_NAMESPACE_BEGIN    BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_AFIO_V2, (stl1z, inline), (asio))
#define BOOST_STL1z_NETWORKING_MAP_NAMESPACE_END      BOOST_BINDLIB_NAMESPACE_END  (BOOST_AFIO_V2, (stl1z, inline), (asio))
#define BOOST_STL1z_NETWORKING_MAP_NO_V4_MAPPED_T
#define BOOST_STL1z_NETWORKING_MAP_NO_EXECUTOR_ARG_T
#define BOOST_STL1z_NETWORKING_MAP_NO_WRAP
#define BOOST_STL1z_NETWORKING_MAP_NO_MAKE_WORK
#define BOOST_STL1z_NETWORKING_MAP_NO_ASYNC_COMPLETION
#define BOOST_STL1z_NETWORKING_MAP_NO_EXECUTOR_WORK
#define BOOST_STL1z_NETWORKING_MAP_NO_EXECUTION_CONTEXT
#define BOOST_STL1z_NETWORKING_MAP_NO_POST
#define BOOST_STL1z_NETWORKING_MAP_NO_EXECUTOR
#define BOOST_STL1z_NETWORKING_MAP_NO_USES_EXECUTOR
#define BOOST_STL1z_NETWORKING_MAP_NO_BAD_ADDRESS_CAST
#define BOOST_STL1z_NETWORKING_MAP_NO_BAD_EXECUTOR
#define BOOST_STL1z_NETWORKING_MAP_NO_GET_ASSOCIATED_EXECUTOR
#define BOOST_STL1z_NETWORKING_MAP_NO_ASSOCIATED_EXECUTOR
#define BOOST_STL1z_NETWORKING_MAP_NO_GET_ASSOCIATED_ALLOCATOR
#define BOOST_STL1z_NETWORKING_MAP_NO_ASSOCIATED_ALLOCATOR
#define BOOST_STL1z_NETWORKING_MAP_NO_IS_EXECUTOR
#define BOOST_STL1z_NETWORKING_MAP_NO_MAKE_ADDRESS_V4
#define BOOST_STL1z_NETWORKING_MAP_NO_MAKE_ADDRESS_V6
#define BOOST_STL1z_NETWORKING_MAP_NO_MAKE_ADDRESS
#define BOOST_STL1z_NETWORKING_MAP_NO_ADDRESS_CAST
#define BOOST_STL1z_NETWORKING_MAP_NO_DEFER
#define BOOST_STL1z_NETWORKING_MAP_NO_DISPATCH
#define BOOST_STL1z_NETWORKING_MAP_NO_THREAD_POOL
#define BOOST_STL1z_NETWORKING_MAP_NO_IS_MUTABLE_BUFFER_SEQUENCE
#define BOOST_STL1z_NETWORKING_MAP_NO_IS_CONST_BUFFER_SEQUENCE
#define BOOST_STL1z_NETWORKING_MAP_NO_EXECUTOR_WRAPPER
#define BOOST_STL1z_NETWORKING_MAP_NO_SYSTEM_EXECUTOR
#define BOOST_STL1z_NETWORKING_MAP_NO_HANDLER_TYPE
#define BOOST_STL1z_NETWORKING_MAP_NO_STRAND
#define BOOST_STL1z_NETWORKING_MAP_NO_READ_SIZE_HELPER
#define BOOST_STL11_RATIO_MAP_NAMESPACE_BEGIN         BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_AFIO_V2, (stl11, inline))
#define BOOST_STL11_RATIO_MAP_NAMESPACE_END           BOOST_BINDLIB_NAMESPACE_END  (BOOST_AFIO_V2, (stl11, inline))
#define BOOST_STL11_THREAD_MAP_NAMESPACE_BEGIN        BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_AFIO_V2, (stl11, inline))
#define BOOST_STL11_THREAD_MAP_NAMESPACE_END          BOOST_BINDLIB_NAMESPACE_END  (BOOST_AFIO_V2, (stl11, inline))
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_AFIO_V2_STL11_IMPL, atomic)
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_AFIO_V2_STL11_IMPL, chrono)
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_AFIO_V2_STL11_IMPL, condition_variable)
#include BOOST_BINDLIB_INCLUDE_STL1z(../bindlib, BOOST_AFIO_V2_FILESYSTEM_IMPL, filesystem)
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_AFIO_V2_STL11_IMPL, future)
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_AFIO_V2_STL11_IMPL, mutex)
#include BOOST_BINDLIB_INCLUDE_STL1z(../bindlib, BOOST_AFIO_V2_ASIO_IMPL, networking)
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_AFIO_V2_STL11_IMPL, ratio)
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_AFIO_V2_STL11_IMPL, thread)

#define BOOST_STL1z_NETWORKING_MAP_NAMESPACE_BEGIN    BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_AFIO_V2, (stl1z, inline), (asio))
#define BOOST_STL1z_NETWORKING_MAP_NAMESPACE_END      BOOST_BINDLIB_NAMESPACE_END  (BOOST_AFIO_V2, (stl1z, inline), (asio))
BOOST_STL1z_NETWORKING_MAP_NAMESPACE_BEGIN
// Need to bind in asio::windows
#ifdef WIN32
# if ASIO_STANDALONE
namespace error = ::asio::error;
namespace windows = ::asio::windows;
# else
namespace error = ::boost::asio::error;
namespace windows = ::boost::asio::windows;
# endif
#endif
BOOST_STL1z_NETWORKING_MAP_NAMESPACE_END
#undef BOOST_STL1z_NETWORKING_MAP_NAMESPACE_BEGIN
#undef BOOST_STL1z_NETWORKING_MAP_NAMESPACE_END

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Map an error category
BOOST_AFIO_V2_NAMESPACE_BEGIN
using std::to_string;

#if BOOST_AFIO_USE_BOOST_THREAD
//using boost::make_exception_ptr;
template<class T> inline boost::exception_ptr make_exception_ptr(T e)
{
  try
  {
    throw e;
  }
  catch(...)
  {
    return boost::current_exception();
  }
}
template<class _Res> using stl_future = ::boost::future<_Res>;
#else
using std::make_exception_ptr;
template<class _Res> using stl_future = ::std::future<_Res>;
#endif

#if ASIO_STANDALONE
typedef asio::error_code error_code;
using std::generic_category;
using std::system_category;
using std::system_error;
#else
typedef boost::system::error_code error_code;
using boost::system::generic_category;
using boost::system::system_category;
using boost::system::system_error;
#endif
#if defined(_MSC_VER) && 0
// Stupid MSVC doesn't resolve namespace binds correctly ...
#if ASIO_STANDALONE
namespace asio {
  namespace asio = ::asio;
  namespace detail { namespace asio = ::asio; }
  //namespace windows { namespace asio = ::asio; }
}
namespace asio_handler_cont_helpers { namespace asio = ::asio; }
namespace asio_handler_alloc_helpers { namespace asio = ::asio; }
namespace asio_handler_invoke_helpers { namespace asio = ::asio; }

#else
namespace boost { namespace asio {
  namespace asio = ::boost::asio;
  namespace detail { namespace asio = ::boost::asio; }
  namespace windows { namespace asio = ::boost::asio; }
} }
#endif
#endif
BOOST_AFIO_V2_NAMESPACE_END
#endif

#include "../spinlock/include/boost/spinlock/monad.hpp"
BOOST_AFIO_V2_NAMESPACE_BEGIN
  using BOOST_SPINLOCK_V1_NAMESPACE::is_lockable_locked;
  using spins_to_sleep = BOOST_SPINLOCK_V1_NAMESPACE::spins_to_sleep;
  template<size_t _0> using spins_to_yield = BOOST_SPINLOCK_V1_NAMESPACE::spins_to_yield<_0>;
  template<size_t _0, bool _1=true> using spins_to_loop = BOOST_SPINLOCK_V1_NAMESPACE::spins_to_loop<_0, _1>;
  using null_spin_policy = BOOST_SPINLOCK_V1_NAMESPACE::null_spin_policy;
  template<class T> using spinlockbase = BOOST_SPINLOCK_V1_NAMESPACE::spinlockbase<T>;
  template<class T> using lockable_ptr = BOOST_SPINLOCK_V1_NAMESPACE::lockable_ptr<T>;
  template<typename T, template<class> class spinpolicy2=spins_to_loop<125>::policy, template<class> class spinpolicy3=spins_to_yield<250>::policy, template<class> class spinpolicy4=spins_to_sleep::policy> using spinlock = BOOST_SPINLOCK_V1_NAMESPACE::spinlock<T, spinpolicy2, spinpolicy3, spinpolicy4>;
  template<typename R> using monad = BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::monad<R>;
  template<typename R> using result = BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::result<R>;
  template<typename R> using option = BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::option<R>;
  using BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::empty;
  using BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::make_option;
  using BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::monad_errc;
  using BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::monad_category;
  BOOST_AFIO_V2_NAMESPACE_END

///////////////////////////////////////////////////////////////////////////////
//  Set up dll import/export options
#if (defined(BOOST_AFIO_DYN_LINK) || defined(BOOST_ALL_DYN_LINK)) && \
    !defined(BOOST_AFIO_STATIC_LINK)

# if defined(BOOST_AFIO_SOURCE)
#  undef BOOST_AFIO_HEADERS_ONLY
#  define BOOST_AFIO_DECL BOOST_SYMBOL_EXPORT
#  define BOOST_AFIO_BUILD_DLL
# else
#  define BOOST_AFIO_DECL
# endif
#else
# define BOOST_AFIO_DECL
#endif // building a shared library


///////////////////////////////////////////////////////////////////////////////
//  Auto library naming
#if !defined(BOOST_AFIO_SOURCE) && !defined(BOOST_ALL_NO_LIB) && \
    !defined(BOOST_AFIO_NO_LIB) && !AFIO_STANDALONE && !BOOST_AFIO_HEADERS_ONLY

#define BOOST_LIB_NAME boost_afio

// tell the auto-link code to select a dll when required:
#if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_AFIO_DYN_LINK)
#define BOOST_DYN_LINK
#endif

#include <boost/config/auto_link.hpp>

#endif  // auto-linking disabled

//#define BOOST_THREAD_VERSION 4
//#define BOOST_THREAD_PROVIDES_VARIADIC_THREAD
//#define BOOST_THREAD_DONT_PROVIDE_FUTURE
//#define BOOST_THREAD_PROVIDES_SIGNATURE_PACKAGED_TASK
#if BOOST_AFIO_HEADERS_ONLY == 1
# define BOOST_AFIO_HEADERS_ONLY_FUNC_SPEC inline
# define BOOST_AFIO_HEADERS_ONLY_MEMFUNC_SPEC inline
# define BOOST_AFIO_HEADERS_ONLY_VIRTUAL_SPEC inline virtual
// GCC gets upset if inline virtual functions aren't defined
# ifdef BOOST_GCC
#  define BOOST_AFIO_HEADERS_ONLY_VIRTUAL_UNDEFINED_SPEC { BOOST_AFIO_THROW_FATAL(std::runtime_error("Attempt to call pure virtual member function")); abort(); }
# else
#  define BOOST_AFIO_HEADERS_ONLY_VIRTUAL_UNDEFINED_SPEC =0;
# endif
#else
# define BOOST_AFIO_HEADERS_ONLY_FUNC_SPEC extern BOOST_AFIO_DECL
# define BOOST_AFIO_HEADERS_ONLY_MEMFUNC_SPEC
# define BOOST_AFIO_HEADERS_ONLY_VIRTUAL_SPEC virtual
# define BOOST_AFIO_HEADERS_ONLY_VIRTUAL_UNDEFINED_SPEC =0;
#endif

#if defined(__has_feature)
# if __has_feature(thread_sanitizer)
#  define BOOST_AFIO_DISABLE_THREAD_SANITIZE __attribute__((no_sanitize_thread))
# endif
#endif
#ifndef BOOST_AFIO_DISABLE_THREAD_SANITIZE
# define BOOST_AFIO_DISABLE_THREAD_SANITIZE
#endif

#ifndef BOOST_AFIO_LAMBDA_MOVE_CAPTURE
# ifdef __cpp_init_captures
#  define BOOST_AFIO_LAMBDA_MOVE_CAPTURE(var) var=std::move(var)
# else
#  define BOOST_AFIO_LAMBDA_MOVE_CAPTURE(var) var
# endif
#endif

#ifndef BOOST_AFIO_THREAD_LOCAL
# ifdef __cpp_thread_local
#  define BOOST_AFIO_THREAD_LOCAL thread_local
# elif defined(_MSC_VER)
#  define BOOST_AFIO_THREAD_LOCAL __declspec(thread)
# elif defined(__GNUC__)
#  define BOOST_AFIO_THREAD_LOCAL __thread
# else
#  error Unknown compiler, cannot set BOOST_AFIO_THREAD_LOCAL
# endif
#endif


#endif  // BOOST_AFIO_NEED_DEFINE

