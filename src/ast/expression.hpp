#pragma once

#include <string>
#include <vector>
#include <memory>

#include "common.hpp"
#include "ast/statement.hpp"

namespace AST {


class Expression {
public:
	virtual ~Expression() = default;
};

using RExpression = Ref<Expression>;

class Prototype {
public:
	Prototype(const std::string& name, std::vector<std::string> args)
		: m_name(name), m_args(std::move(args)) {}

	const std::string& GetName() const { return m_name; }

private:
	std::string m_name;
	std::vector<std::string> m_args;

};

class FunctionAST {

public:
	FunctionAST(Ref<Prototype> proto, Ref<Expression> body) :
		m_proto(std::move(proto)),
		m_body(std::move(body)) {}

private:
	Ref<Prototype> m_proto;
	Ref<Expression> m_body;

};

namespace Expr {
	
class Number : public Expression {
public:
	Number(double val) : Value(val) { }
	double Value;
};

class Lambda : public Expression {
public:
	Lambda(Ref<Stmt::Function> statement) : StatementFunc(statement) { }
	Ref<Stmt::Function> StatementFunc;
};


class Unary : public Expression {
public:
	Unary(Ref<Expression> right, Ref<Token> op) : Right(right), Op(op) { }
	Ref<Expression> Right;
	Ref<Token> Op;
};

class Logical : public Expression {
public:
	Logical(Ref<Expression> left, Ref<Expression> right, Ref<Token> op) : 
		Left(left), Right(right), Op(op) { };
	Ref<Expression> Left;
	Ref<Expression> Right;
	Ref<Token> Op;
};

class This : public Expression {
public:
	This(Ref<Token> kw) : kw(kw) { }
	Ref<Token> kw;
};

class Grouping : public Expression {
public:
	Grouping(Ref<Expression> e) : expression(e) { }
	Ref<Expression> expression;
};

class Let : public Expression {
public:
	Let(const std::string_view name) : Name(name) { }
	std::string Name;
};

class Call : public Expression {
public:
	Call(const std::string_view calle, std::vector<Ref<Expression>> args)
		: Calle(calle), Args(std::move(args)) {}
	std::string Calle;
	std::vector<Ref<Expression>> Args;
};

class Binary : public Expression {
public:
	Binary(Ref<Token> op, Ref<Expression> left, Ref<Expression> right) :
		Op(op),
		Left(std::move(left)),
		Right(std::move(right)) { }
	Ref<Token> Op;
	Ref<Expression> Left, Right;
};

class Literal : public Expression {
public:
	Literal(void* value) : Value(value) { };
	void* Value;
};

}


}