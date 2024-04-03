#include "GCS.Extras.h"

#include <codecvt>
#include <locale>

#ifdef _WIN32
    #define WIN true
#else
    #define WIN false
#endif

void setNormalLocale()
{
    if (WIN == 1)
        std::system("chcp 65001");
}

std::wstring strToWstr(const std::string &srcString)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(srcString);
}

std::string wstrToStr(const std::wstring &srcWstring)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(srcWstring);
}