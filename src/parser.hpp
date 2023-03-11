#pragma once

#include <vector>
#include "lexer.hpp"
#include "ast/statement.hpp"
#include "ast/expression.hpp"

class Parser {
	
public:
	Parser(Lexer lexer);
	std::vector<AST::Statement> Parse();

private:
	AST::Statement CreateDeclaration();
	AST::Statement CreateStructStatement();
	AST::Statement CreateClassStatement();
	AST::Statement CreateFunction();
	AST::Statement CreateLetStatement();
	AST::Statement CreatePropertyDeclaration();
	AST::Statement CreateReturnStatement();
	AST::Statement CreateForStatement();
	AST::Statement CreateWhileStatement();
	AST::Statement CreateIfStatement();
	AST::Statement CreateStatement();
	std::vector<AST::Statement> CreateBlock();
	std::vector<AST::Statement> CreateStructInitializer();
	AST::Statement CreateExpressionStatement();
	AST::Expression ExpressionConsumeStatement();
	AST::Expression Expression();
	AST::Expression Assignement();
	AST::Expression Or();
	AST::Expression And();
	AST::Expression Equality();
	AST::Expression Comparison();
	AST::Expression Term();
	AST::Expression Factor();
	AST::Expression Unary();
	AST::Expression Call();
	AST::Expression FinishCall(AST::Expression callee);
	AST::Expression Primary();
	void Synchronize();
	Token Consume(Token::Kind kind, const std::string_view message);
	bool Match(Token::Kind kind);
	bool Check(Token::Kind kind);
	Token Advance();
	Token Peek();
	bool IsAtEnd();
	Token Previous();
	Token Previous(const std::int32_t offset);
private:
	std::vector<Token> m_tokens;
	std::size_t m_current = 0;
	std::vector<AST::Statement> m_statements;
};