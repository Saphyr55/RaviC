#pragma once

#ifndef RAVI_STATEMENT_H
#define RAVI_STATEMENT_H

#include <vector>
#include "analysis/lexer.hpp"
#include "analysis/ast/expression.hpp"

namespace AST {

class Statement {
public:
	virtual ~Statement() = default;
};

using RStatement = Ref<Statement>;

namespace Stmt {

class Function : public Statement {
public:
	Function(
		Analysis::RToken name,
		Analysis::RToken returnType,
		std::vector<Analysis::RToken> paramsId,
		std::vector<Analysis::RToken> paramsType,
		std::vector<RStatement> block
	);
	Analysis::RToken Name;
	Analysis::RToken ReturnType;
	std::vector<Analysis::RToken> ParamsId;
	std::vector<Analysis::RToken> ParamsType;
	std::vector<RStatement> block;
};

class Block : public Statement {
public:
	Block(std::vector<Ref<Statement>> statements);
	std::vector<Ref<Statement>> Statements;
};

class While : public Statement {
public:
	While(Ref<::AST::Expression> condition_, RStatement body);
	Ref<Statement> Body;
	Ref<::AST::Expression> Condition_;
};

class If : public Statement {
public:
	If(Ref<::AST::Expression> condition, RStatement thenBranch, RStatement elseBranch);
	Ref<::AST::Expression> Condition;
	Ref<Statement> ThenBranch, ElseBranch;
};

class Let : public Statement {
public:
	Let(
		Analysis::RToken name,
		Analysis::RToken type,
		Ref<::AST::Expression> initializer,
		bool mutable_
	);
	Ref<Analysis::Token> Name;
	Ref<Analysis::Token> Type;
	Ref<::AST::Expression> Initializer;
	bool Mutable;
};

class Return : public Statement {
public:
	Return(Analysis::RToken kw, ::AST::RExpression value);
	Analysis::RToken Kw;
	::AST::RExpression Value;
	Ref<Function> FunctionStmt;
};

class Expression: public Statement {
public:
	Expression(Ref<::AST::Expression> expression_);
	Ref<::AST::Expression> Expression_;
};

}
}

#endif
