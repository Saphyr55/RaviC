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
	}


	Ref<Token> Parser::Advance() {
        m_previous = m_current;
		m_current = lexer.NextPeekToken();
		return m_current;
    }

	Ref<Token> Parser::Consume(Token::Kind kind, const std::string_view message) {
		if (Check(kind)) {
			return Advance();
		}
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
		if (tk->KindType == Token::Kind::TkEOF) return "Error: " + msg + " at end";
		return "Error: To the line " + 
			std::to_string(tk->Line + 1) + " for '" + tk->Text.c_str() + "' | " + msg;
	}
	
	Parser::Parser(VM::Chunk& current_chunk, Lexer& lexer) 
		: current_chunk(current_chunk), lexer(lexer) { }

	void Parser::Emit8(const Byte& byte) {
		current_chunk.SetLine(m_previous->Line);
		current_chunk.Write8(byte);
	}

	void Parser::Emit16(const Byte& byte1, const Byte& byte2) {
		current_chunk.SetLine(m_previous->Line);
		current_chunk.Write16(byte1, byte2);
	}

	void Parser::EmitConstant(const Value& value) {
		int constant = current_chunk.AddConstant(value);
		if (constant > UINT8_MAX) {
			throw Report("Too many constants in one chunk.");
		}
		Emit16(VM::OpCode::Constant, constant);
	}


}