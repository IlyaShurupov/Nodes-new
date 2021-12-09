#pragma once

#include "Types.h"

template <typename TYPE>
class const_buff {

public:
	const TYPE* str;

	const_buff() {
		str = nullptr;
	}

	bool operator==(const const_buff& in) {
		alni len = cstrlen(in.str);
		if (len != cstrlen(str)) {
			return false;
		}

		for (alni i = 0; i < len; i++) {
			if (str[i] != in.str[i]) {
				return false;
			}
		}

		return true;
	}

	const_buff(const TYPE* in) {
		str = in;
	}
};

using constring = const_buff<char>;
using const_wtring = const_buff<wchar_t>;
