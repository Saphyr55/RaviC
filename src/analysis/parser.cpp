#include "analysis/parser.hpp"
#include "vm/virtual_machine.hpp"

#include <iostream>
#include <cstdlib>
#include <utility>

namespace Analysis {

    Parser::Parser(VM::Chunk& current_chunk, Lexer& lexer)
            : m_current_chunk(current_chunk), lexer(lexer) {

    }

    void Parser::Decleration() {
        if (Match(Token::Kind::Let)) {
            LetDeclaration();
        } else Statement();
        if (m_panic_mode)
            Synchronize();
    }

    void Parser::Statement() {
        if (Match(Token::Kind::Arrow)) {
            PrintStatement();
        } else {
            ExpressionStatement();
        }
    }

    void Parser::PrintStatement() {
        Expression();
        Emit8(VM::OpCode::Print);
    }

    void Parser::ExpressionStatement() {
        Expression();
        Emit8(VM::OpCode::Pop);
    }

    void Parser::LetDeclaration() {
        Consume(Token::Kind::Identifier, "Expect variable name.");
        auto addr = IdentifierConstant(m_previous);
        if (Match(Token::Kind::Assign)) {
            Expression();
        } else {
            Emit8(VM::OpCode::Null);
        }
        Consume(Token::Kind::Semicolon, "Expect ';' after variable declaration.");
        Emit16(VM::OpCode::Store, addr);
    }

    void Parser::Expression() {
		ParsePrecedence(Precedence::ASSIGNMENT);
	}

    void Parser::Let() {
        Emit16(VM::OpCode::Load, IdentifierConstant(m_previous));
    }

    void Parser::Literal() {
        switch (m_previous->Type) {
            case Token::Kind::Null:     Emit8(VM::OpCode::Null);    break;
            case Token::Kind::True:     Emit8(VM::OpCode::True);    break;
            case Token::Kind::False:    Emit8(VM::OpCode::False);   break;
            default: return;
        }
    }

    void Parser::String() {
        EmitConstant(Common::Value(CopyStringDQ(m_previous)));
    }

	void Parser::Number() {
		Common::Value value(std::strtod(m_previous->Text.c_str(), nullptr));
		EmitConstant(value);
	}

	void Parser::Grouping() {
		Expression();
		Consume(Token::Kind::CloseParenthesis, "Expect ')' after expression.");
	}

	void Parser::Binary() {

        RToken operator_ = m_previous;
        Rule rule = Rule::Get(operator_->Type);
        ParsePrecedence(Precedence(rule.precedence + 1));

		switch (operator_->Type) {
            case Token::Kind::Plus:         Emit8(VM::OpCode::Add);             break;
            case Token::Kind::Minus:        Emit8(VM::OpCode::Substract);       break;
            case Token::Kind::Star:         Emit8(VM::OpCode::Multiply);        break;
            case Token::Kind::Slash:        Emit8(VM::OpCode::Divide);          break;
            case Token::Kind::Equal:        Emit8(VM::OpCode::Equal);           break;
            case Token::Kind::NotEqual:     Emit8(VM::OpCode::NotEqual);        break;
            case Token::Kind::Less:         Emit8(VM::OpCode::Less);            break;
            case Token::Kind::LessEqual:    Emit8(VM::OpCode::LessEqual);       break;
            case Token::Kind::GreaterEqual: Emit8(VM::OpCode::GreaterEqual);    break;
            case Token::Kind::Greater:      Emit8(VM::OpCode::Greater);         break;
            default: return;
		}
	}

	void Parser::Unary() {
		
		RToken operator_ = m_previous;
		ParsePrecedence(Precedence::UNARY);
		
		switch (operator_->Type) {

            case Token::Kind::Not:
                Emit8(VM::OpCode::Not);
                break;
		    case Token::Kind::Minus:
                Emit8(VM::OpCode::Negate);
                break;
            default:
                break;
		}
	}

	void Parser::ParsePrecedence(Precedence pre) {

		Advance();
		Rule rule = Rule::Get(m_previous->Type);
		
        if (rule.prefix == nullptr) {
            throw Report("Expect expression.");
        }

		rule.prefix(*this);

		while (pre <= Rule::Get(m_current->Type).precedence) {

			Advance();
			Rule previous_rule = Rule::Get(m_previous->Type);
			previous_rule.infix(*this);
		}
	}

    Byte Parser::IdentifierConstant(const Ref<Token>& name) {
        return m_current_chunk.AddConstant(Common::Value(CopyString(name)));
    }

    std::string Parser::CopyStringDQ(const Ref<Token>& tk) {
        return tk->Text.substr(1,  tk->Text.size() - 2);
    }

    std::string Parser::CopyString(const Ref<Token>& tk) {
        return tk->Text.substr(0,  tk->Text.size() - 1);
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
		return m_current->Type == kind;
	}

	bool Parser::IsAtEnd() {
		return m_current->Type == Token::Kind::TkEOF;
	}

	Ref<Token> Parser::Peek() {
		return lexer.PeekNextToken();
	}

	std::exception Parser::Report(const Ref<Token>& tk, const std::string_view msg) {
		std::cerr << Diagnostic(tk, msg) << "\n";
		return {};
	}

	std::exception Parser::Report(const std::string_view msg) {
		return Report(Peek(), msg);
	}

	std::string Parser::Diagnostic(const Ref<Token>& tk, const std::string_view msg) {

        m_panic_mode = true;

		if (tk->Type == Token::Kind::TkEOF) return "Error: " + std::string(msg);
		
		return "Error: To the line " +
			std::to_string(tk->Line + 1) + " for '" + tk->Text + "' | " + std::string(msg);
	}

	void Parser::Emit8(const Byte& byte) {
		m_current_chunk.SetLine(m_previous->Line);
		m_current_chunk.Write8(byte);
	}

	void Parser::Emit16(const Byte& byte1, const Byte& byte2) {
		m_current_chunk.SetLine(m_previous->Line);
		m_current_chunk.Write16(byte1, byte2);
	}

	void Parser::EmitConstant(const Common::Value& value) {

		auto constant = m_current_chunk.AddConstant(value);
		
		if (constant > UINT8_MAX) {
			throw Report("Too many constants in one chunk.");
		}
		
		Emit16(VM::OpCode::Constant, constant);
	}

    bool Parser::Match(Token::Kind kind) {
        if (!Check(kind))
            return false;

        Advance();
        return true;
    }

    void Parser::Synchronize() {

        m_panic_mode = false;

        while (m_current->Type != Token::Kind::TkEOF) {
            if (m_previous->Type == Token::Kind::Semicolon) return;
            switch (m_current->Type) {
                case Token::Kind::Class:
                case Token::Kind::Func:
                case Token::Kind::Let:
                case Token::Kind::For:
                case Token::Kind::If:
                case Token::Kind::While:
                case Token::Kind::Return:
                    return;
                default:;
            }

            Advance();
        }
    }

    Parser::Rule Parser::Rule::Get(Token::Kind type) {
		return rules[type];
	}

	Parser::Rule::Rule(std::function<void(Parser&)> prefix, std::function<void(Parser&)> infix, Precedence precedence)
		: prefix(std::move(prefix)), infix(std::move(infix)), precedence(precedence) { }

	std::unordered_map<Token::Kind, Parser::Rule> Parser::Rule::rules = {
		{Token::Kind::OpenParenthesis,  Rule(&Parser::Grouping,		nullptr,	Precedence::NONE)},
		{Token::Kind::CloseParenthesis, Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::OpenBracket,		Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::CloseBracket,		Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Comma,			Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Dot,				Rule(nullptr,					nullptr,	Precedence::NONE)},
        {Token::Kind::Not,				Rule(&Parser::Unary,			nullptr,	Precedence::NONE)},
        {Token::Kind::Minus,			Rule(&Parser::Unary,   &Parser::Binary,	Precedence::TERM)},
		{Token::Kind::Plus,				Rule(nullptr,		    &Parser::Binary,	Precedence::TERM)},
		{Token::Kind::Semicolon,		Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Slash,			Rule(nullptr,			&Parser::Binary,	Precedence::FACTOR)},
		{Token::Kind::Star,				Rule(nullptr,			&Parser::Binary,	Precedence::FACTOR)},
		{Token::Kind::Assign,			Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::NotEqual,			Rule(nullptr,			&Parser::Binary,	Precedence::EQUALITY)},
		{Token::Kind::Equal,			Rule(nullptr,			&Parser::Binary,	Precedence::COMPARISON)},
		{Token::Kind::Greater,			Rule(nullptr,			&Parser::Binary,	Precedence::COMPARISON)},
		{Token::Kind::GreaterEqual,		Rule(nullptr,			&Parser::Binary,	Precedence::COMPARISON)},
		{Token::Kind::Less,				Rule(nullptr,			&Parser::Binary,	Precedence::COMPARISON)},
		{Token::Kind::LessEqual,		Rule(nullptr,			&Parser::Binary,	Precedence::COMPARISON)},
		{Token::Kind::Identifier,		Rule(&Parser::Let,				nullptr,	Precedence::NONE)},
		{Token::Kind::String,			Rule(&Parser::String,			nullptr,	Precedence::NONE)},
		{Token::Kind::Number,			Rule(&Parser::Number,			nullptr,	Precedence::NONE)},
		{Token::Kind::LogicalAnd,		Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Else,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::False,			Rule(&Parser::Literal,		    nullptr,	Precedence::NONE)},
        {Token::Kind::True,				Rule(&Parser::Literal,		    nullptr,	Precedence::NONE)},
        {Token::Kind::Null,				Rule(&Parser::Literal,		    nullptr,	Precedence::NONE)},
        {Token::Kind::For,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Func,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::If,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::LogicalOr,		Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Return,			Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::Let,				Rule(nullptr,					nullptr,	Precedence::NONE)},
		{Token::Kind::TkEOF,			Rule(nullptr,					nullptr,	Precedence::NONE)},
	};

}
