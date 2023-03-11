#pragma once

#include <string>
#include <vector>
#include <memory>

#include "common.hpp"

namespace AST {

namespace Expr {
	
	class Expression {
	public:
		virtual ~Expression() = default;
	};

	class Number : public Expression {
	public:
		Number(double val) : Value(val) { }
		double Value;
	};
	
	class Unary : public Expression {
	public:
		Unary(Expression right, Token op) : Right(right), Op(op) { }
		Expression Right;
		Token Op;
	};

	class Logical : public Expression {
	public:
		Logical(Expression left, Expression right, Token op) : 
			Left(left), Right(right), Op(op) { };
		Expression Left;
		Expression Right;
		Token Op;
	};

	class This : public Expression {
	public:
		This(Token kw) : kw(kw) { }
		Token kw;
	};

	class Grouping : public Expression {
	public:
		Grouping(Expression e) : expression(e) { }
		Expression expression;
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
		Binary(Token op, Ref<Expression> left, Ref<Expression> right) :
			Op(op),
			Left(std::move(left)),
			Right(std::move(right)) { }
		Token Op;
		Ref<Expression> Left, Right;
	};

	class Literal : public Expression {
	public:
		Literal(void* value) : Value(value) { };
		void* Value;
	};

}

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
}