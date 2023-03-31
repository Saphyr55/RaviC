#include "analysis/ast/parser.hpp"
#include <iostream>

namespace AST {

	Parser::Parser(std::vector<Ref<Analysis::Token>> tokens) 
		: m_tokens(tokens) 
	{ }

	std::vector<RStatement> Parser::Parse(const std::string_view text) {
		
		Analysis::Lexer lexer(text);
		
		std::queue<Analysis::RToken> queue_tokens = lexer.Scan();
		std::vector<Analysis::RToken> tokens;
		for (int i = 0; !queue_tokens.empty(); i++) {
			tokens.push_back(queue_tokens.front());
			queue_tokens.pop();
		}

		Parser parser(tokens);

		return parser.Parse();
	}

	std::vector<RStatement> Parser::Parse() {

		while (!IsAtEnd()) m_statements.push_back(CreateDeclaration());
		return m_statements;
	}

	RStatement Parser::CreateDeclaration() {

		try {
			if (Match(Analysis::Token::Kind::Func)) return CreateFunction(false);
			if (Match(Analysis::Token::Kind::Let)) return CreateLetStatement();

			return CreateStatement();
		}
		catch (const std::exception&) {
			Synchronize();
		}
		return nullptr;
	}

	RStatement Parser::CreateStatement()
	{
		if (Match(Analysis::Token::Kind::For)) return CreateForStatement();
		if (Match(Analysis::Token::Kind::While)) return CreateWhileStatement();
		if (Match(Analysis::Token::Kind::If)) return CreateIfStatement();
		if (Match(Analysis::Token::Kind::Return)) return CreateReturnStatement();
		if (Match(Analysis::Token::Kind::OpenBracket)) return std::make_shared<Stmt::Block>(CreateBlock());
		return CreateExpressionStatement();
	}

	RStatement Parser::CreateStructStatement() {
		return nullptr;
	}

	RStatement Parser::CreateClassStatement() {
		return nullptr;
	}

	Ref<Stmt::Function> Parser::CreateFunction(bool isLambda) {

		Ref<Analysis::Token> name;
		Ref<Analysis::Token> returnType;
		std::vector<Analysis::RToken> parametersId;
		std::vector<Analysis::RToken> parametersType;
		if (!isLambda) {
			name = Consume(Analysis::Token::Kind::Identifier, "Expected a function name.");
		}
		Consume(Analysis::Token::Kind::OpenParenthesis, "Expect '(' after function name.");
		if (!Check(Analysis::Token::Kind::CloseParenthesis)) {
			do {
				if (parametersId.size() >= 255)
					throw Report(Peek(), "Can't have more than 255 parameters.");
				Analysis::RToken id = Consume(Analysis::Token::Kind::Identifier, "Expect parameter name.");
				Consume(Analysis::Token::Kind::Colon, "Expect ':' to precise the type.");
				Analysis::RToken type = Consume(Analysis::Token::Kind::Identifier, "Expect a type for '" + std::string(id->Text.c_str()) + "'.");
				parametersId.push_back(id);
				parametersType.push_back(type);
			} while (Match(Analysis::Token::Kind::Comma));
		}

		Consume(Analysis::Token::Kind::CloseParenthesis, "Expect ')' after parameters.");

		if (Match(Analysis::Token::Kind::Arrow))
			returnType = Consume(Analysis::Token::Kind::Identifier, "Expected a return type after '->'.");

		Consume(Analysis::Token::Kind::OpenBracket, "Expect '{' before function body.");

		std::vector<RStatement> body = CreateBlock();

		return std::make_shared<Stmt::Function>(name, returnType, parametersId, parametersType, body);
	}

	RStatement Parser::CreateLetStatement() {

		bool mutable_ = Match(Analysis::Token::Kind::Mut);
		Analysis::RToken name = Consume(Analysis::Token::Kind::Identifier, "Expect a name before let assignment.");
		Analysis::RToken type;
		RExpression initializer;
		if (Match(Analysis::Token::Kind::Colon)) {
			type = Consume(Analysis::Token::Kind::Identifier, "Expected declaring type after ':'.");
		}
		if (Match(Analysis::Token::Kind::Assign)) {
			initializer = CExpression();
		}
		Consume(Analysis::Token::Kind::Semicolon, "Expect ';' after let declaration.");
		return std::make_shared<Stmt::Let>(name, type, initializer, mutable_);
	}

	RStatement Parser::CreatePropertyDeclaration() {

		return std::shared_ptr<Statement>();
	}

	RStatement Parser::CreateReturnStatement() {

		Analysis::RToken kw = Previous();
		RExpression value = Check(Analysis::Token::Kind::Semicolon) ? nullptr : CExpression();
		Consume(Analysis::Token::Kind::Semicolon, "Expected ';' after return value.");
		return std::make_shared<Stmt::Return>(kw, value);
	}

	RStatement Parser::CreateForStatement() {

		Consume(Analysis::Token::Kind::OpenParenthesis, "Expected '(' after 'for' loop");

		RStatement init;
		if (Match(Analysis::Token::Kind::Semicolon)) init = nullptr;
		else if (Match(Analysis::Token::Kind::Let)) init = CreateLetStatement();
		else init = CreateExpressionStatement();

		RExpression condition = Check(Analysis::Token::Kind::Semicolon) ? nullptr : CExpression();
		Consume(Analysis::Token::Kind::Semicolon, "Expected ';' after loop condition");

		RExpression increment = Check(Analysis::Token::Kind::CloseParenthesis) ? nullptr : CExpression();
		Consume(Analysis::Token::Kind::CloseParenthesis, "Expected ')' to close for loop");

		RStatement body = CreateStatement();

		if (increment != nullptr) {
			std::vector<RStatement> block;
			block.push_back(body);
			block.push_back(std::make_shared<Stmt::Expression>(increment));
			body = std::make_shared<Stmt::Block>(block);
		}

		if (condition == nullptr)
			condition = std::make_shared<Expr::Literal>((void*)true);

		body = std::make_shared<Stmt::While>(condition, body);

		if (init != nullptr) {
			std::vector<RStatement> block;
			block.push_back(init);
			block.push_back(body);
			body = std::make_shared<Stmt::Block>(block);
		}

		return body;
	}

	RStatement Parser::CreateWhileStatement() {

		Consume(Analysis::Token::Kind::OpenParenthesis, "Expect '(' after 'while'.");
		RExpression condition = CExpression();
		Consume(Analysis::Token::Kind::CloseParenthesis, "Expect ')' after condition.");
		return std::make_shared<Stmt::While>(condition, CreateStatement());
	}

	RStatement Parser::CreateIfStatement() {

		Consume(Analysis::Token::Kind::OpenParenthesis, "Expected '(' after if.");
		RExpression condition = CExpression();
		Consume(Analysis::Token::Kind::CloseParenthesis, "Expected ')' after if condition.");

		RStatement thenBranch = CreateStatement();
		RStatement elseBranch = Match(Analysis::Token::Kind::Else) ? CreateStatement() : nullptr;

		return std::make_shared<Stmt::If>(condition, thenBranch, elseBranch);
	}

	std::vector<RStatement> Parser::CreateBlock() {
		std::vector<RStatement> statements;
		while (!Check(Analysis::Token::Kind::CloseBracket) && !IsAtEnd()) {
			statements.push_back(CreateDeclaration());
		}
		Consume(Analysis::Token::Kind::CloseBracket, "Expect '}' after block.");
		return statements;
	}

	std::vector<RStatement> Parser::CreateStructInitializer() {

		return std::vector<RStatement>();
	}

	RStatement Parser::CreateExpressionStatement() {

		return std::make_shared<Stmt::Expression>(ExpressionConsumeStatement());
	}

	RExpression Parser::ExpressionConsumeStatement() {

		RExpression value = CExpression();
		Consume(Analysis::Token::Kind::Semicolon, "Expect ';' after value.");
		return value;
	}

	RExpression Parser::CExpression() {

		return Assignement();
	}

	RExpression Parser::Assignement() {

		Ref<Expression> expression = Or();

		if (Match(Analysis::Token::Kind::Assign)) {

			Analysis::RToken equals = Previous();

			Ref<Expression> value = Or();
			if (Ref<Expr::Let> let = std::dynamic_pointer_cast<Expr::Let>(expression)) {
				return std::make_shared<Expr::Assign>(let->Name, value);
			}
			else if (Ref<Expr::Getter> get = std::dynamic_pointer_cast<Expr::Getter>(expression)) {
				return std::make_shared<Expr::Setter>(get->Value, get->Name, value);
			}

			throw Report(equals, "Invalid assignment target.");
		}

		return expression;
	}

	RExpression Parser::Or() {

		RExpression expression = And();

		while (Match(Analysis::Token::Kind::LogicalOr)) {
			Analysis::RToken operator_ = Previous();
			RExpression right = And();
			expression = std::make_shared<Expr::Logical>(expression, operator_, right);
		}
		return expression;
	}

	RExpression Parser::And() {

		RExpression expression = Equality();

		while (Match(Analysis::Token::Kind::LogicalAnd)) {
			Analysis::RToken operator_ = Previous();
			RExpression right = Equality();
			expression = std::make_shared<Expr::Logical>(expression, operator_, right);
		}
		return expression;
	}

	RExpression Parser::Equality() {

		RExpression expression = Comparison();

		while (Match(Analysis::Token::Kind::NotEqual) || Match(Analysis::Token::Kind::Equal)) {
			Analysis::RToken operator_ = Previous();
			RExpression right = Comparison();
			expression = std::make_shared<Expr::Binary>(expression, operator_, right);
		}

		return expression;
	}

	RExpression Parser::Comparison() {

		RExpression expression = Term();

		while (
			Match(Analysis::Token::Kind::Greater) || Match(Analysis::Token::Kind::GreaterEqual) ||
			Match(Analysis::Token::Kind::Less) || Match(Analysis::Token::Kind::LessEqual)) {

			Analysis::RToken operator_ = Previous();
			RExpression right = Term();
			expression = std::make_shared<Expr::Binary>(expression, operator_, right);
		}

		return expression;
	}

	RExpression Parser::Term() {

		RExpression expression = Factor();

		while (Match(Analysis::Token::Kind::Minus) || Match(Analysis::Token::Kind::Plus)) {

			Analysis::RToken operator_ = Previous();
			RExpression right = Factor();
			expression = std::make_shared<Expr::Binary>(expression, operator_, right);
		}

		return expression;
	}

	RExpression Parser::Factor() {

		RExpression expression = Unary();

		while (Match(Analysis::Token::Kind::Not) || Match(Analysis::Token::Kind::Minus)) {

			Analysis::RToken operator_ = Previous();
			RExpression right = Unary();
			expression = std::make_shared<Expr::Binary>(expression, operator_, right);
		}

		return expression;
	}

	RExpression Parser::Unary() {

		if (Match(Analysis::Token::Kind::Not) || Match(Analysis::Token::Kind::Minus)) {

			Analysis::RToken operator_ = Previous();
			RExpression right = Unary();
			return std::make_shared<Expr::Unary>(right, operator_);
		}
		return Call();
	}

	RExpression Parser::Call() {

		RExpression expression = Primary();
		bool search = true;

		while (search) {
		
			if (Match(Analysis::Token::Kind::OpenParenthesis)) {
				
				expression = FinishCall(expression);

			} else if (Match(Analysis::Token::Kind::Dot)) {
				
				Analysis::RToken name = Consume(Analysis::Token::Kind::Identifier, "Expect property name after '.'.");
				expression = std::make_shared<Expr::Getter>(expression, name);
			
			} else search = false;
		}
		return expression;
	}

	RExpression Parser::FinishCall(Ref<Expression> callee) {

		std::vector<RExpression> arguments;

		if (!Check(Analysis::Token::Kind::CloseParenthesis)) {
			do {
				if (arguments.size() >= 255) {
					throw Report(Peek(), "Can't have more than 255 arguments.");
				}
				arguments.push_back(CExpression());
			} while (Match(Analysis::Token::Kind::Comma));
		}

		Analysis::RToken paren = Consume(Analysis::Token::Kind::CloseParenthesis, "Expect ')' after arguments.");

		return std::make_shared<Expr::Call>(callee, arguments);
	}

	RExpression Parser::Primary() {

		if (Match(Analysis::Token::Kind::False)) return std::make_shared<Expr::Literal>((void*)false);
		if (Match(Analysis::Token::Kind::True)) return std::make_shared<Expr::Literal>((void*)true);
		// if (Match(Analysis::Token::Kind::Null)) return std::make_shared<Expr::Literal>(nullptr);
		if (Match(Analysis::Token::Kind::Number)) return std::make_shared<Expr::Literal>(Previous()->Data);
		if (Match(Analysis::Token::Kind::String)) return std::make_shared<Expr::Literal>(Previous()->Data);
		if (Match(Analysis::Token::Kind::Identifier)) return std::make_shared<Expr::Let>(Previous());
		// if (Match(Analysis::Token::Kind::ThisKw)) return std::make_shared<Expr::This>(previous());
		if (Match(Analysis::Token::Kind::Func)) return std::make_shared<Expr::Lambda>(CreateFunction(true));
		if (Match(Analysis::Token::Kind::OpenParenthesis)) {
			RExpression expr = CExpression();
			Consume(Analysis::Token::Kind::CloseParenthesis, "Expect ')' after expression.");
			return std::make_shared<Expr::Grouping>(expr);
		}
		throw Report(Peek(), "Expect expression.");
	}

	void Parser::Synchronize() {
		Advance();
		while (!IsAtEnd()) {
			if (Previous()->KindType == Analysis::Token::Kind::Semicolon) {
				return;
			}
			switch (Peek()->KindType)
			{
				//		case Analysis::Token::Kind::Class:
				//			return;
			case Analysis::Token::Kind::Func:
				return;
			case Analysis::Token::Kind::Let:
				return;
			case Analysis::Token::Kind::For:
				return;
				//		case Analysis::Token::Kind::Namespace:
				//			return;
			case Analysis::Token::Kind::If:
				return;
			case Analysis::Token::Kind::Mut:
				return;
				//		case Analysis::Token::Kind::Struct:
				//			return;
			case Analysis::Token::Kind::Return:
				return;
			case Analysis::Token::Kind::While:
				return;
			default:
				break;
			}
			Advance();
		}
	}

	Ref<Analysis::Token> Parser::Consume(Analysis::Token::Kind kind, const std::string_view message) {
		if (Check(kind)) {
			return Advance();
		}
		throw Report(Peek(), std::string(message));
	}

	bool Parser::Match(Analysis::Token::Kind kind) {
		if (Check(kind)) {
			Advance();
			return true;
		}
		return false;
	}


	bool Parser::Check(Analysis::Token::Kind kind) {
		if (IsAtEnd()) return false;
		return Peek()->KindType == kind;
	}

	Ref<Analysis::Token> Parser::Advance() {
		if (!IsAtEnd()) {
			m_current++;
		}
		return Previous();
	}

	Ref<Analysis::Token> Parser::Peek() {
		return m_tokens[m_current];
	}

	bool Parser::IsAtEnd() {
		return Peek()->KindType == Analysis::Token::Kind::TkEOF;
	}

	Ref<Analysis::Token> Parser::Previous() {
		return Previous(1);
	}

	Ref<Analysis::Token> Parser::Previous(const std::int32_t offset) {
		return m_tokens.at(m_current - offset);
	}

	std::exception Parser::Report(Ref<Analysis::Token> tk, const std::string& msg) {
		std::cerr << Diagnostic(tk, msg) << "\n";
		return std::exception();
	}

	std::string Parser::Diagnostic(Ref<Analysis::Token> tk, const std::string& msg) {
		if (tk->KindType == Analysis::Token::Kind::TkEOF) return "Error: " + msg + " at end";
		return "Error: To the line " + std::to_string(tk->Line + 1) + " for '" + tk->Text.c_str() + "' | " + msg;
	}

}


