#include "parser.hpp"
#include <iostream>

Parser::Parser(Lexer lexer) {
	
}

std::vector<AST::RStatement> Parser::Parse() {
	return std::vector<AST::RStatement>();
}

AST::RStatement Parser::CreateDeclaration() {
	try {
		if (Match(Token::Kind::Func)) return CreateFunction(false);
		if (Match(Token::Kind::Let)) return CreateLetStatement();

		return CreateStatement();
	}
	catch (const std::exception&) {
		Synchronize();
	}
}

AST::RStatement Parser::CreateStructStatement() {
	return nullptr;
}

AST::RStatement Parser::CreateClassStatement() {
	return nullptr;
}

AST::RStatement Parser::CreateFunction(bool isLambda) {

	Ref<Token> name;
	Ref<Token> returnType;
	std::vector<RToken> parametersId;
	std::vector<RToken> parametersType;
	if (!isLambda) {
		name = Consume(Token::Kind::Identifier, "Expected a function name.");
	}
	Consume(Token::Kind::OpenParenthesis, "Expect '(' after function name.");
	if (!Check(Token::Kind::CloseParenthesis)) {
		do {
			if (parametersId.size() >= 255)
				throw Report(Peek(), "Can't have more than 255 parameters.");
			RToken id = Consume(Token::Kind::Identifier, "Expect parameter name.");
			Consume(Token::Kind::Colon, "Expect ':' to precise the type.");
			RToken type = Consume(Token::Kind::Identifier, "Expect a type for '" + std::string(id->Text.c_str()) + "'.");
			parametersId.push_back(id);
			parametersType.push_back(type);
		} while (Match(Token::Kind::Comma));
	}

	Consume(Token::Kind::CloseParenthesis, "Expect ')' after parameters.");

	if (Match(Token::Kind::Arrow))
		returnType = Consume(Token::Kind::Identifier, "Expected a return type after '->'.");

	Consume(Token::Kind::OpenBracket, "Expect '{' before function body.");

	std::vector<AST::RStatement> body = CreateBlock();

	return std::make_shared<AST::Stmt::Function>(name, returnType, parametersId, parametersType, body);
}

AST::RStatement Parser::CreateLetStatement()
{
	return std::shared_ptr<AST::Statement>();
}

AST::RStatement Parser::CreatePropertyDeclaration()
{
	return std::shared_ptr<AST::Statement>();
}

AST::RStatement Parser::CreateReturnStatement()
{
	return  std::shared_ptr<AST::Statement>();
}

AST::RStatement Parser::CreateForStatement()
{
	return std::shared_ptr<AST::Statement>();
}

AST::RStatement Parser::CreateWhileStatement()
{
	return std::shared_ptr<AST::Statement>();
}

AST::RStatement Parser::CreateIfStatement()
{
	return  std::shared_ptr<AST::Statement>();
}

AST::RStatement Parser::CreateStatement()
{
	return std::shared_ptr<AST::Statement>();
}

std::vector<AST::RStatement> Parser::CreateBlock()
{
	return std::vector<AST::RStatement>();
}

std::vector<AST::RStatement> Parser::CreateStructInitializer()
{
	return std::vector<AST::RStatement>();
}

AST::RStatement Parser::CreateExpressionStatement()
{
	return std::shared_ptr<AST::Statement>();
}

AST::RExpression Parser::ExpressionConsumeStatement()
{
	return std::shared_ptr<AST::Expression>();
}

AST::RExpression Parser::CExpression()
{
	return Assignement();
}

AST::RExpression Parser::Assignement()
{
	AST::RExpression expression = Or();

	if (Match(Token::Kind::Assign)) {

		RToken equals = Previous();
		/*

		AST::RExpression value = Or();
		if (AST::Expr::Let* let = dynamic_cast<AST::Expr::Let*>(expression.get())) {
			return new AssignExpression(le.name(), value);
		}
		else if (expression instanceof GetterExpression ge) {
			return new Expression.Setter(ge.getObject(), ge.getName(), value);
		}
		*/
		throw Report(equals, "Invalid assignment target.");
	}

	return expression;
}

AST::RExpression Parser::Or()
{

}

AST::RExpression Parser::And()
{
	return std::shared_ptr<AST::Expression>();
}

AST::RExpression Parser::Equality()
{
	return std::shared_ptr<AST::Expression>();
}

AST::RExpression Parser::Comparison()
{
	return std::shared_ptr<AST::Expression>();
}

AST::RExpression Parser::Term()
{
	return std::shared_ptr<AST::Expression>();
}

AST::RExpression Parser::Factor()
{
	return std::shared_ptr<AST::Expression>();
}

AST::RExpression Parser::Unary()
{
	return std::shared_ptr<AST::Expression>();
}

AST::RExpression Parser::Call()
{
	return std::shared_ptr<AST::Expression>();
}

AST::RExpression Parser::FinishCall(AST::RExpression callee)
{
	return std::shared_ptr<AST::Expression>();
}

AST::RExpression Parser::Primary() {

	if (Match(Token::Kind::False)) return std::make_shared<AST::Expr::Literal>(false);
	if (Match(Token::Kind::True)) return std::make_shared<AST::Expr::Literal>(true);
	// if (Match(Token::Kind::Null)) return std::make_shared<AST::Expr::Literal>(nullptr);
	if (Match(Token::Kind::Number)) return std::make_shared<AST::Expr::Literal>(Previous()->Data);
	if (Match(Token::Kind::String)) return std::make_shared<AST::Expr::Literal>(Previous()->Data);
	if (Match(Token::Kind::Identifier)) return std::make_shared<AST::Expr::Let>(Previous());
	// if (Match(Token::Kind::ThisKw)) return std::make_shared<AST::Expr::This>(previous());
	if (Match(Token::Kind::Func)) return std::make_shared<AST::Expr::Lambda>(CreateFunction(true));
	if (Match(Token::Kind::OpenParenthesis)) {
		AST::RExpression expr = CExpression();
		Consume(Token::Kind::CloseParenthesis, "Expect ')' after expression.");
		return std::make_shared<AST::Expr::Grouping>(expr);
	}
	throw Report(Peek(), "Expect expression.");
}

void Parser::Synchronize() {
	Advance();
	while (!IsAtEnd()) {
		if (Previous()->KindType == Token::Kind::Semicolon) {
			return;
		}
		switch (Peek()->KindType)
		{
//		case Token::Kind::Class:
//			return;
		case Token::Kind::Func:
			return;
		case Token::Kind::Let:
			return;
		case Token::Kind::For:
			return;
//		case Token::Kind::Namespace:
//			return;
		case Token::Kind::If:
			return;
		case Token::Kind::Mut:
			return;
//		case Token::Kind::Struct:
//			return;
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

RToken Parser::Consume(Token::Kind kind, const std::string_view message) {
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
	return Peek()->KindType == kind;
}

Ref<Token> Parser::Advance() {
	if (!IsAtEnd()) {
		m_current++;
	}
	return Previous();
}

Ref<Token> Parser::Peek() {
	return m_tokens[m_current];
}

bool Parser::IsAtEnd() {
	return Peek()->KindType == Token::Kind::TkEOF;
}

Ref<Token> Parser::Previous() {
	return Previous(1);
}

Ref<Token> Parser::Previous(const std::int32_t offset) {
	return m_tokens.at(m_current - offset);
}

std::exception Parser::Report(Ref<Token> tk, const std::string& msg) {
	std::cerr << Diagnostic(tk, msg) << "\n";
	return std::exception();
}

std::string Parser::Diagnostic(Ref<Token> tk, const std::string& msg) {
	if (tk->KindType == Token::Kind::TkEOF) return "Error: " + msg + " at end";
	return "Error: To the line" + std::to_string(tk->Line + 1) + " for '" + tk->Text.c_str() + "' | " + msg;
}
