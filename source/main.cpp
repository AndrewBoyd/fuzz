﻿#pragma once
#include "fuzz_types.h"
#include "fuzz_grammar.h"
#include "fuzz_strings.h"

#include <utility/string_utils.h>
#include <fmt/format.h>
#include <iostream>
#include <fstream>
#include <Windows.h>

template<typename parse_t = fuzz_grammar::Expression>
void test(auto input_string)
{
	auto as_u8 = string_utils::to_u8(input_string);
	std::cout << "testing: \"" << reinterpret_cast<char const*>(as_u8.c_str()) << "\".... " << std::endl;
	auto result = parse<parse_t>(input_string);
	std::cout << fuzz::to_string(result) << std::endl;
	std::cout << std::endl;
}

void testFuzz() {
	namespace fz = fuzz;
	namespace fg = fuzz_grammar;
	test(u8"true");
	test(u8"false");
	test(u8"3.141592654");
	test(u8"-3");
	test(u8"\"Hello, World\"");
	test(u8"\"Greek: αβγδ; German: Übergrößenträger\"");
	test(u8"Bartholemew");
	test(u8"PiByFour");
	test(u8"(true)");
	test(u8"((true))");
	test(u8"5*3");
	test(u8"(1/sqrt5)%3");
	test(u8"5+3");
	test(u8"5+3 > (1+sqrt5)/2");
	test(u8"myText == \"Hello, World!\"");
	test(u8"return 3+4*q");
	test(u8"let myCoolStuff := 3+4*q");
	test(u8"temp lads := 3+4*q");
	test(u8"[1, 2, cool_beans, 3+38]");
	test(u8"{name; value; [3,2,1]; }");

	test(u8"{{nested}}");
	test(u8"[[nested]]");
	test(u8"[{nested}]");
	test(u8"{[nested]}");

	test(u8"{ return x+y; }");
	test(u8"/x /y { hello; return x+y } ");
	test(u8"a.b$c ");
	test(u8"a$b.c ");
	test(u8"/x /y { return x+y } $ [3, (/x{ return x }).[4] ] ");
}

int main() 
{
	::SetConsoleOutputCP(CP_UTF8);
	testFuzz();
	return 0;
}
