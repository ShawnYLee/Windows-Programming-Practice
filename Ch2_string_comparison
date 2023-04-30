
#include <iostream>
#include <string>
#include <locale>
#ifdef UNICODE
#define TCHAR WCHAR
#else
#define TCHAR CHAR
#endif
using namespace std;

int main()
{
    std::string str1 = "coop";
    std::string str2 = "co-op";

    // Perform a string comparison
    int result1 = strcmp(str1.c_str(),str2.c_str());
    std::cout << "String comparison result: " << result1 << std::endl;

    // Perform a word comparison with the English locale
    _locale_t locale = _create_locale(LC_ALL, "en-US");
    int result2 = _stricmp_l(str1.c_str(), str2.c_str(), locale);
    std::cout << "String comparison result: " << result2 << std::endl;

}


