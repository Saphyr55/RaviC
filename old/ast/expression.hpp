#pragma once

#ifndef RAVI_EXPRESSION_H
#define RAVI_EXPRESSION_H

#include <string>
#include <vector>
#include <memory>

#include "common/common.hpp"
#include "analysis/lexer.hpp"

namespace AST {

class Expression {
public:
	virtual ~Expression() = default;
	virtual Ref<Value> GenCode() = 0;
};

namespace Stmt {
class Function;
}

using RExpression = Ref<Expression>;

namespace Expr {

class Lambda final : public Expression {

public:
	Lambda(Ref<::AST::Stmt::Function> function);
	Ref<Value> GenCode();

	Ref<::AST::Stmt::Function> StatementFunc;
};

class Assign final : public Expression {

public:
	Assign(Ref<Analysis::Token> name, Ref<Expression> value);	
	Ref<Value> GenCode();

	Ref<Expression> Value;
	Ref<Analysis::Token> Name;
};

class Unary final : public Expression {

public:
	Unary(Ref<Expression> right, Ref<Analysis::Token> op);
	Ref<Value> GenCode();
	
	Ref<Expression> Right;
	Ref<Analysis::Token> Op;
};

class Logical final : public Expression {

public:
	Logical(Ref<Expression> left, Ref<Analysis::Token> op, Ref<Expression> right);
	Ref<Value> GenCode();

	Ref<Expression> Left;
	Ref<Expression> Right;
	Ref<Analysis::Token> Op;
};

class This final : public Expression {

public:
	This(Ref<Analysis::Token> kw);
	Ref<Value> GenCode();

	Ref<Analysis::Token> kw;
};

class Grouping final : public Expression {

public:
	Grouping(Ref<Expression> e);
	Ref<Value> GenCode();

	Ref<Expression> Expression_;
};

class Let final : public Expression {

public:
	Let(Ref<Analysis::Token> name);
	Ref<Value> GenCode();

	Ref<Analysis::Token> Name;
};

class Call final : public Expression {

public:
	Call(Ref<Expression> calle, std::vector<Ref<Expression>> args);
	Ref<Value> GenCode();

	Ref<Expression> Calle;
	std::vector<Ref<Expression>> Args;
};

class Binary final : public Expression {

public:
	Binary(Ref<Expression> left, Ref<Analysis::Token> op, Ref<Expression> right);
	Ref<Value> GenCode();

	Ref<Analysis::Token> Op;
	Ref<Expression> Left, Right;
};

class Literal final : public Expression {

public:
	Literal(void* value);
	Ref<Value> GenCode();

	void* Value;
};

class Getter final : public Expression{

public:
	Getter(Ref<Expression> value, Ref<Analysis::Token> name);
	Ref<Value> GenCode();

	Ref<Expression> Value;
	Ref<Analysis::Token> Name;
};

class Setter final : public Expression {

public:
	Setter(Ref<Expression> object, Ref<Analysis::Token> name, Ref<Expression> value);
	Ref<Value> GenCode();

	Ref<Expression> Value;
	Ref<Expression> Obj;
	Ref<Analysis::Token> Name;
};

}

}

#endif
