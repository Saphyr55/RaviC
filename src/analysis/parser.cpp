#include "analysis/parser.hpp"
#include "vm/virtual_machine.hpp"

#include <iostream>
#include <cstdlib>

namespace Analysis {

	void Parser::Expression() {
		
		ParsePrecedence(Precedence::ASSIGNMENT);
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
		ParsePrecedence(Precedence(rule.precedence + 1));

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
		ParsePrecedence(Precedence::UNARY);
		
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

		while (pre <= Rule::Get(m_current->KindType).precedence) {

			Advance();
			Rule previous_rule = Rule::Get(m_previous->KindType);
			previous_rule.infix(*this);
		}
	}
	
	Ref<Token> Parser::Consume(Token::Kind kind, const std::string_view message) {
		
		if (Check(kind)) 
			return Advance();
		
		throw Report(std::string(message));
	}

	Ref<Token> Parser::Advance() {

		m_previous = m_current;
		m_current = Peek();
		return m_current;
	}

	bool Parser::Check(Token::Kind kind) {

		return m_current->KindType == kind;
	}

	bool Parser::IsAtEnd() {

		return m_current->KindType == Token::Kind::TkEOF;
	}

	Ref<Token> Parser::Peek() {

		return lexer.PeekNextToken();
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

		Advance();
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
		{Token::Kind::OpenParenthesis,  Rule(&Parser::Grouping,			nullptr,	Precedence::NONE)},
		{Token::Kind::CloseParenthesis, Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::OpenBracket,		Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::CloseBracket,		Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Comma,			Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Dot,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Minus,			Rule(&Parser::Unary,    &Parser::Binary,	Precedence::TERM)},
		{Token::Kind::Plus,				Rule(nullptr,			&Parser::Binary,	Precedence::TERM)},
		{Token::Kind::Semicolon,		Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Slash,			Rule(nullptr,			&Parser::Binary,	Precedence::FACTOR)},
		{Token::Kind::Star,				Rule(nullptr,			&Parser::Binary,	Precedence::FACTOR)},
		{Token::Kind::Assign,			Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Not,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::NotEqual,			Rule(nullptr,			&Parser::Binary,	Precedence::COMPARISON)},
		{Token::Kind::Equal,			Rule(nullptr,			&Parser::Binary,	Precedence::COMPARISON)},
		{Token::Kind::Greater,			Rule(nullptr,			&Parser::Binary,	Precedence::COMPARISON)},
		{Token::Kind::GreaterEqual,		Rule(nullptr,			&Parser::Binary,	Precedence::COMPARISON)},
		{Token::Kind::Less,				Rule(nullptr,			&Parser::Binary,	Precedence::COMPARISON)},
		{Token::Kind::LessEqual,		Rule(nullptr,			&Parser::Binary,	Precedence::COMPARISON)},
		{Token::Kind::Identifier,		Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::String,			Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Number,			Rule(&Parser::Number,			nullptr,	Precedence::NONE)},
		{Token::Kind::LogicalAnd,		Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Else,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::False,			Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::For,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Func,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::If,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::LogicalOr,		Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Return,			Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::True,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Let,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::TkEOF,				Rule(nullptr,					nullptr,	Precedence::NONE)},
	};

}
