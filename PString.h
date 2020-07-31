#pragma once

// wrapper for IAIMPString
class PString
{
	ComPtr<IAIMPString> m_str;
public:
	PString() { if (g_core) g_core->CreateObject(IID_IAIMPString, (void**)&m_str); }
	PString(const std::wstring& str) {
		new (this)PString();
		m_str->SetData(const_cast<WCHAR*>(str.c_str()), str.size());
	}
	PString(const WCHAR* str) {
		new (this)PString(std::wstring(str));
	}
	// todo: json lib value support

	auto GetStr() { return m_str; }

	template<class FromInt>
	static std::wstring Num2Str(FromInt num) {
		static std::wstringstream ss;
		ss.clear();
		ss << num;
		std::wstring res;
		ss >> res;
		return res;
	}

	template<class ToInt>
	static ToInt Str2Num(std::wstring str) {
		static std::wstringstream ss;
		ss.clear();
		ss << str;
		ToInt res;
		ss >> res;
		return res;
	}

	static BYTE ToHex(BYTE x)
	{
		return x > 9 ? x + 55 : x + 48;
	}

	static BYTE FromHex(BYTE x)
	{
		BYTE y;
		if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
		else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
		else if (x >= '0' && x <= '9') y = x - '0';
		else assert(("Invalid char argument!", false));
		return y;
	}

	static std::string UriEncode(const std::string& str)
	{
		std::string strTemp = "";
		size_t length = str.length();
		for (size_t i = 0; i < length; i++)
		{
			if (isalnum((BYTE)str[i]) ||
				(str[i] == '-') ||
				(str[i] == '_') ||
				(str[i] == '.') ||
				(str[i] == '~'))
				strTemp += str[i];
			else if (str[i] == ' ')
				strTemp += "+";
			else
			{
				strTemp += '%';
				strTemp += ToHex((BYTE)str[i] >> 4);
				strTemp += ToHex((BYTE)str[i] % 16);
			}
		}
		return strTemp;
	}

	static std::string UriDecode(const std::string& str)
	{
		std::string strTemp = "";
		size_t length = str.length();
		for (size_t i = 0; i < length; i++)
		{
			if (str[i] == '+') strTemp += ' ';
			else if (str[i] == '%')
			{
				assert(i + 2 < length);
				BYTE high = FromHex((BYTE)str[++i]);
				BYTE low = FromHex((BYTE)str[++i]);
				strTemp += high * 16 + low;
			}
			else strTemp += str[i];
		}
		return strTemp;
	}

	// UTF-16 to UTF-8
	static std::string W2U(const std::wstring& str)
	{
		char* pBuffer;
		int iTextLen;
		iTextLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
		pBuffer = new char[iTextLen + 1];
		memset((void*)pBuffer, 0, sizeof(char) * (iTextLen + 1));
		WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pBuffer, iTextLen, NULL, NULL);
		std::string res;
		res = pBuffer;
		delete[] pBuffer;

		return res;
	}

	// UTF-8 to UTF-16
	static std::wstring U2W(const std::string& str) {
		int len = 0;
		len = str.length();
		int rLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pBuffer;
		pBuffer = new wchar_t[rLen + 1];
		memset(pBuffer, 0, (rLen + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)pBuffer, rLen);
		std::wstring res;
		res = pBuffer;
		delete pBuffer;

		return res;
	}

	static auto UriEncodeW(std::wstring str) {
		return U2W(UriEncode(W2U(str)));
	}

	static auto UriDecodeW(std::wstring str) {
		return U2W(UriDecode(W2U(str)));
	}
};
