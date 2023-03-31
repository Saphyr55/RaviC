#pragma once

#include <vector>
#include "analysis/lexer.hpp"
#include "analysis/ast/statement.hpp"
#include "analysis/ast/expression.hpp"

namespace AST {

class Parser {

public:
	Parser(std::vector<Ref<Analysis::Token>> tokens);
	static std::vector<RStatement> Parse(const std::string_view text);
	std::vector<RStatement> Parse();

private:
	Ref<Statement> CreateDeclaration();
	Ref<Statement> CreateStructStatement();
	Ref<Statement> CreateClassStatement();
	Ref<Stmt::Function> CreateFunction(bool isLambda);
	Ref<Statement> CreateLetStatement();
	Ref<Statement> CreatePropertyDeclaration();
	Ref<Statement> CreateReturnStatement();
	Ref<Statement> CreateForStatement();
	Ref<Statement> CreateWhileStatement();
	Ref<Statement> CreateIfStatement();
	Ref<Statement> CreateStatement();
	std::vector<RStatement> CreateBlock();
	std::vector<RStatement> CreateStructInitializer();
	Ref<Statement> CreateExpressionStatement();
	Ref<Expression> ExpressionConsumeStatement();
	Ref<Expression> CExpression();
	Ref<Expression> Assignement();
	Ref<Expression> Or();
	Ref<Expression> And();
	Ref<Expression> Equality();
	Ref<Expression> Comparison();
	Ref<Expression> Term();
	Ref<Expression> Factor();
	Ref<Expression> Unary();
	Ref<Expression> Call();
	Ref<Expression> FinishCall(Ref<Expression> callee);
	Ref<Expression> Primary();

	bool IsAtEnd();
	void Synchronize();
	bool Match(Analysis::Token::Kind kind);
	bool Check(Analysis::Token::Kind kind);
	Ref<Analysis::Token> Consume(Analysis::Token::Kind kind, const std::string_view message);
	Ref<Analysis::Token> Advance();
	Ref<Analysis::Token> Peek();

	Ref<Analysis::Token> Previous();
	Ref<Analysis::Token> Previous(const std::int32_t offset);

	static std::exception Report(Ref<Analysis::Token>, const std::string&);
	static std::string Diagnostic(Ref<Analysis::Token>, const std::string&);

private:

	std::vector<Ref<Analysis::Token>> m_tokens;
	std::size_t m_current = 0;
	std::vector<Ref<Statement>> m_statements;

};

}