#pragma once

#include <vector>
#include "lexer.hpp"
#include "ast/statement.hpp"
#include "ast/expression.hpp"

class Parser {
	
public:
	Parser(Lexer lexer);
	std::vector<AST::RStatement> Parse();

private:
	Ref<AST::Statement> CreateDeclaration();
	Ref<AST::Statement> CreateStructStatement();
	Ref<AST::Statement> CreateClassStatement();
	Ref<AST::Statement> CreateFunction(bool isLambda);
	Ref<AST::Statement> CreateLetStatement();
	Ref<AST::Statement> CreatePropertyDeclaration();
	Ref<AST::Statement> CreateReturnStatement();
	Ref<AST::Statement> CreateForStatement();
	Ref<AST::Statement> CreateWhileStatement();
	Ref<AST::Statement> CreateIfStatement();
	Ref<AST::Statement> CreateStatement();
	std::vector<AST::RStatement> CreateBlock();
	std::vector<AST::RStatement> CreateStructInitializer();
	Ref<AST::Statement> CreateExpressionStatement();
	Ref<AST::Expression> ExpressionConsumeStatement();
	Ref<AST::Expression> CExpression();
	Ref<AST::Expression> Assignement();
	Ref<AST::Expression> Or();
	Ref<AST::Expression> And();
	Ref<AST::Expression> Equality();
	Ref<AST::Expression> Comparison();
	Ref<AST::Expression> Term();
	Ref<AST::Expression> Factor();
	Ref<AST::Expression> Unary();
	Ref<AST::Expression> Call();
	Ref<AST::Expression> FinishCall(Ref<AST::Expression> callee);
	Ref<AST::Expression> Primary();
	void Synchronize();
	Ref<Token> Consume(Token::Kind kind, const std::string_view message);
	bool Match(Token::Kind kind);
	bool Check(Token::Kind kind);
	Ref<Token> Advance();
	Ref<Token> Peek();
	bool IsAtEnd();
	Ref<Token> Previous();
	Ref<Token> Previous(const std::int32_t offset);
	static std::exception Report(Ref<Token>, const std::string&);
	static std::string Diagnostic(Ref<Token>, const std::string&);
private:
	std::vector<Ref<Token>> m_tokens;
	std::size_t m_current = 0;
	std::vector<AST::Statement> m_statements;
};