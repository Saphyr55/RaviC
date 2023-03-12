#pragma once

#ifndef RAVI_EXPRESSION_H
#define RAVI_EXPRESSION_H

#include <string>
#include <vector>
#include <memory>

#include "common.hpp"
#include "lexer.hpp"

namespace AST {

class Expression {
public:
	virtual ~Expression() = default;
};

namespace Stmt {
class Function;
}

using RExpression = Ref<Expression>;

namespace Expr {

class Lambda final : public Expression {

public:
	Lambda(Ref<::AST::Stmt::Function> function);
	Ref<::AST::Stmt::Function> StatementFunc;
};

class Assign final : public Expression {
public:
	Assign(Ref<Token> name, Ref<Expression> value);
	Ref<Expression> Value;
	Ref<Token> Name;
};

class Unary final : public Expression {
public:
	Unary(Ref<Expression> right, Ref<Token> op);
	Ref<Expression> Right;
	Ref<Token> Op;
};

class Logical final : public Expression {
public:
	Logical(Ref<Expression> left, Ref<Token> op, Ref<Expression> right);
	Ref<Expression> Left;
	Ref<Expression> Right;
	Ref<Token> Op;
};

class This final : public Expression {
public:
	This(Ref<Token> kw);
	Ref<Token> kw;
};

class Grouping final : public Expression {
public:
	Grouping(Ref<Expression> e);
	Ref<Expression> Expression_;
};

class Let final : public Expression {
public:
	Let(Ref<Token> name);
	Ref<Token> Name;
};

class Call final : public Expression {
public:
	Call(Ref<Expression> calle, std::vector<Ref<Expression>> args);
	Ref<Expression> Calle;
	std::vector<Ref<Expression>> Args;
};

class Binary final : public Expression {
public:
	Binary(Ref<Expression> left, Ref<Token> op, Ref<Expression> right);
	Ref<Token> Op;
	Ref<Expression> Left, Right;
};

class Literal final : public Expression {
public:
	Literal(void* value);
	void* Value;
};

class Getter final : public Expression{
public:
	Getter(Ref<Expression> value, Ref<Token> name);
	Ref<Expression> Value;
	Ref<Token> Name;
};

class Setter final : public Expression {
public:
	Setter(Ref<Expression> object, Ref<Token> name, Ref<Expression> value);
	Ref<Expression> Value;
	Ref<Expression> Obj;
	Ref<Token> Name;
};


}
}

#endif
