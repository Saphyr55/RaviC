#pragma once

#include <vector>
#include "lexer.hpp"
#include "ast/expression.hpp"

namespace AST {

class Statement {
public:
	virtual ~Statement() = default;
};

namespace Stmt {

class Block : public Statement {
public:
	Block(std::vector<Statement> statements) : Statements(statements) {}
	std::vector<Statement> Statements;
};

class Expression: public Statement {
public:
	Expression(::AST::Expr::Expression expression) : expression(expression) { }
	::AST::Expr::Expression expression;
};

class If : public Statement {
public:
	If(Expression condition, Statement thenBranch, Statement elseBranch) :
		condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
	Expression condition;
	Statement thenBranch, elseBranch;
};

class LetStatement : public Statement {
public:
	LetStatement(
		Token name,
		Token type,
		Expression initializer,
		bool mutable_
	) : 
		name(name),
		type(type),
		initializer(initializer),
		mutable_(mutable_) {}
	Token name;
	Token type;
	Expression initializer;
	bool mutable_;
};

class Return : public Statement {
public:

};

class Function : public Statement {
public:
	Function(
		Token name,
		std::vector<Token> paramsId,
		std::vector<Token> paramsType,
		std::vector<Statement> block,
		Token returnType
	) : 
		Name(name),
		ParamsId(paramsId),
		ParamsType(paramsType),
		ReturnType(returnType) { }

	Token Name;
	std::vector<Token> ParamsId;
	std::vector<Token> ParamsType;
	std::vector<Statement> block;
	Token ReturnType;
};

}
}