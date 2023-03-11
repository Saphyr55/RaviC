#include <iostream>
#include <cctype>

#include "lexer.hpp"

Lexer::Lexer(const std::string_view text) :
	m_tokens(),
    m_text(text),
    m_col(0),
    m_line(0),
    m_position(0) {

}

std::vector<RToken> Lexer::Scan() {
    
    while (!IsAtEnd()) {
        m_start = m_position;
        NextToken();
    }

	m_tokens.push_back(std::make_shared<Token>(
        Token::Kind::TkEOF,
        nullptr,
        "\0",
		m_line,
		m_col
	));
    return m_tokens;
}

void Lexer::NextToken() {
    const char c = Advance();
    switch (c) {

	case Less:         
        AddToken(Match(Equal) ? Token::Kind::LessEqual: Token::Kind::Less);
		break;

	case Greater:         
        AddToken(Match(Equal) ? Token::Kind::GreaterEqual : Token::Kind::Greater);
        break;

    case Comma: 
        AddToken(Token::Kind::Comma);
        break;

    case BackQuote: 
        CreateStringToken(BackQuote);
        break;

    case DoubleQuote: 
        CreateStringToken(DoubleQuote);
        break;

    case OpenParenthesis: 
        AddToken(Token::Kind::OpenParenthesis);
        break;

    case CloseParenthesis: 
        AddToken(Token::Kind::CloseParenthesis);
        break;
    
    case Bang: 
        AddToken(Match(Equal) ? Token::Kind::NotEqual : Token::Kind::Not);
        break;

    case Equal: 
        AddToken(Match(Equal) ? Token::Kind::Equal : Token::Kind::Assign); 
        break;

    case Ampersand: 
        AddToken(Match(Ampersand) ? Token::Kind::LogicalAnd : Token::Kind::BinaryAnd);
        break;

    case Pipeline: 
        AddToken(Match(Pipeline) ? Token::Kind::LogicalOr : Token::Kind::BinaryOr);
        break;

    case OpenBrackets:
        AddToken(Token::Kind::OpenBracket);
        break;
    
    case CloseBrackets: 
        AddToken(Token::Kind::CloseBracket);
        break;

    case Dot: 
        AddToken(Token::Kind::Dot);
        break;

    case Colon: 
        AddToken(Token::Kind::Colon);
        break;
    
    case Semicolon: 
        AddToken(Token::Kind::Semicolon);
        break;

    case MinusOp: 
        AddToken(Match(Greater) ? Token::Kind::Arrow : Token::Kind::Minus);
        break;

    case PlusOp: 
        AddToken(Token::Kind::Plus);
        break;

    case StartOp: 
        AddToken(Token::Kind::Star);
        break;
    
    case SlashOp: 
        AddSlashToken();
        break;

    case BackslashN: 
        m_line++; 
        m_col = 0;
        break;

    case Space: 
        break;

    case BackslashR: 
        break;
    
    case BackslashT:
        break;
    
    default: 
        AddDefaultToken(c);    
        break;
    }
}

void Lexer::AddToken(Token::Kind kind, void* value) {
    std::cout << Token::ToString(kind) << "\n";
	m_tokens.push_back(std::make_shared<Token>(
        kind,
		value,
		m_text.substr(m_start, m_position - m_start),
		m_line,
		m_col
    ));
}

bool Lexer::Match(const char expected) {
    if (IsAtEnd()) {
        return false;
    }
    if (m_text[m_position] == expected) {
        return false;
    }
    Next();
    return true;
}

char Lexer::Peek() {
    if (IsAtEnd()) {
        return '\0';
    }
    return m_text[m_position];
}

void Lexer::AddDefaultToken(const char c) {
	if (std::isdigit(c)) AddNumberToken();
	else if (std::isalpha(c)) AddIdentifierToken();
	else Report("Unexpected character.");
}

void Lexer::AddNumberToken() {
	while (std::isdigit(Peek())) Next();
	if (Peek() == Dot && std::isdigit(PeekNext())) {
		Next();
		while (std::isdigit(Peek())) Next();
	}
    double x = std::stod(m_text.substr(m_start, m_position - m_start));
	AddToken(Token::Kind::Number, &x);
}

void Lexer::AddIdentifierToken() {
    while (std::isalnum(Peek())) Next();
    try {
		AddToken(Words.at(m_text.substr(m_start, m_position - m_start)));
    }
    catch (const std::exception&) {
        AddToken(Token::Kind::Identifier);
    }
}

void Lexer::CreateStringToken(const char q) {
	while (Peek() != q && !IsAtEnd()) {
		if (Peek() == '\n') m_line++;
		Advance();
	}
	if (IsAtEnd()) {
		Report("Unterminated string.");
		return;
	}
	Next();
	std::string value = m_text.substr(m_start + 1, (m_position - 1) - (m_start + 1));
	AddToken(Token::Kind::String, &value);
}

void Lexer::AddSlashToken() {
	if (Match(SlashOp)) {
		while (Peek() != '\n' && !IsAtEnd())
			Next();
	}
    else {
		AddToken(Token::Kind::Slash);
    }
}

char Lexer::PeekNext() {
    if (m_position + 1 >= m_text.size()) return '\0';
    return m_text[m_position + 1];
}

char Lexer::Advance() {
    return m_text[Next()];
}

std::size_t Lexer::Next() {
    m_col++;
    return m_position++;
}

bool Lexer::IsAtEnd() {
    return m_position >= m_text.size();
}

void Lexer::Report(std::string message) {
	std::cout << "Error:" << message << " at [" << (m_line + 1) << "," << (m_col + 1) << "].\n";
}

std::string Token::ToString(Kind kind)
{
    switch (kind)
    {
    case Token::Kind::TkEOF:
        break;
    case Token::Kind::Func:
        break;
    case Token::Kind::Number:
        break;
    case Token::Kind::Let:
        break;
    case Token::Kind::Mut:
        break;
    case Token::Kind::Return:
        break;
    case Token::Kind::Identifier:
        break;
    case Token::Kind::True:
        break;
    case Token::Kind::False:
        break;
    case Token::Kind::Struct:
        break;
    case Token::Kind::Class:
        break;
    case Token::Kind::While:
        break;
    case Token::Kind::For:
        break;
    case Token::Kind::If:
        break;
    case Token::Kind::Else:
        break;
    case Token::Kind::Namespace:
        break;
    case Token::Kind::Less:
        break;
    case Token::Kind::LessEqual:
        break;
    case Token::Kind::Greater:
        break;
    case Token::Kind::GreaterEqual:
        break;
    case Token::Kind::Semicolon:
        break;
    case Token::Kind::Colon:
        break;
    case Token::Kind::Dot:
        break;
    case Token::Kind::Arrow:
        break;
    case Token::Kind::OpenBracket:
        break;
    case Token::Kind::CloseBracket:
        break;
    case Token::Kind::OpenParenthesis:
        break;
    case Token::Kind::CloseParenthesis:
        break;
    case Token::Kind::BinaryAnd:
        break;
    case Token::Kind::LogicalAnd:
        break;
    case Token::Kind::BinaryOr:
        break;
    case Token::Kind::LogicalOr:
        break;
    case Token::Kind::Comma:
        break;
    case Token::Kind::Assign:
        break;
    case Token::Kind::Not:
        break;
    case Token::Kind::NotEqual:
        break;
    case Token::Kind::Equal:
        break;
    case Token::Kind::Minus:
        break;
    case Token::Kind::Plus:
        break;
    case Token::Kind::Star:
        break;
    case Token::Kind::Slash:
        return "/";
    case Token::Kind::String:
        return "string";
    default:
        break;
    }
}
