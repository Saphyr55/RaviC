#pragma once

#include <vector>
#include "lexer.hpp"
#include "ast/expression.hpp"

namespace AST {

class Statement {
public:
	virtual ~Statement() = default;
};

using RStatement = Ref<Statement>;

namespace Stmt {

class Block : public Statement {
public:
	Block(std::vector<RStatement> statements) : Statements(statements) {}
	std::vector<RStatement> Statements;
};

class While : public Statement {
public:
	While(RExpression condition, RStatement body) : Body(body), Condition(condition) { }
	RStatement Body;
	RExpression Condition;
};

class Expression: public Statement {
public:
	Expression(::AST::RExpression expression) : expression(expression) { }
	::AST::RExpression expression;
};

class If : public Statement {
public:
	If(RExpression condition, RStatement thenBranch, RStatement elseBranch) :
		condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
	RExpression condition;
	RStatement thenBranch, elseBranch;
};

class LetStatement : public Statement {
public:
	LetStatement(
		RToken name,
		RToken type,
		RExpression initializer,
		bool mutable_
	) : 
		name(name),
		type(type),
		initializer(initializer),
		mutable_(mutable_) {}
	RToken name;
	RToken type;
	RExpression initializer;
	bool mutable_;
};

class Return : public Statement {
public:

};

class Function : public Statement {
public:
	Function(
		RToken name,
		RToken returnType,
		std::vector<RToken> paramsId,
		std::vector<RToken> paramsType,
		std::vector<RStatement> block
	) : 
		Name(name),
		ParamsId(paramsId),
		ParamsType(paramsType),
		ReturnType(returnType) { }

	RToken Name;
	std::vector<RToken> ParamsId;
	std::vector<RToken> ParamsType;
	std::vector<RStatement> block;
	RToken ReturnType;
};

}
}