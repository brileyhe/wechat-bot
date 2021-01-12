#pragma once
#include<string>
#include <locale>
#include <codecvt>

namespace arris {
namespace util {
//namespace str{
    /*std::wstring __stdcall string_to_wstring(const std::string& s)
    {
        using default_convert = std::codecvt<wchar_t, char, std::mbstate_t>;
        static std::wstring_convert<default_convert>conv(new default_convert("CHS"));
        return conv.from_bytes(s);
    }
    std::string __stdcall wstring_to_string(const std::wstring& s)
    {
        using default_convert = std::codecvt<wchar_t, char, std::mbstate_t>;
        static std::wstring_convert<default_convert>conv(new default_convert("CHS"));
        return conv.to_bytes(s);
    }
    std::string __stdcall ansi_to_utf8(const std::string& s)
    {
        static std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
        return conv.to_bytes(string_to_wstring(s));
    }
    std::string __stdcall utf8_to_ansi(const std::string& s)
    {
        static std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
        return wstring_to_string(conv.from_bytes(s));
    }*/
	wchar_t* wstring_to_wchar(const std::wstring& ws)
	{
		return const_cast<wchar_t*>(ws.c_str());
	}
	std::string ucs2_to_utf8(wchar_t* pucs2)
	{
		//预转换，得到所需空间的大小，这次用的函数和上面名字相反
		int len = ::WideCharToMultiByte(CP_UTF8, NULL, pucs2, wcslen(pucs2), NULL, 0, NULL, NULL);
		//同上，分配空间要给'\0'留个空间
		//UTF8虽然是Unicode的压缩形式，但也是多字节字符串，所以可以以char的形式保存
		char* p_utf8 = new char[len + 1];
		//转换
		//unicode版对应的strlen是wcslen
		::WideCharToMultiByte(CP_UTF8, NULL, pucs2, wcslen(pucs2), p_utf8, len, NULL, NULL);
		//最后加上'\0'
		p_utf8[len] = '\0';
		std::string buf(p_utf8);
		if (NULL != p_utf8)
		{
			p_utf8 = NULL;
			delete[] p_utf8;
		}
		return buf;
	}
	std::wstring __stdcall string_to_wstring(const std::string& input)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(input);
	}
	std::string __stdcall wstring_to_string(const std::wstring& input)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(input);
	}
	std::wstring __stdcall inttowstring(int para)
	{
		std::wstringstream ss;
		ss << para;
		std::wstring r = ss.str();
		return r;
	}
	std::string __stdcall inttostring(int para)
	{
		std::stringstream ss;
		ss << para;
		std::string r = ss.str();
		return r;
	}
	std::string __stdcall time2id()
	{
		struct tm t;
		time_t now;  //声明time_t类型变量
		time(&now);      //获取系统日期和时间
		localtime_s(&t, &now);   //获取当地日期和时间

		std::string year = inttostring(t.tm_year + 1900);
		std::string month;
		if ((t.tm_mon + 1) < 10)
		{
			month = "0" + inttostring(t.tm_mon + 1);
		}
		else
		{
			month = inttostring(t.tm_mon + 1);
		}

		std::string day;
		if (t.tm_mday < 10)
		{
			day = "0" + inttostring(t.tm_mday);
		}
		else
		{
			day = inttostring(t.tm_mday);
		}



		std::string hour;
		if (t.tm_hour < 10)
		{
			hour = "0" + inttostring(t.tm_hour);
		}
		else {
			hour = inttostring(t.tm_hour);

		}
		std::string min;
		if (t.tm_min < 10)
		{
			min = "0" + inttostring(t.tm_min);
		}
		else {
			min = inttostring(t.tm_min);
		}


		std::string sec;
		if (t.tm_sec < 10)
		{
			sec = "0" + inttostring(t.tm_sec);

		}
		else {
			sec = inttostring(t.tm_sec);
		}


		std::string format = year + month + day + hour + min + sec;
		return format;
	}
	std::wstring __stdcall ws_wxtime()
	{
		struct tm t;
		time_t now;  //声明time_t类型变量
		time(&now);      //获取系统日期和时间
		localtime_s(&t, &now);   //获取当地日期和时间

		std::wstring year = inttowstring(t.tm_year + 1900);

		std::wstring month;
		if (t.tm_mon + 1 < 10)
		{
			month = TEXT("0") + inttowstring(t.tm_mon + 1);
		}
		else
		{
			month = inttowstring(t.tm_mon + 1);
		}

		std::wstring day;
		if (t.tm_mday < 10)
		{
			day = TEXT("0") + inttowstring(t.tm_mday);
		}
		else
		{
			day = inttowstring(t.tm_mday);
		}


		std::wstring hour;
		if (t.tm_hour < 10)
		{
			hour = TEXT("0") + inttowstring(t.tm_hour);

		}
		else {
			hour = inttowstring(t.tm_hour);
		}

		std::wstring min;
		if (t.tm_min < 10)
		{
			min = TEXT("0") + inttowstring(t.tm_min);

		}
		else
		{
			min = inttowstring(t.tm_min);
		}


		std::wstring sec;
		if (t.tm_sec < 10)
		{
			sec = TEXT("0") + inttowstring(t.tm_sec);

		}
		else
		{
			sec = inttowstring(t.tm_sec);

		}

		std::wstring format = year + TEXT("-") + month + TEXT("-") + day + TEXT(" ") + hour + TEXT(":") + min + TEXT(":") + sec;
		return format;
	}

	std::string __stdcall s_wxtime()
	{
		struct tm t;
		time_t now;  //声明time_t类型变量
		time(&now);      //获取系统日期和时间
		localtime_s(&t, &now);   //获取当地日期和时间

		std::string year = inttostring(t.tm_year + 1900);
		std::string month;
		if ((t.tm_mon + 1) < 10)
		{
			month = "0" + inttostring(t.tm_mon + 1);
		}
		else
		{
			month = inttostring(t.tm_mon + 1);
		}

		std::string day;
		if (t.tm_mday < 10)
		{
			day = "0" + inttostring(t.tm_mday);
		}
		else
		{
			day = inttostring(t.tm_mday);
		}



		std::string hour;
		if (t.tm_hour < 10)
		{
			hour = "0" + inttostring(t.tm_hour);
		}
		else {
			hour = inttostring(t.tm_hour);

		}
		std::string min;
		if (t.tm_min < 10)
		{
			min = "0" + inttostring(t.tm_min);
		}
		else {
			min = inttostring(t.tm_min);
		}


		std::string sec;
		if (t.tm_sec < 10)
		{
			sec = "0" + inttostring(t.tm_sec);

		}
		else {
			sec = inttostring(t.tm_sec);
		}


		std::string format = year + "-" + month + ("-") + day + (" ") + hour + (":") + min + (":") + sec;
		return format;
	}
    
//}//namespace str
}//namespace util
}//namespace arris