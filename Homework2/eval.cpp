#include "Map.h"
#include <string>
#include <iostream>
#include <stack>
#include <cctype>
#include <cassert>

using namespace std;

bool syntaxOK(const string infix);
bool inMap(const string infix, const Map& values);
bool hasPrecedence(char cur, char top);
void convert(string infix, string& postfix);
bool isValid(const string infix);
bool isoperator(const char c);

// Evaluates an integer arithmetic expression
//   If infix is a syntactically valid infix integer expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values map), then postfix is set to the postfix
//   form of the expression; otherwise postfix may or may not be
//   changed, result is unchanged, and the function returns 1.  If
//   infix is syntactically valid but contains at least one lower
//   case letter operand that does not appear in the values map, then
//   result is unchanged and the function returns 2.  If infix is
//   syntactically valid and all its lower case operand letters
//   appear in the values map, then if evaluating the expression
//   (using for each letter in the expression the value in the map
//   that corresponds to it) attempts to divide by zero, then result
//   is unchanged and the function returns 3; otherwise, result is
//   set to the value of the expression and the function returns 0.
int evaluate(string infix, const Map& values, string& postfix, int& result)
{
	int v;
	convert(infix, postfix);
	if (!syntaxOK(infix))
		return 1;
	if (!inMap(infix, values))
		return 2;
	stack<int> eval;
	for (int i = 0; i < postfix.size(); i++)
	{
		if (isalpha(postfix[i]))
		{
			char cur = postfix[i];
			values.get(cur, v);
			eval.push(v);
		}
		else
		{
			int operand1 = eval.top();
			eval.pop();
			int operand2 = eval.top();
			eval.pop();
			int end;
			switch (postfix[i])
			{
			case '+':
				end = operand1 + operand2;
				eval.push(end);
				break;
			case '-':
				end = operand2 - operand1;
				eval.push(end);
				break;
			case '*':
				end = operand1 * operand2;
				eval.push(end);
				break;
			case '/':
				if (operand1 == 0)
					return 3;
				end = operand1 / operand2;
				eval.push(end);
				break;
			}
		}
	}
	result = eval.top();
	return 0;
}

bool isValid(const string infix)
{
	stack<char> p;
	for (int i = 0; i < infix.length(); i++)
	{
		if (infix[i] == '(')
			p.push(infix[i]);
		if (infix[i] == ')')
		{
			if (!p.empty() && p.top() == '(')
				p.pop();
			else
				return false;
		}
	}
	if (!p.empty())
		return false;
	return true;
}

bool isoperator(const char c)
{
	switch (c)
	{
	case '+':
	case '-':
	case '*':
	case '/':
		return true;
	}
	return false;
}

bool syntaxOK(const string infix)
{
	if (infix == "")
		return false;
	int alphaCount = 0;
	int operatorCount = 0;
	if (!isValid(infix))
		return false;
	char first = infix[0];
	char last = infix[infix.size() - 1];
	if (isalpha(first))
	{
		if (isupper(first))
			return false;
		alphaCount++;
	}
	if (isalpha(last))
	{
		if (isupper(last))
			return false;
		alphaCount++;
	}
	switch (first)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case ')':
		return false;

	}
	switch (last)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '(':
		return false;
	}
	for (int i = 1; i < infix.size() - 1; i++)
	{
		char cur = infix[i];
		if (isalpha(cur))
		{
			if (isupper(cur))
				return false;
			cur = 'a';
		}
		switch (cur)
		{
		case '+':
		case '-':
		case '*':
		case '/':
			if (infix[i - 1] != ')' && !isalpha(infix[i - 1]) && !isblank(infix[i - 1]))
				return false;
			if (infix[i + 1] != '(' && !isalpha(infix[i + 1]) && !isblank(infix[i + 1]))
				return false;
			operatorCount++;
			break;
		case 'a':
			if (!isoperator(infix[i - 1]) && infix[i - 1] != '(' && !isblank(infix[i - 1]))
				return false;
			if (!isoperator(infix[i + 1]) && infix[i + 1] != ')' && !isblank(infix[i + 1]))
				return false;
			alphaCount++;
			break;
		case '(':
			if (!isoperator(infix[i - 1]) && !isblank(infix[i - 1]) && infix[i - 1] != '(')
				return false;
			if (!isblank(infix[i + 1]) && !isalpha(infix[i + 1]) && infix[i + 1] != '(')
				return false;
			break;
		case ')':
			if (!isalpha(infix[i - 1]) && !isblank(infix[i - 1]) && infix[i - 1] != ')')
				return false;
			if (!isoperator(infix[i + 1]) && !isblank(infix[i + 1]) && infix[i + 1] != ')')
				return false;
			break;
		}
	}
	if (operatorCount == 0 && alphaCount > 1)
		return false;
	if (alphaCount == 0)
		return false;
	return true;
}

bool inMap(const string infix, const Map& values)
{
	for (int i = 0; i < infix.size(); i++)
	{
		if (isalpha(infix[i]))
		{
			if (!values.contains(infix[i]))
				return false;
		}
	}
	return true;
}

bool hasPrecedence(char cur, char top)
{
	if (cur == '+' || cur == '-')
	{
		if (top == '+' || top == '-')
			return true;
		else
			return false;
	}
	return true;
}

void convert(string infix, string& postfix)
{
	string infixCopy = infix;
	postfix = "";
	stack<char> operators;
	for (int i = 0; i < infixCopy.size(); i++)
	{
		if (isalpha(infixCopy[i]))
			infixCopy[i] = 'a';
		switch (infixCopy[i])
		{
			case 'a':
				postfix += infix[i];
				break;
			case '(':
				operators.push(infix[i]);
				break;
			case ')':
				while (operators.top() != '(')
				{
					postfix += operators.top();
					operators.pop();
				}
				operators.pop();
				break;
			case '+':
			case '-':
			case '*':
			case '/':
				while (!operators.empty() && operators.top() != '(' && !hasPrecedence(infix[i], operators.top()))
				{
					postfix += operators.top();
					operators.pop();
				}
				operators.push(infix[i]);
				break;
		}
	}
	while (!operators.empty())
	{
		postfix += operators.top();
		operators.pop();
	}
}

int main()
{
	char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	int  vals[] = { 3,  -9,   6,   2,   4,   1 };
	Map m;
	for (int k = 0; vars[k] != '#'; k++)
		m.insert(vars[k], vals[k]);
	string pf;
	int answer;
	assert(evaluate("a+ e", m, pf, answer) == 0 &&
		pf == "ae+" && answer == -6);
	answer = 999;
	assert(evaluate("", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a i", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("ai", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("()", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("()o", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("y(o+u)", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("y(*o)", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+E", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("(a+(i-o)", m, pf, answer) == 1 && answer == 999);
	//// unary operators not allowed:
	assert(evaluate("-a", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a*b", m, pf, answer) == 2 &&
		pf == "ab*" && answer == 999);
	assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0 &&
		pf == "yoau-*+" && answer == -1);
	answer = 999;
	assert(evaluate("o/(y-y)", m, pf, answer) == 3 &&
		pf == "oyy-/" && answer == 999);
	assert(evaluate(" a  ", m, pf, answer) == 0 &&
		pf == "a" && answer == 3);
	assert(evaluate("((a))", m, pf, answer) == 0 &&
		pf == "a" && answer == 3);
	cout << "Passed all tests" << endl;
}