﻿#ifndef __TSTRINGW_H__
#define __TSTRINGW_H__

#include <utilities-def.h>
#include <interface/sstring-i.h>
#include <string/sstringdata.h>

SNSBEGIN

/// @brief Character traits for ASCII strings.
struct UTILITIES_API wchar_traits
{
    /// @brief Computes the length of a null-terminated string.
    /// @param psz The string to measure.
    /// @return The length of the string.
    static size_t StrLen(const wchar_t* psz);

    /// @brief Compares two strings lexicographically.
    /// @param psz1 The first string.
    /// @param psz2 The second string.
    /// @return An integer less than, equal to, or greater than zero if psz1 is found, respectively, to be less than, to match, or be greater than psz2.
    static int Compare(const wchar_t* psz1, const wchar_t* psz2);

    /// @brief Compares two strings lexicographically, ignoring case.
    /// @param psz1 The first string.
    /// @param psz2 The second string.
    /// @return An integer less than, equal to, or greater than zero if psz1 is found, respectively, to be less than, to match, or be greater than psz2.
    static int CompareNoCase(const wchar_t* psz1, const wchar_t* psz2);

    /// @brief Finds the first occurrence of a character in a string.
    /// @param psz The string to search.
    /// @param ch The character to find.
    /// @return A pointer to the first occurrence of ch in psz, or NULL if ch is not found.
    static const wchar_t* StrChr(const wchar_t* psz, wchar_t ch);

    /// @brief Finds the last occurrence of a character in a string.
    /// @param psz The string to search.
    /// @param ch The character to find.
    /// @return A pointer to the last occurrence of ch in psz, or NULL if ch is not found.
    static const wchar_t* StrRChr(const wchar_t* psz, wchar_t ch);

    /// @brief Finds the first occurrence of a substring in a string.
    /// @param psz The string to search.
    /// @param psz2 The substring to find.
    /// @return A pointer to the first occurrence of psz2 in psz, or NULL if psz2 is not found.
    static const wchar_t* StrStr(const wchar_t* psz, const wchar_t* psz2);

    /// @brief Converts a string to uppercase.
    /// @param psz The string to convert.
    /// @return A pointer to the converted string.
    static wchar_t* StrUpper(wchar_t* psz);

    /// @brief Converts a string to lowercase.
    /// @param psz The string to convert.
    /// @return A pointer to the converted string.
    static wchar_t* StrLower(wchar_t* psz);

    /// @brief Checks if a character is a whitespace character.
    /// @param ch The character to check.
    /// @return Non-zero if the character is a whitespace character, zero otherwise.
    static int IsSpace(wchar_t ch);

    /// @brief Converts a character to lowercase.
    /// @param ch The character to convert.
    /// @return The lowercase version of the character.
    static wchar_t CharLower(wchar_t ch);

    /// @brief Converts a character to uppercase.
    /// @param ch The character to convert.
    /// @return The uppercase version of the character.
    static wchar_t CharUpper(wchar_t ch);

    /// @brief Moves to the next character in a string.
    /// @param psz The string to move through.
    /// @return A pointer to the next character in the string.
    static wchar_t* CharNext(wchar_t* psz);

    /// @brief Formats a string using a format string and variable arguments.
    /// @param ppszDst The destination string buffer.
    /// @param pszFormat The format string.
    /// @param args The variable arguments list.
    /// @return The number of characters written to the buffer.
    static int Format(wchar_t** ppszDst, const wchar_t* pszFormat, va_list & args);

    /// @brief Loads a string resource from a module.
    /// @param hInst The handle to the module.
    /// @param uID The resource identifier.
    /// @param lpBuffer The buffer to store the loaded string.
    /// @param nBufferMax The maximum size of the buffer.
    /// @return The number of characters copied to the buffer.
    static int LoadString(HINSTANCE hInst, UINT uID, wchar_t* lpBuffer, int nBufferMax);
};

/// @brief A class representing an ASCII string.
class UTILITIES_API SStringW : public IStringW
{
public:
	typedef const wchar_t * pctstr;
	typedef wchar_t _tchar;
	typedef wchar_traits _tchar_traits;

    /// @brief Default constructor.
    SStringW();

    /// @brief Copy constructor.
    /// @param stringSrc The string to copy.
    SStringW(const SStringW& stringSrc);

    /// @brief Constructor from an IStringW object.
    /// @param stringSrc The string to copy.
    SStringW(const IStringW* stringSrc);

    /// @brief Constructor from a single character.
    /// @param ch The character to initialize the string with.
    /// @param nLength The number of times to repeat the character.
    SStringW(wchar_t ch, int nLength = 1);

    /// @brief Constructor from a substring of a character array.
    /// @param psz The character array.
    /// @param nLength The length of the substring.
    SStringW(const wchar_t* psz, int nLength);

    /// @brief Constructor from a character array.
    /// @param psz The character array.
    SStringW(const wchar_t* psz);

    /// @brief Destructor.
    ~SStringW();

public:
    /// @brief Retrieves the length of the string.
    /// @return The length of the string.
    STDMETHOD_(int, GetLength)(THIS) SCONST;

    /// @brief Checks if the string is empty.
    /// @return TRUE if the string is empty, FALSE otherwise.
    STDMETHOD_(BOOL, IsEmpty)(THIS) SCONST;

    /// @brief Empties the string.
    STDMETHOD_(void, Empty)(THIS);

    /// @brief Retrieves the character at a specified index.
    /// @param nIndex The index of the character.
    /// @return The character at the specified index.
    STDMETHOD_(wchar_t, GetAt)(THIS_ int nIndex) SCONST;

    /// @brief Sets the character at a specified index.
    /// @param nIndex The index of the character.
    /// @param ch The new character value.
    STDMETHOD_(void, SetAt)(THIS_ int nIndex, wchar_t ch);

    /// @brief Retrieves a C-style string representation of the string.
    /// @return A pointer to the C-style string.
    STDMETHOD_(const wchar_t *, c_str)(THIS) SCONST;

    /// @brief Compares the string with another string.
    /// @param psz The string to compare with.
    /// @return An integer less than, equal to, or greater than zero if the string is found, respectively, to be less than, to match, or be greater than psz.
    STDMETHOD_(int, Compare)(THIS_ const wchar_t* psz) SCONST;

    /// @brief Compares the string with another string, ignoring case.
    /// @param psz The string to compare with.
    /// @return An integer less than, equal to, or greater than zero if the string is found, respectively, to be less than, to match, or be greater than psz.
    STDMETHOD_(int, CompareNoCase)(THIS_ const wchar_t* psz) SCONST;

    /// @brief Trims leading and trailing whitespace characters from the string.
    STDMETHOD_(void, TrimBlank)(THIS);

    /// @brief Inserts a character at a specified index.
    /// @param nIndex The index at which to insert the character.
    /// @param ch The character to insert.
    /// @return The new length of the string.
    STDMETHOD_(int, InsertChar)(THIS_ int nIndex, wchar_t ch);

    /// @brief Inserts a substring at a specified index.
    /// @param nIndex The index at which to insert the substring.
    /// @param psz The substring to insert.
    /// @return The new length of the string.
    STDMETHOD_(int, Insert)(THIS_ int nIndex, const wchar_t* psz);

    /// @brief Deletes a substring from the string.
    /// @param nIndex The index at which to start deletion.
    /// @param nCount The number of characters to delete.
    /// @return The new length of the string.
    STDMETHOD_(int, Delete)(THIS_ int nIndex, int nCount = 1);

    /// @brief Replaces all occurrences of a character with another character.
    /// @param chOld The character to replace.
    /// @param chNew The character to replace with.
    /// @return The number of characters replaced.
    STDMETHOD_(int, ReplaceChar)(THIS_ wchar_t chOld, wchar_t chNew);

    /// @brief Replaces all occurrences of a substring with another substring.
    /// @param pszOld The substring to replace.
    /// @param pszNew The substring to replace with.
    /// @return The number of substrings replaced.
    STDMETHOD_(int, Replace)(THIS_ const wchar_t* pszOld, const wchar_t* pszNew);

    /// @brief Removes all occurrences of a character from the string.
    /// @param chRemove The character to remove.
    /// @return The number of characters removed.
    STDMETHOD_(int, Remove)(THIS_ wchar_t chRemove);

    /// @brief Finds the first occurrence of a character in the string.
    /// @param ch The character to find.
    /// @param nStart The index at which to start the search.
    /// @return The index of the first occurrence of the character, or -1 if not found.
    STDMETHOD_(int, FindChar)(THIS_ wchar_t ch, int nStart DEF_VAL(0)) SCONST;

    /// @brief Finds the last occurrence of a character in the string.
    /// @param ch The character to find.
    /// @return The index of the last occurrence of the character, or -1 if not found.
    STDMETHOD_(int, ReverseFind)(THIS_ wchar_t ch) SCONST;

    /// @brief Finds the first occurrence of a substring in the string.
    /// @param pszSub The substring to find.
    /// @param nStart The index at which to start the search.
    /// @return The index of the first occurrence of the substring, or -1 if not found.
    STDMETHOD_(int, Find)(THIS_ const wchar_t* pszSub, int nStart DEF_VAL(0)) SCONST;

    /// @brief Retrieves a modifiable buffer for the string.
    /// @param nMinBufLength The minimum buffer length.
    /// @return A pointer to the buffer.
    STDMETHOD_(wchar_t*, GetBuffer)(THIS_ int nMinBufLength DEF_VAL(-1));

    /// @brief Releases the buffer and sets the new length of the string.
    /// @param nNewLength The new length of the string.
    STDMETHOD_(void, ReleaseBuffer)(THIS_ int nNewLength DEF_VAL(-1));

    /// @brief Retrieves a modifiable buffer for the string and sets the new length.
    /// @param nNewLength The new length of the string.
    /// @return A pointer to the buffer.
    STDMETHOD_(wchar_t*, GetBufferSetLength)(THIS_ int nNewLength);

    /// @brief Sets the length of the string.
    /// @param nLength The new length of the string.
    STDMETHOD_(void, SetLength)(THIS_ int nLength);

    /// @brief Copies the contents of another string into this string.
    /// @param src The string to copy from.
    STDMETHOD_(void, Copy)(THIS_ const IStringW* src);

    /// @brief Assigns a character array to the string.
    /// @param src The character array to assign.
    STDMETHOD_(void, Assign)(THIS_ LPCWSTR src);

    /// @brief Assigns a substring of a character array to the string.
    /// @param src The character array to assign.
    /// @param nLen The length of the substring.
    STDMETHOD_(void, Assign2)(THIS_ LPCWSTR src, int nLen);

    /// @brief Retrieves private data associated with the string.
    /// @return A pointer to the private data.
    STDMETHOD_(LPVOID, GetPrivData)(THIS) SCONST;

    /// @brief Converts the string to uppercase.
    STDMETHOD_(void, ToUpper)(THIS) OVERRIDE;

    /// @brief Converts the string to lowercase.
    STDMETHOD_(void, ToLower)(THIS) OVERRIDE;

    /// @brief Trims trailing whitespace characters from the string.
    /// @param chTarget The character to trim.
    STDMETHOD_(void, TrimRight)(THIS_ wchar_t chTarget DEF_VAL(VK_SPACE)) OVERRIDE;

    /// @brief Trims leading whitespace characters from the string.
    /// @param chTarget The character to trim.
    STDMETHOD_(void, TrimLeft)(THIS_ wchar_t chTarget DEF_VAL(VK_SPACE)) OVERRIDE;

    /// @brief Trims leading and trailing whitespace characters from the string.
    /// @param chTarget The character to trim.
    STDMETHOD_(void, Trim)(THIS_ wchar_t chTarget DEF_VAL(VK_SPACE)) OVERRIDE;

    /// @brief Appends a character to the string.
    /// @param ch The character to append.
    STDMETHOD_(void, AppendChar)(THIS_ wchar_t ch) OVERRIDE;

    /// @brief Appends a substring to the string.
    /// @param pszStr The substring to append.
    /// @param nLen The length of the substring.
    STDMETHOD_(void, AppendStr)(THIS_ const wchar_t* pszStr, int nLen DEF_VAL(-1)) OVERRIDE;

    /// @brief Releases the string and its resources.
    STDMETHOD_(void, Release)(THIS) OVERRIDE;

    /// @brief Converts the string to an unsigned integer.
    /// @return The unsigned integer value.
    STDMETHOD_(UINT, ToUint)(CTHIS) SCONST OVERRIDE;

    /// @brief Converts the string to a long integer.
    /// @return The long integer value.
    STDMETHOD_(long, ToLong)(CTHIS) SCONST OVERRIDE;

    /// @brief Converts the string to an integer.
    /// @return The integer value.
    STDMETHOD_(int, ToInt)(CTHIS) SCONST OVERRIDE;

    /// @brief Converts the string to a float.
    /// @return The float value.
    STDMETHOD_(float, ToFloat)(CTHIS) SCONST OVERRIDE;

    /// @brief Converts the string to a double.
    /// @return The double value.
    STDMETHOD_(double, ToDouble)(CTHIS) SCONST OVERRIDE;

    /// @brief Converts the string to a boolean.
    /// @return The boolean value.
    STDMETHOD_(BOOL, ToBool)(CTHIS) SCONST OVERRIDE;

    /// @brief Extracts a substring from the string.
    /// @param nFirst The starting index of the substring.
    /// @return The extracted substring.
    SStringW Mid(int nFirst) const;

    /// @brief Extracts a substring from the string.
    /// @param nFirst The starting index of the substring.
    /// @param nCount The length of the substring.
    /// @return The extracted substring.
    SStringW Mid(int nFirst, int nCount) const;

    /// @brief Extracts the rightmost part of the string.
    /// @param nCount The length of the substring.
    /// @return The extracted substring.
    SStringW Right(int nCount) const;

    /// @brief Extracts the leftmost part of the string.
    /// @param nCount The length of the substring.
    /// @return The extracted substring.
    SStringW Left(int nCount) const;

    /// @brief Converts the string to uppercase.
    /// @return A reference to the modified string.
    SStringW& MakeUpper();

    /// @brief Converts the string to lowercase.
    /// @return A reference to the modified string.
    SStringW& MakeLower();

    /// @brief Checks if the string starts with a specified prefix.
    /// @param prefix The prefix to check.
    /// @param IgnoreCase Whether the comparison should be case-insensitive.
    /// @return TRUE if the string starts with the prefix, FALSE otherwise.
    bool StartsWith(const SStringW& prefix, bool IgnoreCase = false) const;

    /// @brief Checks if the string ends with a specified suffix.
    /// @param suffix The suffix to check.
    /// @param IgnoreCase Whether the comparison should be case-insensitive.
    /// @return TRUE if the string ends with the suffix, FALSE otherwise.
    bool EndsWith(const SStringW& suffix, bool IgnoreCase = false) const;

    /// @brief Loads a string resource from a module.
    /// @param nID The resource identifier.
    /// @param hInst The handle to the module.
    /// @return TRUE if the string was successfully loaded, FALSE otherwise.
    BOOL LoadString(UINT nID, HINSTANCE hInst);

    /// @brief Formats a string using a format string and variable arguments.
    /// @param hInst The handle to the module.
    /// @param nFormatID The resource identifier of the format string.
    /// @param ... The variable arguments list.
    /// @return TRUE if the string was successfully formatted, FALSE otherwise.
    BOOL __cdecl Format(HINSTANCE hInst, UINT nFormatID, ...);

    /// @brief Appends formatted data to the string using a format string and variable arguments.
    /// @param hInst The handle to the module.
    /// @param nFormatID The resource identifier of the format string.
    /// @param ... The variable arguments list.
    void __cdecl AppendFormat(HINSTANCE hInst, UINT nFormatID, ...);

    /// @brief Formats a string using a format string and variable arguments.
    /// @param pszFormat The format string.
    /// @param ... The variable arguments list.
    /// @return The formatted string.
    SStringW __cdecl Format(const wchar_t* pszFormat, ...);

    /// @brief Appends formatted data to the string using a format string and variable arguments.
    /// @param pszFormat The format string.
    /// @param ... The variable arguments list.
    /// @return The modified string.
    SStringW __cdecl AppendFormat(const wchar_t* pszFormat, ...);

    /// @brief Converts the string to a C-style string.
    /// @return A pointer to the C-style string.
    operator const wchar_t*() const;

    /// @brief Retrieves the character at a specified index.
    /// @param nIndex The index of the character.
    /// @return The character at the specified index.
    wchar_t operator[](int nIndex) const;

    /// @brief Overloaded assignment operator from another SStringW object.
    /// @param stringSrc The string to assign.
    /// @return A reference to the modified string.
    SStringW& operator=(const SStringW& stringSrc);

    /// @brief Overloaded assignment operator from a character array.
    /// @param psz The character array to assign.
    /// @return A reference to the modified string.
    SStringW& operator=(const wchar_t* psz);

    /// @brief Overloaded assignment operator from a single character.
    /// @param ch The character to assign.
    /// @return A reference to the modified string.
    const SStringW& operator=(wchar_t ch);

    /// @brief Overloaded concatenation operator with a character array.
    /// @param psz The character array to concatenate.
    /// @return A reference to the modified string.
    const SStringW& operator+=(const wchar_t* psz);

    /// @brief Overloaded concatenation operator with a single character.
    /// @param ch The character to concatenate.
    /// @return A reference to the modified string.
    const SStringW& operator+=(wchar_t ch);

    /// @brief Overloaded concatenation operator with another SStringW object.
    /// @param src The string to concatenate.
    /// @return A reference to the modified string.
    const SStringW& operator+=(const SStringW& src);

    /// @brief Appends another SStringW object to the string.
    /// @param src The string to append.
    /// @return A reference to the modified string.
    SStringW& Append(const SStringW& src);

    /// @brief Checks if a character is a blank character.
    /// @param c The character to check.
    /// @return TRUE if the character is a blank character, FALSE otherwise.
    static bool IsBlankChar(const wchar_t &c);

    /// @brief Overloaded equality operator between two SStringW objects.
    /// @param s1 The first string.
    /// @param s2 The second string.
    /// @return TRUE if the strings are equal, FALSE otherwise.
    friend inline bool __stdcall operator==(const SStringW& s1, const SStringW& s2)
    {
        return s1.Compare(s2) == 0;
    }

    /// @brief Overloaded equality operator between an SStringW object and a character array.
    /// @param s1 The string.
    /// @param s2 The character array.
    /// @return TRUE if the string and character array are equal, FALSE otherwise.
    friend inline bool __stdcall operator==(const SStringW& s1, const wchar_t* s2)
    {
        return s1.Compare(s2) == 0;
    }

    /// @brief Overloaded equality operator between a character array and an SStringW object.
    /// @param s1 The character array.
    /// @param s2 The string.
    /// @return TRUE if the character array and string are equal, FALSE otherwise.
    friend inline bool __stdcall operator==(const wchar_t* s1, const SStringW& s2)
    {
        return s2.Compare(s1) == 0;
    }

    /// @brief Overloaded inequality operator between two SStringW objects.
    /// @param s1 The first string.
    /// @param s2 The second string.
    /// @return TRUE if the strings are not equal, FALSE otherwise.
    friend inline bool __stdcall operator!=(const SStringW& s1, const SStringW& s2)
    {
        return s1.Compare(s2) != 0;
    }

    /// @brief Overloaded inequality operator between an SStringW object and a character array.
    /// @param s1 The string.
    /// @param s2 The character array.
    /// @return TRUE if the string and character array are not equal, FALSE otherwise.
    friend inline bool __stdcall operator!=(const SStringW& s1, const wchar_t* s2)
    {
        return s1.Compare(s2) != 0;
    }

    /// @brief Overloaded inequality operator between a character array and an SStringW object.
    /// @param s1 The character array.
    /// @param s2 The string.
    /// @return TRUE if the character array and string are not equal, FALSE otherwise.
    friend inline bool __stdcall operator!=(const wchar_t* s1, const SStringW& s2)
    {
        return s2.Compare(s1) != 0;
    }

    /// @brief Overloaded less-than operator between two SStringW objects.
    /// @param s1 The first string.
    /// @param s2 The second string.
    /// @return TRUE if the first string is less than the second string, FALSE otherwise.
    friend inline bool __stdcall operator<(const SStringW& s1, const SStringW& s2)
    {
        return s1.Compare(s2) < 0;
    }

    /// @brief Overloaded less-than operator between an SStringW object and a character array.
    /// @param s1 The string.
    /// @param s2 The character array.
    /// @return TRUE if the string is less than the character array, FALSE otherwise.
    friend inline bool __stdcall operator<(const SStringW& s1, const wchar_t* s2)
    {
        return s1.Compare(s2) < 0;
    }

    /// @brief Overloaded less-than operator between a character array and an SStringW object.
    /// @param s1 The character array.
    /// @param s2 The string.
    /// @return TRUE if the character array is less than the string, FALSE otherwise.
    friend inline bool __stdcall operator<(const wchar_t* s1, const SStringW& s2)
    {
        return s2.Compare(s1) > 0;
    }

    /// @brief Overloaded greater-than operator between two SStringW objects.
    /// @param s1 The first string.
    /// @param s2 The second string.
    /// @return TRUE if the first string is greater than the second string, FALSE otherwise.
    friend inline bool __stdcall operator>(const SStringW& s1, const SStringW& s2)
    {
        return s1.Compare(s2) > 0;
    }

    /// @brief Overloaded greater-than operator between an SStringW object and a character array.
    /// @param s1 The string.
    /// @param s2 The character array.
    /// @return TRUE if the string is greater than the character array, FALSE otherwise.
    friend inline bool __stdcall operator>(const SStringW& s1, const wchar_t* s2)
    {
        return s1.Compare(s2) > 0;
    }

    /// @brief Overloaded greater-than operator between a character array and an SStringW object.
    /// @param s1 The character array.
    /// @param s2 The string.
    /// @return TRUE if the character array is greater than the string, FALSE otherwise.
    friend inline bool __stdcall operator>(const wchar_t* s1, const SStringW& s2)
    {
        return s2.Compare(s1) < 0;
    }

    /// @brief Overloaded less-than-or-equal-to operator between two SStringW objects.
    /// @param s1 The first string.
    /// @param s2 The second string.
    /// @return TRUE if the first string is less than or equal to the second string, FALSE otherwise.
    friend inline bool __stdcall operator<=(const SStringW& s1, const SStringW& s2)
    {
        return s1.Compare(s2) <= 0;
    }

    /// @brief Overloaded less-than-or-equal-to operator between an SStringW object and a character array.
    /// @param s1 The string.
    /// @param s2 The character array.
    /// @return TRUE if the string is less than or equal to the character array, FALSE otherwise.
    friend inline bool __stdcall operator<=(const SStringW& s1, const wchar_t* s2)
    {
        return s1.Compare(s2) <= 0;
    }

    /// @brief Overloaded less-than-or-equal-to operator between a character array and an SStringW object.
    /// @param s1 The character array.
    /// @param s2 The string.
    /// @return TRUE if the character array is less than or equal to the string, FALSE otherwise.
    friend inline bool __stdcall operator<=(const wchar_t* s1, const SStringW& s2)
    {
        return s2.Compare(s1) >= 0;
    }

    /// @brief Overloaded greater-than-or-equal-to operator between two SStringW objects.
    /// @param s1 The first string.
    /// @param s2 The second string.
    /// @return TRUE if the first string is greater than or equal to the second string, FALSE otherwise.
    friend inline bool __stdcall operator>=(const SStringW& s1, const SStringW& s2)
    {
        return s1.Compare(s2) >= 0;
    }

    /// @brief Overloaded greater-than-or-equal-to operator between an SStringW object and a character array.
    /// @param s1 The string.
    /// @param s2 The character array.
    /// @return TRUE if the string is greater than or equal to the character array, FALSE otherwise.
    friend inline bool __stdcall operator>=(const SStringW& s1, const wchar_t* s2)
    {
        return s1.Compare(s2) >= 0;
    }

    /// @brief Overloaded greater-than-or-equal-to operator between a character array and an SStringW object.
    /// @param s1 The character array.
    /// @param s2 The string.
    /// @return TRUE if the character array is greater than or equal to the string, FALSE otherwise.
    friend inline bool __stdcall operator>=(const wchar_t* s1, const SStringW& s2)
    {
        return s2.Compare(s1) <= 0;
    }

    /// @brief Overloaded addition operator between two SStringW objects.
    /// @param string1 The first string.
    /// @param string2 The second string.
    /// @return A new string that is the concatenation of the two strings.
    friend inline SStringW __stdcall operator+(const SStringW& string1, const SStringW& string2)
    {
        SStringW s;
        s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pszData, string2.GetData()->nDataLength, string2.m_pszData);
        return s;
    }

    /// @brief Overloaded addition operator between an SStringW object and a character array.
    /// @param string The string.
    /// @param psz The character array.
    /// @return A new string that is the concatenation of the string and the character array.
    friend inline SStringW __stdcall operator+(const SStringW& string, const wchar_t* psz)
    {
        SASSERT(psz != NULL);
        SStringW s;
        s.ConcatCopy(string.GetData()->nDataLength, string.m_pszData, SStringW::SafeStrlen(psz), psz);
        return s;
    }

    /// @brief Overloaded addition operator between a character array and an SStringW object.
    /// @param psz The character array.
    /// @param string The string.
    /// @return A new string that is the concatenation of the character array and the string.
    friend inline SStringW __stdcall operator+(const wchar_t* psz, const SStringW& string)
    {
        SASSERT(psz != NULL);
        SStringW s;
        s.ConcatCopy(SStringW::SafeStrlen(psz), psz, string.GetData()->nDataLength, string.m_pszData);
        return s;
    }

    /// @brief Overloaded addition operator between an SStringW object and a single character.
    /// @param string1 The string.
    /// @param ch The character.
    /// @return A new string that is the concatenation of the string and the character.
    friend inline SStringW __stdcall operator+(const SStringW& string1, wchar_t ch)
    {
        SStringW s;
        s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pszData, 1, &ch);
        return s;
    }

    /// @brief Overloaded addition operator between a single character and an SStringW object.
    /// @param ch The character.
    /// @param string The string.
    /// @return A new string that is the concatenation of the character and the string.
    friend inline SStringW __stdcall operator+(wchar_t ch, const SStringW& string)
    {
        SStringW s;
        s.ConcatCopy(1, &ch, string.GetData()->nDataLength, string.m_pszData);
        return s;
    }

    // Implementation
protected:
    /// @brief Retrieves the allocated length of the string buffer.
    /// @return The allocated length of the string buffer.
    int GetAllocLength() const;

    /// @brief Computes the length of a null-terminated string safely.
    /// @param psz The string to measure.
    /// @return The length of the string.
    static int SafeStrlen(const wchar_t* psz);

    /// @brief Preallocates memory for the string buffer.
    /// @param nLength The length to preallocate.
    void Preallocate(int nLength);

    /// @brief Frees any extra allocated memory in the string buffer.
    void FreeExtra();

protected:
    // implementation helpers
    /// @brief Retrieves the data structure of the string.
    /// @return A pointer to the data structure.
    TStringData* GetData() const;

    /// @brief Initializes the string.
    void Init();

    /// @brief Initializes the string from an IStringW object.
    /// @param stringSrc The IStringW object to initialize from.
    void InitFromIString(const IStringW *stringSrc);

    /// @brief Formats a string using a format string and variable arguments.
    /// @param pszFormat The format string.
    /// @param args The variable arguments list.
    /// @return TRUE if the string was successfully formatted, FALSE otherwise.
    BOOL _Format(const wchar_t* pszFormat, va_list & args);

    /// @brief Appends formatted data to the string using a format string and variable arguments.
    /// @param pszFormat The format string.
    /// @param args The variable arguments list.
    void _AppendFormat(const wchar_t* pszFormat, va_list & args);

    // Assignment operators
    //  All assign a new value to the string
    //      (a) first see if the buffer is big enough
    //      (b) if enough room, copy on top of old buffer, set size and type
    //      (c) otherwise free old string data, and create a new one
    //
    //  All routines return the new string (but as a 'const TStringT&' so that
    //      assigning it again will cause a copy, eg: s1 = s2 = "hi there".
    //

    /// @brief Allocates and copies a substring of the string.
    /// @param dest The destination string.
    /// @param nCopyLen The length of the substring to copy.
    /// @param nCopyIndex The starting index of the substring.
    /// @param nExtraLen The extra length to allocate.
    void AllocCopy(SStringW& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;

    /// @brief Assigns a substring of a character array to the string.
    /// @param nSrcLen The length of the substring.
    /// @param pszSrcData The character array to assign from.
    void AssignCopy(int nSrcLen, const wchar_t* pszSrcData);

    /// @brief Concatenates two substrings and copies the result to the string.
    /// @param nSrc1Len The length of the first substring.
    /// @param pszSrc1Data The first substring.
    /// @param nSrc2Len The length of the second substring.
    /// @param pszSrc2Data The second substring.
    /// @return TRUE if the concatenation was successful, FALSE otherwise.
    bool ConcatCopy(int nSrc1Len, const wchar_t* pszSrc1Data, int nSrc2Len, const wchar_t* pszSrc2Data);

    /// @brief Concatenates a substring to the string in place.
    /// @param nSrcLen The length of the substring.
    /// @param pszSrcData The substring to concatenate.
    void ConcatInPlace(int nSrcLen, const wchar_t* pszSrcData);

    /// @brief Copies the string before writing to it.
    void CopyBeforeWrite();

    /// @brief Allocates memory for the string before writing to it.
    /// @param nLen The length to allocate.
    /// @return TRUE if the allocation was successful, FALSE otherwise.
    bool AllocBeforeWrite(int nLen);

    /// @brief Allocates memory for the string buffer.
    /// @param nLength The length to allocate.
    /// @return TRUE if the allocation was successful, FALSE otherwise.
    bool AllocBuffer(int nLength);

    /// @brief Reallocates memory for the string buffer.
    /// @param nNewLength The new length to allocate.
    /// @return TRUE if the reallocation was successful, FALSE otherwise.
    bool ReallocBuffer(int nNewLength);

    /// @brief Releases the data structure of the string.
    void _ReleaseData();

    // always allocate one extra character for '\0' termination
    // assumes [optimistically] that data length will equal allocation length

    /// @brief Allocates a new data structure for the string.
    /// @param nLength The length to allocate.
    /// @param pOldData The old data structure to reuse (optional).
    /// @return A pointer to the new data structure.
    static TStringData* AllocData(int nLength, TStringData* pOldData = NULL);

    /// @brief Releases a data structure.
    /// @param pData The data structure to release.
    static void ReleaseData(TStringData* pData);

protected:
    /// @brief Pointer to the ref counted string data.
    wchar_t* m_pszData;
};

SNSEND
#endif//__TSTRINGW_H__