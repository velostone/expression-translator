#pragma once

#include <iostream>
#include <map>
#include <string>
#include "tstack.h"

class Term
{
protected:
	enum types { number, operation, open_bracket, close_bracket };
	std::map<char, size_t> pritorities{ {'*', 3}, {'/', 3}, {'+', 2}, {'-', 2}, {'(', 1}, {')', 1} };
	types type;
public:
	types get_type() const noexcept { return type; }
	virtual double get_value() const noexcept = 0;
	virtual char get_op() const noexcept = 0;
};

class Number : public Term
{
	size_t value;
	char get_op() const noexcept { return 0; };
public:
	double get_value() const noexcept { return value; }
	Number(double _value) : value(_value) { type = number; }
};

class Operation : public Term
{
	char op;
	double get_value() const noexcept { return 0; };
	size_t priority;
public:
	char get_op() const noexcept { return op; }
	size_t get_priority() const noexcept { return priority; }
	Operation(char _op) : op(_op)
	{
		priority = pritorities[op];
		type = operation;
	}
};

class OpenBracket : public Term
{
	char bracket;
	char get_op() const noexcept { return bracket; };
	double get_value() const noexcept { return 0; };
public:
	OpenBracket() : bracket('(') { type = open_bracket; }
};

class CloseBracket : public Term
{
	char bracket;
	char get_op() const noexcept { return bracket; };
	double get_value() const noexcept { return 0; };
public:
	CloseBracket() : bracket(')') { type = close_bracket; }
};

class Translator : private Term
{
	char get_op() const noexcept { return 0; };
	double get_value() const noexcept { return 0; };
	using Term::types;
	TVector<Term*> terms;
public:
	double calculator()
	{
		types current_type;
		Stack<double> st;
		double right_numb, left_numb;
		size_t sz = terms.size();
		for (size_t i = 0; i < sz; ++i)
		{
			current_type = terms[i]->get_type();
			if (current_type == number)
				st.push(terms[i]->get_value());
			else
			{
				right_numb = st.top();
				st.pop();
				left_numb = st.top();
				st.pop();
				switch (terms[i]->get_op())
				{
				case '*':
					st.push(left_numb * right_numb);
					break;
				case '/':
					if (right_numb == 0)
						throw std::runtime_error("Division by zero!");
					st.push(left_numb / right_numb);
					break;
				case '+':
					st.push(left_numb + right_numb);
					break;
				case '-':
					st.push(left_numb - right_numb);
					break;
				}
			}
			return st.top();
		}
	}
	void tokenizer(std::string str)
	{
		std::string N;
		size_t number_status = 0;
		for (size_t i = 0; i < str.size(); ++i)
		{
			if (!number_status)
			{
				if (str[i] == '(')
					terms.push_back(new OpenBracket);
				else if (str[i] == '*' || str[i] == '/' || str[i] == '+' || str[i] == '-')
					terms.push_back(new Operation(str[i]));
				else if (str[i] == ')')
					terms.push_back(new CloseBracket);
				else if (str[i] >= 48 && str[i] <= 57 || str[i] == '.')
				{
					number_status = 1;
					N += str[i];
				}
				else throw std::logic_error("Invalid syntax!");
			}
			else
			{
				if (str[i] >= 48 && str[i] <= 57 || str[i] == '.')
					N += str[i];
				else if (str[i] == '(')
				{
					number_status = 0;
					terms.push_back(new Number(std::stod(N)));
					terms.push_back(new OpenBracket);
					N.clear();
				}
				else if (str[i] == '*' || str[i] == '/' || str[i] == '+' || str[i] == '-')
				{
					number_status = 0;
					terms.push_back(new Number(std::stod(N)));
					terms.push_back(new Operation(str[i]));
					N.clear();
				}
				else if (str[i] == ')')
				{
					number_status = 0;
					terms.push_back(new Number(std::stod(N)));
					terms.push_back(new CloseBracket);
					N.clear();
				}
				else throw std::logic_error("Invalid syntax!");
			}
		}
		//for (int i = 0; i < terms.size(); ++i)
		//	std::cout << terms[i]->get_op();
	}
	void parser()
	{
		std::map<types, size_t> states{ { number, 0 }, { operation, 1 }, { open_bracket, 2 }, { close_bracket, 3 } };
		size_t state;
		size_t open_counter = 0;
		size_t close_counter = 0;
		state = states[terms[0]->get_type()];
		if (state == 1)
			throw std::logic_error("Operation can't be the first symbol!");
		if (state == 3)
			throw std::logic_error("')' can't be the first symbol!");
		for (size_t i = 0; i < terms.size(); ++i)
		{
			if (terms[i]->get_type() == open_bracket)
				++open_counter;
			else if (terms[i]->get_type() == close_bracket)
				++close_counter;
			switch (state)
			{
			case 0:
				if (terms[i]->get_type() == open_bracket || terms[i]->get_type() == number)
					throw std::logic_error("Invalid syntax!");
				state = states[terms[i]->get_type()];
				break;
			case 1:
				if (terms[i]->get_type() == operation || terms[i]->get_type() == close_bracket)
					throw std::logic_error("Invalid syntax!");
				state = states[terms[i]->get_type()];
				break;
			case 2:
				if (terms[i]->get_type() == close_bracket || terms[i]->get_type() == operation)
					throw std::logic_error("Invalid syntax!");
				state = states[terms[i]->get_type()];
				break;
			case 3:
				if (terms[i]->get_type() == operation || number)
					throw std::logic_error("Invalid syntax!");
				state = states[terms[i]->get_type()];
				break;
			}
		}
		state = states[terms[terms.size() - 1]->get_type()];
		if (state == 1 || state == 2)
			throw std::logic_error("Invalid syntax!");
		if (open_counter != close_counter)
			throw std::logic_error("Invalid syntax!");
	}
};