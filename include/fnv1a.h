#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

template <typename T, T Prime, T OffsetBasis>
struct basic_fnv1a
{
  using type = T;

  template <class Char>
  static constexpr Char ascii_tolower(Char c)
  {
    if ( c >= 'A' && c <= 'Z' )
      return c - ('A' - 'a');
    return c;
  }

  template <class Char>
  static constexpr Char ascii_toupper(Char c)
  {
    if ( c >= 'a' && c <= 'z' )
      return c - ('a' - 'A');
    return c;
  }

  ///////////////////////////////////////////////////////////////////////////////

  static constexpr T make_hash(
    const char *s,
    char(*fx)(char) = nullptr)
  {
    T hash = OffsetBasis;
    for ( ; *s; ++s ) {
      const auto c = fx ? fx(*s) : *s;
      hash ^= static_cast<T>(c);
      hash *= Prime;
    }
    return hash;
  }

  static constexpr T make_hash(
    const wchar_t *s,
    wchar_t(*fx)(wchar_t) = nullptr)
  {
    T hash = OffsetBasis;
    for ( ; *s; ++s ) {
      const auto c = fx ? fx(*s) : *s;
      hash ^= static_cast<T>(c & 0xff);
      hash *= Prime;
      hash ^= static_cast<T>((c >> CHAR_BIT) & 0xff);
      hash *= Prime;
    }
    return hash;
  }

#if defined(__cpp_char8_t)
  static constexpr T make_hash(
    const char8_t *s,
    char8_t(*fx)(char8_t) = nullptr)
  {
    T hash = OffsetBasis;
    for ( ; *s; ++s ) {
      const auto c = fx ? fx(*s) : *s;
      hash ^= static_cast<T>(c);
      hash *= Prime;
    }
    return hash;
  }
#endif
#if defined(__cpp_unicode_characters)
  static constexpr T make_hash(
    const char16_t *s,
    char16_t(*fx)(char16_t) = nullptr)
  {
    T hash = OffsetBasis;
    for ( ; *s; ++s ) {
      const auto c = fx ? fx(*s) : *s;
      hash ^= static_cast<T>(c & 0xff);
      hash *= Prime;
      hash ^= static_cast<T>((c >> CHAR_BIT) & 0xff);
      hash *= Prime;
    }
    return hash;
  }

  static constexpr T make_hash(
    const char32_t *s,
    char32_t(*fx)(char32_t) = nullptr)
  {
    T hash = OffsetBasis;
    for ( ; *s; ++s ) {
      auto c = fx ? fx(*s) : *s;
      hash ^= static_cast<T>(c & 0xff);
      hash *= Prime;
      c >>= CHAR_BIT;
      hash ^= static_cast<T>(c & 0xff);
      hash *= Prime;
      c >>= CHAR_BIT;
      hash ^= static_cast<T>(c & 0xff);
      hash *= Prime;
      c >>= CHAR_BIT;
      hash ^= static_cast<T>(c & 0xff);
      hash *= Prime;
    }
    return hash;
  }
#endif

  ///////////////////////////////////////////////////////////////////////////////

  static constexpr T make_hash(
    const char *s,
    std::size_t size,
    char(*fx)(char) = nullptr)
  {
    T hash = OffsetBasis;
    for ( std::size_t i = 0; i < size; ++i ) {
      const auto c = fx ? fx(s[i]) : s[i];
      hash ^= static_cast<T>(c);
      hash *= Prime;
    }
    return hash;
  }

  static constexpr T make_hash(
    const wchar_t *s,
    std::size_t size,
    wchar_t(*fx)(wchar_t) = nullptr)
  {
    T hash = OffsetBasis;
    for ( std::size_t i = 0; i < size; ++i ) {
      const auto c = fx ? fx(s[i]) : s[i];
      hash ^= static_cast<T>(c & 0xff);
      hash *= Prime;
      hash ^= static_cast<T>((c >> CHAR_BIT) & 0xff);
      hash *= Prime;
    }
    return hash;
  }

#if defined(__cpp_char8_t)
  static constexpr T make_hash(
    const char8_t *s,
    std::size_t size,
    char8_t(*fx)(char8_t) = nullptr)
  {
    T hash = OffsetBasis;
    for ( std::size_t i = 0; i < size; ++i ) {
      const auto c = fx ? fx(s[i]) : s[i];
      hash ^= static_cast<T>(c);
      hash *= Prime;
    }
    return hash;
  }
#endif
#if defined(__cpp_unicode_characters)
  static constexpr T make_hash(
    const char16_t *s,
    std::size_t size,
    char16_t(*fx)(char16_t) = nullptr)
  {
    T hash = OffsetBasis;
    for ( std::size_t i = 0; i < size; ++i ) {
      const auto c = fx ? fx(s[i]) : s[i];
      hash ^= static_cast<T>(c & 0xff);
      hash *= Prime;
      hash ^= static_cast<T>((c >> CHAR_BIT) & 0xff);
      hash *= Prime;
    }
    return hash;
  }
  static constexpr T make_hash(
    const char32_t *s,
    std::size_t size,
    char32_t(*fx)(char32_t) = nullptr)
  {
    T hash = OffsetBasis;
    for ( std::size_t i = 0; i < size; ++i ) {
      auto c = fx ? fx(s[i]) : s[i];
      hash ^= static_cast<T>(c & 0xff);
      hash *= Prime;
      c >>= CHAR_BIT;
      hash ^= static_cast<T>(c & 0xff);
      hash *= Prime;
      c >>= CHAR_BIT;
      hash ^= static_cast<T>(c & 0xff);
      hash *= Prime;
      c >>= CHAR_BIT;
      hash ^= static_cast<T>(c & 0xff);
      hash *= Prime;
    }
    return hash;
  }
#endif

  template <typename Char, typename Traits>
  static inline constexpr T make_hash(
    const std::basic_string_view<Char, Traits> &s,
     Char(*fx)(Char) = nullptr)
  {
    return make_hash(s.data(), s.size(), fx);
  }

  template <typename Char, std::size_t Size>
  static inline constexpr T make_hash(
    const std::array<Char, Size> &s,
    Char(*fx)(Char) = nullptr)
  {
    return make_hash(s.data(), s.size(), fx);
  }

  template <typename Char, typename Traits, typename Alloc>
  static inline T make_hash(
    const std::basic_string<Char, Traits, Alloc> &s,
    Char(*fx)(Char) = nullptr)
  {
    return make_hash(s.c_str(), s.size(), fx);
  }
};

using fnv1a32 = basic_fnv1a<std::uint32_t, 0x1000193UL, 2166136261UL>;
using fnv1a64 = basic_fnv1a<std::uint64_t, 0x100000001b3ULL, 14695981039346656037ULL>;
#ifdef _M_X64
using fnv1a = fnv1a64;
#else
using fnv1a = fnv1a32;
#endif

inline constexpr auto operator"" _fnv1a32(const char *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len);
}
inline constexpr auto operator"" _fnv1a32(const wchar_t *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len);
}
#if defined(__cpp_char8_t)
inline constexpr auto operator"" _fnv1a32(const char8_t *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len);
}
#endif
#if defined(__cpp_unicode_characters)
inline constexpr auto operator"" _fnv1a32(const char16_t *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len);
}
inline constexpr auto operator"" _fnv1a32(const char32_t *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len);
}
#endif

///////////////////////////////////////////////////////////////////////////////

inline constexpr auto operator"" _fnv1a32l(const char *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len, fnv1a32::ascii_tolower);
}
inline constexpr auto operator"" _fnv1a32l(const wchar_t *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len, fnv1a32::ascii_tolower);
}
#if defined(__cpp_char8_t)
inline constexpr auto operator"" _fnv1a32l(const char8_t *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len, fnv1a32::ascii_tolower);
}
#endif
#if defined(__cpp_unicode_characters)
inline constexpr auto operator"" _fnv1a32l(const char16_t *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len, fnv1a32::ascii_tolower);
}
inline constexpr auto operator"" _fnv1a32l(const char32_t *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len, fnv1a32::ascii_tolower);
}
#endif

///////////////////////////////////////////////////////////////////////////////

inline constexpr auto operator"" _fnv1a64u(const char *s, std::size_t len)
{
  return fnv1a64::make_hash(s, len, fnv1a64::ascii_toupper);
}
inline constexpr auto operator"" _fnv1a64u(const wchar_t *s, std::size_t len)
{
  return fnv1a64::make_hash(s, len, fnv1a64::ascii_toupper);
}
#if defined(__cpp_char8_t)
inline constexpr auto operator"" _fnv1a32u(const char8_t *s, std::size_t len)
{
  return fnv1a64::make_hash(s, len, fnv1a64::ascii_toupper);
}
#endif
#if defined(__cpp_unicode_characters)
inline constexpr auto operator"" _fnv1a32u(const char16_t *s, std::size_t len)
{
  return fnv1a64::make_hash(s, len, fnv1a64::ascii_toupper);
}
inline constexpr auto operator"" _fnv1a32u(const char32_t *s, std::size_t len)
{
  return fnv1a64::make_hash(s, len, fnv1a64::ascii_toupper);
}
#endif

///////////////////////////////////////////////////////////////////////////////

inline constexpr auto operator"" _fnv1a(const char *s, std::size_t len)
{
  return fnv1a::make_hash(s, len);
}
inline constexpr auto operator"" _fnv1a(const wchar_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len);
}
#if defined(__cpp_char8_t)
inline constexpr auto operator"" _fnv1a(const char8_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len);
}
#endif
#if defined(__cpp_unicode_characters)
inline constexpr auto operator"" _fnv1a(const char16_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len);
}
inline constexpr auto operator"" _fnv1a(const char32_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len);
}
#endif

///////////////////////////////////////////////////////////////////////////////

inline constexpr auto operator"" _fnv1al(const char *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::ascii_tolower);
}
inline constexpr auto operator"" _fnv1al(const wchar_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::ascii_tolower);
}
#if defined(__cpp_char8_t)
inline constexpr auto operator"" _fnv1al(const char8_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::ascii_tolower);
}
#endif
#if defined(__cpp_unicode_characters)
inline constexpr auto operator"" _fnv1al(const char16_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::ascii_tolower);
}
inline constexpr auto operator"" _fnv1al(const char32_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::ascii_tolower);
}
#endif

///////////////////////////////////////////////////////////////////////////////

inline constexpr auto operator"" _fnv1au(const char *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::ascii_toupper);
}
inline constexpr auto operator"" _fnv1au(const wchar_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::ascii_toupper);
}
#if defined(__cpp_char8_t)
inline constexpr auto operator"" _fnv1au(const char8_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::ascii_toupper);
}
#endif
#if defined(__cpp_unicode_characters)
inline constexpr auto operator"" _fnv1au(const char16_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::ascii_toupper);
}
inline constexpr auto operator"" _fnv1au(const char32_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::ascii_toupper);
}
#endif
