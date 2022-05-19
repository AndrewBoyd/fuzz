#pragma once
#include "fuzz_types.h"
#include "fuzz_grammar.h"


#include <utility/string_utils.h>
#include <fmt/format.h>
#include <iostream>
#include <fstream>
#include <type_traits>
#include <Windows.h>
#include <utility/reflectaga.h>
//
//template<typename parse_t = fuzz_grammar::expression>
//void test(std::u8string input_string)
//{
//	std::cout << "testing: \"" << reinterpret_cast<char const*>(input_string.c_str()) << "\".... " << std::endl;
//
//	auto input = lexy::string_input<lexy::utf8_encoding>(input_string);
//	auto result = lexy::parse<parse_t>(input, lexy_ext::report_error);
//
//	std::cout << fuzz::to_string(result.value()) << std::endl;
//}
//
//template<typename parse_t = fuzz_grammar::expression>
//void test(std::string input_string)
//{
//	auto as_u8 = string_utils::string_to_u8string<parse_t>(std::move(input_string));
//	test(as_u8);
//}
//

void testFuzzOld() {
	//namespace fz = fuzz;
	//namespace fg = fuzz_grammar;
	//test(u8"true");
	//test(u8"false");
	//test(u8"3.141592654");
	//test(u8"-3");
	//test(u8"\"Hello, World\"");
	//test(u8"\"Greek: αβγδ; German: Übergrößenträger\"");
	//test(u8"[3, 2, \"Hi Kat :)\", false]");
	//test(u8"[1, 2, [3, 4, 5]]");
	//test(u8"+");
	//test(u8">=");
	//test(u8">");
	//test(u8"6*3.9");
	//test(u8"[2,4,8]+[6,9,2]");
}

void testFuzzNew() {

}

int main() 
{
	::SetConsoleOutputCP(CP_UTF8);

	return 0;
}
