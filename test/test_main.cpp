#include <gtest.h>
#include "expression_translator.h"

int main(int argc, char** argv)
{
	//  ::testing::InitGoogleTest(&argc, argv);
	//  return RUN_ALL_TESTS();
	std::string st;
	std::cin >> st;
	Translator T;
	try
	{
		T.tokenizer(st);
	}
	catch (std::exception& e) { std::cerr << e.what() << std::endl; }
}
