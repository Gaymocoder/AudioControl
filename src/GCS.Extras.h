#ifndef __GCS_EXTRAS_H__
#define __GCS_EXTRAS_H__

#include <iostream>

void setNormalLocale();
std::wstring strToWstr(const std::string &srcString);
std::string wstrToStr(const std::wstring &srcWstring);

#endif