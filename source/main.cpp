#pragma once
#include "fuzz_types.h"
#include "fuzz_grammar.h"
#include "fuzz_strings.h"

#include <utility/string_utils.h>
#include <fmt/format.h>
#include <iostream>
#include <fstream>
#include <Windows.h>


template<typename parse_t = fuzz_grammar::Expression>
void test(std::u8string input_string)
{
	std::cout << "testing: \"" << reinterpret_cast<char const*>(input_string.c_str()) << "\".... " << std::endl;

	auto input = lexy::string_input<lexy::utf8_encoding>(input_string);
	auto parse_result = lexy::parse<parse_t>(input, lexy_ext::report_error);
	auto result = parse_result.value();

	std::cout << fuzz::to_string(result) << std::endl;
	std::cout << std::endl;
}

template<typename parse_t = fuzz_grammar::Expression>
void test(std::string input_string)
{
	auto as_u8 = string_utils::ascii_to_u8<parse_t>(std::move(input_string));
	test(as_u8);
}


void testFuzz() {
	namespace fz = fuzz;
	namespace fg = fuzz_grammar;
	test<fuzz_grammar::Expression>(u8"true");
	test<fuzz_grammar::Expression>(u8"false");
	test<fuzz_grammar::Expression>(u8"3.141592654");
	test<fuzz_grammar::Expression>(u8"-3");
	test<fuzz_grammar::Expression>(u8"\"Hello, World\"");
	test<fuzz_grammar::Expression>(u8"\"Greek: αβγδ; German: Übergrößenträger\"");
	test<fuzz_grammar::Expression>(u8"Bartholemew");
	test<fuzz_grammar::Expression>(u8"PiByFour");
	test<fuzz_grammar::Expression>(u8"(true)");
	test<fuzz_grammar::Expression>(u8"((true))");
	test<fuzz_grammar::Expression>(u8"5*3");
	test<fuzz_grammar::Expression>(u8"(1/sqrt5)%3");
	test<fuzz_grammar::Expression>(u8"5+3");
	test<fuzz_grammar::Expression>(u8"5+3 > (1+sqrt5)/2");
	test<fuzz_grammar::Expression>(u8"myText == \"Hello, World!\"");

	
	//test<fuzz_grammar::PrimitiveExpression>(u8"true");
	//test<fuzz_grammar::Expression>(u8"true");
	//test(u8"false");
	//test(u8"[3, 2, \"Hi Kat :)\", false]");
	//test(u8"[1, 2, [3, 4, 5]]");
	//test(u8"+");
	//test(u8">=");
	//test(u8">");
	//test(u8"6*3.9");
	//test(u8"[2,4,8]+[6,9,2]");
}

int main() 
{
	::SetConsoleOutputCP(CP_UTF8);
	testFuzz();
	return 0;
}
