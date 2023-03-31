#include "analysis/parser.hpp"
#include "vm/virtual_machine.hpp"

#include <iostream>
#include <cstdlib>

namespace Analysis {

	void Parser::Expression() {
		
		ParsePrecedence(ASSIGNMENT);
	}

	void Parser::Number() {
		
		Value value = std::strtod(m_previous->Text.c_str(), nullptr);
		EmitConstant(value);
	}

	void Parser::Grouping() {
		
		Expression();
		Consume(Token::Kind::CloseParenthesis, "Expect ')' after expression.");
	}

	void Parser::Binary() {
		
		RToken operator_ = m_previous;
		Rule rule = Rule::Get(operator_->KindType);
		ParsePrecedence(rule.precedence);

		switch (operator_->KindType)
		{
		case Token::Kind::Plus:
			Emit8(VM::OpCode::Add);
			break;
		case Token::Kind::Minus:
			Emit8(VM::OpCode::Substract);
			break;
		case Token::Kind::Star:
			Emit8(VM::OpCode::Multiply);
			break;
		case Token::Kind::Slash:
			Emit8(VM::OpCode::Divide);
			break;
		default:
			return;
		}
	}

	void Parser::Unary() {
		
		RToken operator_ = m_previous;
		ParsePrecedence(UNARY);
		
		switch (operator_->KindType)
		{
		case Token::Kind::Minus:
			Emit8(VM::OpCode::Negate);
			break;
		default:
			break;
		}
	}

	void Parser::ParsePrecedence(Precedence pre) {
		
		Advance();
		Rule rule = Rule::Get(m_previous->KindType);
		
		if (rule.prefix == nullptr) {
			throw Report("Expect expression.");
		}
		
		rule.prefix(*this);
		
		while (pre <= Rule::Get(m_previous->KindType).precedence) {
			Advance();
			Rule::Get(m_previous->KindType).infix(*this);
		}
	}


	Ref<Token> Parser::Advance() {
		
		m_previous = m_current;
		m_current = lexer.PollToken();
		return m_current;
	}

	Ref<Token> Parser::Consume(Token::Kind kind, const std::string_view message) {
		
		if (Check(kind)) return Advance();
		
		throw Report(std::string(message));
	}

	bool Parser::Check(Token::Kind kind) {
		
		if (IsAtEnd()) return false;
		
		return Peek()->KindType == kind;
	}

	bool Parser::IsAtEnd() {

		return Peek()->KindType == Token::Kind::TkEOF;
	}

	Ref<Token> Parser::Peek() {
		
		return lexer.PeekToken();
	}

	std::exception Parser::Report(Ref<Token> tk, const std::string& msg) {
		
		std::cerr << Diagnostic(tk, msg) << "\n";
		return std::exception();
	}

	std::exception Parser::Report(const std::string& msg) {
		
		return Report(Peek(), msg);
	}

	std::string Parser::Diagnostic(Ref<Token> tk, const std::string& msg) {
		
		if (tk->KindType == Token::Kind::TkEOF) return "Error: " + msg;
		
		return "Error: To the line " +
			std::to_string(tk->Line + 1) + " for '" + tk->Text.c_str() + "' | " + msg;
	}

	Parser::Parser(VM::Chunk& current_chunk, Lexer& lexer)
		: current_chunk(current_chunk), lexer(lexer) {

		lexer.Scan();
		m_current = lexer.PollToken();
	}

	void Parser::Emit8(const Byte& byte) {
		
		current_chunk.SetLine(m_previous->Line);
		current_chunk.Write8(byte);
	}

	void Parser::Emit16(const Byte& byte1, const Byte& byte2) {

		current_chunk.SetLine(m_previous->Line);
		current_chunk.Write16(byte1, byte2);
	}

	void Parser::EmitConstant(const Value& value) {
		auto constant = current_chunk.AddConstant(value);
		if (constant > UINT8_MAX) {
			throw Report("Too many constants in one chunk.");
		}
		Emit16(VM::OpCode::Constant, constant);
	}


	Parser::Rule Parser::Rule::Get(Token::Kind type) {
		return rules[type];
	}

	Parser::Rule::Rule(std::function<void(Parser&)> prefix, std::function<void(Parser&)> infix, Precedence precedence)
		: prefix(prefix), infix(infix), precedence(precedence) { }

	std::unordered_map<Token::Kind, Parser::Rule> Parser::Rule::rules = {
		{Token::Kind::OpenParenthesis, Rule(&Parser::Grouping, nullptr, NONE)},
		{Token::Kind::CloseParenthesis, Rule(nullptr, nullptr,   NONE)},
		{Token::Kind::OpenBracket, Rule(&Parser::Grouping, nullptr,   NONE)},
		{Token::Kind::CloseBracket, Rule( nullptr,     nullptr,   NONE )},
		{Token::Kind::Comma,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Dot,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Minus,Rule(&Parser::Unary,    &Parser::Binary, TERM)},
		{Token::Kind::Plus,Rule(nullptr, &Parser::Binary, TERM )},
		{Token::Kind::Semicolon,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Slash,Rule(nullptr, &Parser::Binary, FACTOR )},
		{Token::Kind::Star,Rule(nullptr, &Parser::Binary, FACTOR )},
		{Token::Kind::Not,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::NotEqual,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Assign,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Equal,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Greater,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::GreaterEqual,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Less,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::LessEqual,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Identifier,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::String,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Number, Rule(&Parser::Number,   nullptr,   NONE)},
		{Token::Kind::LogicalAnd,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Else,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::False,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::For,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Func,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::If,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::LogicalOr,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Return,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::True,Rule(nullptr,     nullptr,   NONE )},
		{Token::Kind::Let,Rule(nullptr,     nullptr,   NONE )}

	};

}
