#include "ast/parser.hpp"
#include <iostream>

Parser::Parser(std::vector<Ref<Token>> tokens) : m_tokens(tokens){
	
}

std::vector<AST::RStatement> Parser::Parse(const std::string_view text) {
	Lexer lexer(text);
	Parser parser(lexer.Scan());
	return parser.Parse();
}

std::vector<AST::RStatement> Parser::Parse() {
	while (!IsAtEnd()) m_statements.push_back(CreateDeclaration());
	return m_statements;
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
	return nullptr;
}

AST::RStatement Parser::CreateStatement()
{
	if (Match(Token::Kind::For)) return CreateForStatement();
	if (Match(Token::Kind::While)) return CreateWhileStatement();
	if (Match(Token::Kind::If)) return CreateIfStatement();
	if (Match(Token::Kind::Return)) return CreateReturnStatement();
	if (Match(Token::Kind::OpenBracket)) return std::make_shared<AST::Stmt::Block>(CreateBlock());
	return CreateExpressionStatement();
}

AST::RStatement Parser::CreateStructStatement() {
	return nullptr;
}

AST::RStatement Parser::CreateClassStatement() {
	return nullptr;
}

Ref<AST::Stmt::Function> Parser::CreateFunction(bool isLambda) {

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

AST::RStatement Parser::CreateLetStatement() {

	bool mutable_ = Match(Token::Kind::Mut);
	RToken name = Consume(Token::Kind::Identifier, "Expect a name before let assignment.");
	RToken type;
	AST::RExpression initializer;
	if (Match(Token::Kind::Colon)) {
		type = Consume(Token::Kind::Identifier, "Expected declaring type after ':'.");
	}
	if (Match(Token::Kind::Assign)) {
		initializer = CExpression();
	}
	Consume(Token::Kind::Semicolon, "Expect ';' after let declaration.");
	return std::make_shared<AST::Stmt::Let>(name, type, initializer, mutable_);
}

AST::RStatement Parser::CreatePropertyDeclaration() {

	return std::shared_ptr<AST::Statement>();
}

AST::RStatement Parser::CreateReturnStatement() {

	RToken kw = Previous();
	AST::RExpression value = Check(Token::Kind::Semicolon) ? nullptr : CExpression();
	Consume(Token::Kind::Semicolon, "Expected ';' after return value.");
	return std::make_shared<AST::Stmt::Return>(kw, value);
}

AST::RStatement Parser::CreateForStatement() {

	Consume(Token::Kind::OpenParenthesis, "Expected '(' after 'for' loop");

	AST::RStatement init;
	if (Match(Token::Kind::Semicolon)) init = nullptr;
	else if (Match(Token::Kind::Let)) init = CreateLetStatement();
	else init = CreateExpressionStatement();

	AST::RExpression condition = Check(Token::Kind::Semicolon) ? nullptr : CExpression();
	Consume(Token::Kind::Semicolon, "Expected ';' after loop condition");

	AST::RExpression increment = Check(Token::Kind::CloseParenthesis) ? nullptr : CExpression();
	Consume(Token::Kind::CloseParenthesis, "Expected ')' to close for loop");

	AST::RStatement body = CreateStatement();

	if (increment != nullptr) {
		std::vector<AST::RStatement> block;
		block.push_back(body);
		block.push_back(std::make_shared<AST::Stmt::Expression>(increment));
		body = std::make_shared<AST::Stmt::Block>(block);
	}

	if (condition == nullptr)
		condition = std::make_shared<AST::Expr::Literal>((void*)true);

	body = std::make_shared<AST::Stmt::While>(condition, body);

	if (init != nullptr) {
		std::vector<AST::RStatement> block;
		block.push_back(init);
		block.push_back(body);
		body = std::make_shared<AST::Stmt::Block>(block);
	}

	return body;
}

AST::RStatement Parser::CreateWhileStatement() {

	Consume(Token::Kind::OpenParenthesis, "Expect '(' after 'while'.");
	AST::RExpression condition = CExpression();
	Consume(Token::Kind::CloseParenthesis, "Expect ')' after condition.");
	return std::make_shared<AST::Stmt::While>(condition, CreateStatement());
}

AST::RStatement Parser::CreateIfStatement() {

	Consume(Token::Kind::OpenParenthesis, "Expected '(' after if.");
	AST::RExpression condition = CExpression();
	Consume(Token::Kind::CloseParenthesis, "Expected ')' after if condition.");

	AST::RStatement thenBranch = CreateStatement();
	AST::RStatement elseBranch = Match(Token::Kind::Else) ? CreateStatement() : nullptr;

	return std::make_shared<AST::Stmt::If>(condition, thenBranch, elseBranch);
}

std::vector<AST::RStatement> Parser::CreateBlock() {
	std::vector<AST::RStatement> statements;
	while (!Check(Token::Kind::CloseBracket) && !IsAtEnd()) {
		statements.push_back(CreateDeclaration());
	}
	Consume(Token::Kind::CloseBracket, "Expect '}' after block.");
	return statements;
}

std::vector<AST::RStatement> Parser::CreateStructInitializer() {

	return std::vector<AST::RStatement>();
}

AST::RStatement Parser::CreateExpressionStatement() {

	return std::make_shared<AST::Stmt::Expression>(ExpressionConsumeStatement());
}

AST::RExpression Parser::ExpressionConsumeStatement() {

	AST::RExpression value = CExpression();
	Consume(Token::Kind::Semicolon, "Expect ';' after value.");
	return value;
}

AST::RExpression Parser::CExpression() {

	return Assignement();
}

AST::RExpression Parser::Assignement() {

	Ref<AST::Expression> expression = Or();

	if (Match(Token::Kind::Assign)) {

		RToken equals = Previous();

		Ref<AST::Expression> value = Or();
		if (Ref<AST::Expr::Let> let = std::dynamic_pointer_cast<AST::Expr::Let>(expression)) {
			return std::make_shared<AST::Expr::Assign>(let->Name, value);
		} else if (Ref<AST::Expr::Getter> get = std::dynamic_pointer_cast<AST::Expr::Getter>(expression)) {
			return std::make_shared<AST::Expr::Setter>(get->Value, get->Name, value);
		}

		throw Report(equals, "Invalid assignment target.");
	}

	return expression;
}

AST::RExpression Parser::Or() {

	AST::RExpression expression = And();

	while (Match(Token::Kind::LogicalOr)) {
		RToken operator_ = Previous();
		AST::RExpression right = And();
		expression = std::make_shared<AST::Expr::Logical>(expression, operator_, right);
	}
	return expression;
}

AST::RExpression Parser::And() {

	AST::RExpression expression = Equality();

	while (Match(Token::Kind::LogicalAnd)) {
		RToken operator_ = Previous();
		AST::RExpression right = Equality();
		expression = std::make_shared<AST::Expr::Logical>(expression, operator_, right);
	}
	return expression;
}

AST::RExpression Parser::Equality() {

	AST::RExpression expression = Comparison();

	while (Match(Token::Kind::NotEqual) || Match(Token::Kind::Equal)) {
		RToken operator_ = Previous();
		AST::RExpression right = Comparison();
		expression = std::make_shared<AST::Expr::Binary>(expression, operator_, right);
	}

	return expression;
}

AST::RExpression Parser::Comparison() {

	AST::RExpression expression = Term();

	while (
		Match(Token::Kind::Greater) || Match(Token::Kind::GreaterEqual) ||
		Match(Token::Kind::Less)    || Match(Token::Kind::LessEqual)) {

		RToken operator_ = Previous();
		AST::RExpression right = Term();
		expression = std::make_shared<AST::Expr::Binary>(expression, operator_, right);
	}

	return expression;
}

AST::RExpression Parser::Term() {

	AST::RExpression expression = Factor();

	while (Match(Token::Kind::Minus) || Match(Token::Kind::Plus)) {

		RToken operator_ = Previous();
		AST::RExpression right = Factor();
		expression = std::make_shared<AST::Expr::Binary>(expression, operator_, right);
	}

	return expression;
}

AST::RExpression Parser::Factor() {

	AST::RExpression expression = Unary();

	while (Match(Token::Kind::Not) || Match(Token::Kind::Minus)) {

		RToken operator_ = Previous();
		AST::RExpression right = Unary();
		expression = std::make_shared<AST::Expr::Binary>(expression, operator_, right);
	}

	return expression;
}

AST::RExpression Parser::Unary() {

	if (Match(Token::Kind::Not) || Match(Token::Kind::Minus)) {
		RToken operator_ = Previous();
		AST::RExpression right = Unary();
		return std::make_shared<AST::Expr::Unary>(right, operator_);
	}
	return Call();
}

AST::RExpression Parser::Call() {

	AST::RExpression expression = Primary();
	bool search = true;
	while (search) {
		if (Match(Token::Kind::OpenParenthesis)) {
			expression = FinishCall(expression);
		}
		else if (Match(Token::Kind::Dot)) {
			RToken name = Consume(Token::Kind::Identifier, "Expect property name after '.'.");
			expression = std::make_shared<AST::Expr::Getter>(expression, name);
		}
		else search = false;
	}
	return expression;
}

AST::RExpression Parser::FinishCall(Ref<AST::Expression> callee) {

	std::vector<AST::RExpression> arguments;

	if (!Check(Token::Kind::CloseParenthesis)) {
		do {
			if (arguments.size() >= 255) {
				throw Report(Peek(), "Can't have more than 255 arguments.");
			}
			arguments.push_back(CExpression());
		} while (Match(Token::Kind::Comma));
	}

	RToken paren = Consume(Token::Kind::CloseParenthesis, "Expect ')' after arguments.");

	return std::make_shared<AST::Expr::Call>(callee, arguments);
}

AST::RExpression Parser::Primary() {

	if (Match(Token::Kind::False)) return std::make_shared<AST::Expr::Literal>((void*)false);
	if (Match(Token::Kind::True)) return std::make_shared<AST::Expr::Literal>((void*)true);
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

Ref<Token> Parser::Consume(Token::Kind kind, const std::string_view message) {
	if (Check(kind)) {
		return Advance();
	}
	throw Report(Peek(), std::string(message));
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
	return "Error: To the line " + std::to_string(tk->Line + 1) + " for '" + tk->Text.c_str() + "' | " + msg;
}
