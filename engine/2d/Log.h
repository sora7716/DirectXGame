#pragma once
#include <string>

/// <summary>
/// ログ
/// </summary>
class Log{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Log() = default;
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Log() = default;

	/// <summary>
	/// コンソールプリント(ロガー)
	/// </summary>
	/// <param name="message">メッセージ</param>
	void ConsolePrintf(const std::string& message);

	/// <summary>
	/// stringをwstringに変換
	/// </summary>
	/// <param name="str">string</param>
	/// <returns>wstring</returns>
	std::wstring ConvertString(const std::string& str);

	/// <summary>
	/// wstringをstringに変換
	/// </summary>
	/// <param name="str">wstirng</param>
	/// <returns>string</returns>
	std::string ConvertString(const std::wstring& str);
};

