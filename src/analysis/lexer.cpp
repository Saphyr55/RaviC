#include <iostream>
#include <cctype>

#include "analysis/lexer.hpp"


namespace Analysis {

    Lexer::Lexer(const std::string_view text) :
        m_tokens(),
        m_text(text),
        m_col(0),
        m_line(0),
        m_position(0) { }

    std::deque<RToken>& Lexer::Scan() {
        
        while (!IsAtEnd()) {
            m_start = m_position;
            NextToken();
        }

        m_tokens.push_back(
            std::make_shared<Token>(
                Token::Kind::TkEOF,
                nullptr,
                "\0",
                m_line,
                m_col
            )
        );

        return m_tokens;
    }

    std::deque<Ref<Token>>& Lexer::GetDeque() {
    
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

    Ref<Token> Lexer::PeekToken() {

        m_start = m_position;
        NextToken();
        
        if (m_tokens.empty())
        {
            return std::make_shared<Token>(
                Token::Kind::TkEOF, nullptr,
                "\0", m_line, m_col
            );
        }

        return m_tokens.back();
    }

    Ref<Token> Lexer::PollToken() {
        Ref<Token> tk = PeekToken();
        m_tokens.pop_back();
        return tk;
    }

    void Lexer::AddToken(Token::Kind kind, void* value) {
        m_current_token = std::make_shared<Token>(
            kind, value,
            m_text.substr(m_start, m_position - m_start),
            m_line, m_col
        );
		m_tokens.push_back(m_current_token);
    }

    Ref<Token> Lexer::GetCurrentTk() {
        return m_current_token;
    }

    bool Lexer::Match(const char expected) {
        if (IsAtEnd()) {
            return false;
        }
        if (m_text[m_position] != expected) {
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
        else Report("Unexpected character");
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
            Report("Unterminated string");
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
        std::cerr << "Error: " << message << " at [" << (m_line + 1) << "," << (m_col + 1) << "].\n";
    }

    std::string Token::ToString(Kind kind)
    {
        switch (kind)
        {
        case Token::Kind::TkEOF:
            return "TkEOF";
        case Token::Kind::Func:
            return "Func";
        case Token::Kind::Number:
            return "Number";
        case Token::Kind::Let:
            return "Let";
        case Token::Kind::Mut:
            return "Mut";
        case Token::Kind::Return:
            return "Return";
        case Token::Kind::Identifier:
            return "Identifier";
        case Token::Kind::True:
            return "True";
        case Token::Kind::False:
            return "False";
        case Token::Kind::Struct:
            return "Struct";
        case Token::Kind::Class:
            return "Class";
        case Token::Kind::While:
            return "While";
        case Token::Kind::For:
            return "For";
        case Token::Kind::If:
            return "If";
        case Token::Kind::Else:
            return "Else";
        case Token::Kind::Namespace:
            return "Namespace";
        case Token::Kind::Less:
            return "Less";
        case Token::Kind::LessEqual:
            return "LessEqual";
        case Token::Kind::Greater:
            return "Greater";
        case Token::Kind::GreaterEqual:
            return "GreaterEqual";
        case Token::Kind::Semicolon:
            return "Semicolon";
        case Token::Kind::Colon:
            return "Colon";
        case Token::Kind::Dot:
            return "Dot";
        case Token::Kind::Arrow:
            return "Arrow";
        case Token::Kind::OpenBracket:
            return "OpenBracket";
        case Token::Kind::CloseBracket:
            return "CloseBracket";
        case Token::Kind::OpenParenthesis:
            return "OpenParenthesis";
        case Token::Kind::CloseParenthesis:
            return "CloseParenthesis";
        case Token::Kind::BinaryAnd:
            return "BinaryAnd";
        case Token::Kind::LogicalAnd:
            return "LogicalAnd";
        case Token::Kind::BinaryOr:
            return "BinaryOr";
        case Token::Kind::LogicalOr:
            return "LogicalOr";
        case Token::Kind::Comma:
            return "Comma";
        case Token::Kind::Assign:
            return "Assign";
        case Token::Kind::Not:
            return "Not";
        case Token::Kind::NotEqual:
            return "NotEqual";
        case Token::Kind::Equal:
            return "Equal";
        case Token::Kind::Minus:
            return "Minus";
        case Token::Kind::Plus:
            return "Plus";
        case Token::Kind::Star:
            return "Star";
        case Token::Kind::Slash:
            return "Slash";
        case Token::Kind::String:
            return "String";
        default:
            return "undefined";
        }
    }

} 
