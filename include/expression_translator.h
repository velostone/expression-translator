#pragma once

#include <iostream>
#include <map>
#include <string>
#include "tstack.h"

class Term
{
protected:
	enum types { number, operation, open_bracket, close_bracket };
	std::map<char, size_t> pritorities{ {'*', 2}, {'/', 2}, {'+', 1}, {'-', 1} };
	types type;
public:
	types get_type() const noexcept { return type; }
	virtual double get_value() const noexcept = 0;
	virtual char get_op() const noexcept = 0;
	virtual size_t get_priority() const noexcept = 0;
};

class Number : public Term
{
	double value;
	char get_op() const noexcept { return 0; };
	size_t get_priority() const noexcept { return 0; };
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
	size_t get_priority() const noexcept { return 1; };
public:
	OpenBracket() : bracket('(') { type = open_bracket; }
};

class CloseBracket : public Term
{
	char bracket;
	char get_op() const noexcept { return bracket; };
	double get_value() const noexcept { return 0; };
	size_t get_priority() const noexcept { return 1; };
public:
	CloseBracket() : bracket(')') { type = close_bracket; }
};

class Translator : private Term
{
	char get_op() const noexcept { return 0; };
	double get_value() const noexcept { return 0; };
	size_t get_priority() const noexcept { return 0; };
	using Term::types;
	TVector<Term*> terms;
	TVector<Term*> polish_notation;
public:
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
		if (!(N[0] == '\0'))
			terms.push_back(new Number(std::stod(N)));
	}
	void get_expression()
	{
		size_t sz = terms.size();
		for (size_t i = 0; i < sz; ++i)
		{
			if (terms[i]->get_type() == number)
				std::cout << terms[i]->get_value();
			else std::cout << terms[i]->get_op();
		}
		std::cout << std::endl;
	}
	void parser()
	{
		std::map<types, size_t> states{ {number, 0}, {operation, 1}, {open_bracket, 2}, {close_bracket, 3} };
		size_t state;
		size_t open_counter = 0;
		size_t close_counter = 0;
		size_t sz = terms.size();
		state = states[terms[0]->get_type()];
		if (state == 1)
			throw std::logic_error("Operation can't be the first symbol!");
		if (state == 2)
			++open_counter;
		if (state == 3)
			throw std::logic_error("')' can't be the first symbol!");
		for (size_t i = 1; i < sz; ++i)
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
				if (terms[i]->get_type() == number || terms[i]->get_type() == open_bracket)
					throw std::logic_error("Invalid syntax!");
				state = states[terms[i]->get_type()];
				break;
			}
		}
		state = states[terms[sz - 1]->get_type()];
		if (state == 1 || state == 2)
			throw std::logic_error("Invalid syntax!");
		if (open_counter != close_counter)
			throw std::logic_error("Invalid syntax!");
	}
	void converter()
	{
		Stack<Term*> st;
		size_t sz = terms.size();
		for (size_t i = 0; i < sz; ++i)
		{
			if (terms[i]->get_type() == number)
				polish_notation.push_back(terms[i]);
			else if (terms[i]->get_type() == operation)
			{
				if (st.isEmpty())
					st.push(terms[i]);
				else
				{
					while (!st.isEmpty() && st.top()->get_type() == operation && terms[i]->get_priority() <= st.top()->get_priority())
					{
						polish_notation.push_back(st.top());
						st.pop();
					}
					st.push(terms[i]);
				}
			}
			else if (terms[i]->get_type() == open_bracket)
				st.push(terms[i]);
			else if (terms[i]->get_type() == close_bracket)
			{
				while (st.top()->get_type() != open_bracket)
				{
					polish_notation.push_back(st.top());
					st.pop();
				}
				st.pop();
			}
		}
		while (!st.isEmpty())
		{
			polish_notation.push_back(st.top());
			st.pop();
		}
	}
	void get_polish_notation()
	{
		size_t sz = polish_notation.size();
		for (size_t i = 0; i < sz; ++i)
		{
			if (polish_notation[i]->get_type() == number)
				std::cout << polish_notation[i]->get_value();
			else std::cout << polish_notation[i]->get_op();
		}
		std::cout << std::endl;
	}
	double calculator()
	{
		types current_type;
		Stack<double> st;
		double right_numb, left_numb;
		size_t sz = polish_notation.size();
		for (size_t i = 0; i < sz; ++i)
		{
			current_type = polish_notation[i]->get_type();
			if (current_type == number)
				st.push(polish_notation[i]->get_value());
			else
			{
				right_numb = st.top();
				st.pop();
				left_numb = st.top();
				st.pop();
				switch (polish_notation[i]->get_op())
				{
				case '*':
					st.push(left_numb * right_numb);
					break;
				case '/':
					if (right_numb == 0)
						throw std::runtime_error("Runtime error: division by zero!");
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
		}
		return st.top();
	}
};