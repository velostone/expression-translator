#include "gtest.h"
#include "expression_translator.h"

TEST(Number, can_create_number)
{
	ASSERT_NO_THROW(Number num(2.5));
}

TEST(Number, can_get_number_value)
{
	Number num(2.5);
	EXPECT_EQ(num.get_value(), 2.5);
}

TEST(Operation, can_create_operation)
{
	ASSERT_NO_THROW(Operation op(2.5));
}

TEST(Operation, can_get_operation)
{
	Operation op('+');
	EXPECT_EQ(op.get_op(), '+');
}

TEST(OpenBracket, can_create_open_bracket)
{
	ASSERT_NO_THROW(OpenBracket ob);
}

TEST(OpenBracket, can_get_open_bracket)
{
	OpenBracket ob;
	EXPECT_EQ(ob.get_op(), '(');
}

TEST(CloseBracket, can_create_close_bracket)
{
	ASSERT_NO_THROW(CloseBracket ob);
}

TEST(CloseBracket, can_get_close_bracket)
{
	CloseBracket cb;
	EXPECT_EQ(cb.get_op(), ')');
}

TEST(Translator, can_create_translator)
{
	ASSERT_NO_THROW(Translator T);
}

class TestTranslator : public ::testing::Test
{
protected:
	std::string* str;
	Translator* T;
public:
	void SetUp()
	{
		str = new std::string("(2.5+3.77)-16/8*(12-5.4)");
		T = new Translator(*str);
	}
	void TearDown()
	{
		delete str;
		delete T;
	}
};

TEST(Translator, can_create_translator_with_expression)
{
	ASSERT_NO_THROW(Translator T);
}

TEST_F(TestTranslator, can_get_expression)
{
	EXPECT_EQ(this->T->get_expression(), *(this->str));
}

TEST_F(TestTranslator, can_get_expression_size)
{
	EXPECT_EQ(this->T->get_expression_size(), this->str->size());
}

TEST_F(TestTranslator, can_tokenize_expression)
{
	ASSERT_NO_THROW(this->T->tokenizer());
}

TEST_F(TestTranslator, can_get_terms_size_after_tokenizer)
{
	this->T->tokenizer();
	EXPECT_EQ(this->T->get_terms_size(), 15);
}

TEST(Translator, throws_then_tokenize_non_correctly_syntax_expression)
{
	Translator T("23+d46+(4*4)");
	ASSERT_ANY_THROW(T.tokenizer());
}

TEST_F(TestTranslator, can_parse_expression)
{
	this->T->tokenizer();
	ASSERT_NO_THROW(this->T->parser());
}

TEST(Translator, throws_then_parse_non_correctly_terms_expression)
{
	Translator T("23+46(4*4)");
	T.tokenizer();
	ASSERT_ANY_THROW(T.parser());
}

TEST(Translator, throws_then_parse_non_correctly_brackets_expression)
{
	Translator T("23+46+(4*4))");
	T.tokenizer();
	ASSERT_ANY_THROW(T.parser());
}

TEST(Translator, throws_then_parse_non_correctly_first_symbol_expression)
{
	Translator T(")23+46+(4*4)");
	T.tokenizer();
	ASSERT_ANY_THROW(T.parser());
}

TEST(Translator, throws_then_parse_non_correctly_last_symbol_expression)
{
	Translator T("23+46+(4*4)*");
	T.tokenizer();
	ASSERT_ANY_THROW(T.parser());
}

TEST_F(TestTranslator, can_convert_expression)
{
	this->T->tokenizer();
	this->T->parser();
	ASSERT_NO_THROW(this->T->converter());
}

TEST_F(TestTranslator, converter_works_correctly)
{
	this->T->tokenizer();
	this->T->parser();
	this->T->converter();
	EXPECT_EQ(this->T->get_polish_notation(), "2.53.77+168/125.4-*-");
}

TEST_F(TestTranslator, can_calculate_expression)
{
	ASSERT_NO_THROW(this->T->calculate());
}

TEST_F(TestTranslator, calculator_works_correctly)
{
	EXPECT_EQ(this->T->calculate(), -6.93);
}

TEST(Translator, throws_then_division_by_zero)
{
	Translator T("13/(8-2*4)");
	ASSERT_ANY_THROW(T.calculate());
}