#pragma once

#include <string>
#include <string_view>

namespace i18n
{
	std::string tr(std::string_view text);
	std::wstring trw(std::wstring_view text);
}
