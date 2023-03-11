#include "parser.hpp"

Parser::Parser(Lexer lexer) {
	
}

std::vector<AST::Statement> Parser::Parse()
{
	return std::vector<AST::Statement>();
}

AST::Statement Parser::CreateDeclaration()
{
	return AST::Statement();
}

AST::Statement Parser::CreateStructStatement()
{
	return AST::Statement();
}

AST::Statement Parser::CreateClassStatement()
{
	return AST::Statement();
}

AST::Statement Parser::CreateFunction()
{
	return AST::Statement();
}

AST::Statement Parser::CreateLetStatement()
{
	return AST::Statement();
}

AST::Statement Parser::CreatePropertyDeclaration()
{
	return AST::Statement();
}

AST::Statement Parser::CreateReturnStatement()
{
	return AST::Statement();
}

AST::Statement Parser::CreateForStatement()
{
	return AST::Statement();
}

AST::Statement Parser::CreateWhileStatement()
{
	return AST::Statement();
}

AST::Statement Parser::CreateIfStatement()
{
	return AST::Statement();
}

AST::Statement Parser::CreateStatement()
{
	return AST::Statement();
}

std::vector<AST::Statement> Parser::CreateBlock()
{
	return std::vector<AST::Statement>();
}

std::vector<AST::Statement> Parser::CreateStructInitializer()
{
	return std::vector<AST::Statement>();
}

AST::Statement Parser::CreateExpressionStatement()
{
	return AST::Statement();
}

AST::Expression Parser::ExpressionConsumeStatement()
{
	return AST::Expression();
}

AST::Expression Parser::Expression()
{
	return AST::Expression();
}

AST::Expression Parser::Assignement()
{
	return AST::Expression();
}

AST::Expression Parser::Or()
{
	return AST::Expression();
}

AST::Expression Parser::And()
{
	return AST::Expression();
}

AST::Expression Parser::Equality()
{
	return AST::Expression();
}

AST::Expression Parser::Comparison()
{
	return AST::Expression();
}

AST::Expression Parser::Term()
{
	return AST::Expression();
}

AST::Expression Parser::Factor()
{
	return AST::Expression();
}

AST::Expression Parser::Unary()
{
	return AST::Expression();
}

AST::Expression Parser::Call()
{
	return AST::Expression();
}

AST::Expression Parser::FinishCall(AST::Expression callee)
{
	return AST::Expression();
}

AST::Expression Parser::Primary()
{

}

void Parser::Synchronize() {
	Advance();
	while (!IsAtEnd()) {
		if (Previous().KindType == Token::Kind::Semicolon) {
			return;
		}
		switch (Peek().KindType)
		{
		case Token::Kind::Class:
			return;
		case Token::Kind::Func:
			return;
		case Token::Kind::Let:
			return;
		case Token::Kind::For:
			return;
		case Token::Kind::Namespace:
			return;
		case Token::Kind::If:
			return;
		case Token::Kind::Mut:
			return;
		case Token::Kind::Struct:
			return;
		case Token::Kind::Return:
			return;
		case Token::Kind::While:
			return;
		default:
			break;
		}
		Advance();
	}
}

Token Parser::Consume(Token::Kind kind, const std::string_view message) {
	if (Check(kind)) {
		return Advance();
	}
	throw std::exception(message.data());
}

bool Parser::Match(Token::Kind kind) {
	if (Check(kind)) {
		Advance();
		return true;
	}
	return false;
}


bool Parser::Check(Token::Kind kind) {
	if (IsAtEnd()) return false;
	return Peek().KindType == kind;
}

Token Parser::Advance() {
	if (!IsAtEnd()) {
		m_current++;
	}
	return Previous();
}

Token Parser::Peek() {
	return m_tokens[m_current];
}

bool Parser::IsAtEnd() {
	return Peek().KindType == Token::Kind::TkEOF;
}

Token Parser::Previous() {
	return Previous(1);
}

Token Parser::Previous(const std::int32_t offset) {
	return m_tokens.at(m_current - offset);
}
