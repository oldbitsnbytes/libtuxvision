/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all Code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once

#include <tuxvision/utility/sys/sys.h>

namespace tux::lang
{
namespace type
{
    using value = integers::u64;
    static constexpr value Void        = 0x0000000000000000;
    static constexpr value Number      = 0x0000000000000001;
    static constexpr value Text        = 0x0000000000000002;
    static constexpr value VoidPtr     = 0x0000000000000004;
    static constexpr value Any         = 0x0000000000000008;
    static constexpr value Leaf        = 0x0000000000000010;
    static constexpr value Sign        = 0x0000000000000020;
    static constexpr value I8          = 0x0000000000000040;
    static constexpr value I16         = 0x0000000000000080;  // 0x7F41
    static constexpr value I32         = 0x0000000000000100;
    static constexpr value I64         = 0x0000000000000200;
    static constexpr value U8          = 0x0000000000000400;
    static constexpr value U16         = 0x0000000000000800;
    static constexpr value U32         = 0x0000000000001000;
    static constexpr value U64         = 0x0000000000002000;
    static constexpr value Float       = 0x0000000000004000;  // real: f32, f64, f128... ^^
    static constexpr value Var         = 0x0000000000008000;  // Ne pas oublier d'ajouter ce type a toutes "variables"
    static constexpr value Bloc        = 0x0000000000010000;
    static constexpr value Pipe        = 0x0000000000020000;
    static constexpr value Keyword     = 0x0000000000040000;
    static constexpr value Unary       = 0x0000000000080000;
    static constexpr value Prefix      = 0x0000000000100000;
    static constexpr value Postfix     = 0x0000000000200000;
    static constexpr value Operator    = 0x0000000000400000;
    static constexpr value Binary      = 0x0000000000800000;
    static constexpr value Function    = 0x0000000001000000;
    static constexpr value FunctionPtr = 0x0000000002000000;
    static constexpr value Object      = 0x0000000004000000;
    static constexpr value Pointer     = 0x0000000008000000;
    static constexpr value Type        = 0x0000000010000000;
    static constexpr value Id          = 0x0000000020000000;
    static constexpr value Ref         = 0x0000000040000000;
    static constexpr value Punctuation = 0x0000000080000000;
    static constexpr value Assign      = 0x0000000100000000;
    static constexpr value Bool        = 0x0000000200000000;
    static constexpr value Hex         = 0x0000000400000000;
    static constexpr value OpenPair    = 0x0000000800000000;
    static constexpr value ClosePair   = 0x0000001000000000;
    static constexpr value Const       = 0x0000002000000000;
    static constexpr value Static      = 0x0000008000000000;  ///< Flag set at the parser phase when applicable.
    static constexpr value F32         = 0x0000010000000000;
    static constexpr value F64         = 0x0000020000000000;
    static constexpr value F128        = 0x0000040000000000;
    static constexpr value OctalFormat = 0x0000080000000000;
    static constexpr value BinFormat   = 0x0000100000000000;
    static constexpr value LineComment = 0x0000200000000000;
    static constexpr value BlocComment = 0x0000400000000000;
    static constexpr value Literal     = 0x0000800000000000;  ///< Literal types { number, string/Text };
    static constexpr value Procedure   = 0x0001000000000000;
    //...
    static constexpr value Null        = 0x1000000000000000;
    static constexpr value Integer     = type::I8 | type::I16 | type::I32 | type::I64;
    static constexpr value Natural     = type::U8 | type::U16 | type::U32 | type::U64;
    static constexpr value Unsigned    = Natural;
};

enum class mnemonic : integers::u8
{
    Null             ,
    LeftShift        ,
    Radical          ,
    Exponent         ,
    RightShift       ,
    Decr             ,
    Incr             ,
    Address          ,
    AssignAdd        ,
    AssignSub        ,
    AssignMul        ,
    AssignDiv        ,
    AssignMod        ,
    AssignAnd        ,
    AssignOr         ,
    AssignXor        ,
    AssignC1         ,
    AssignLeftShift  ,
    AssignRightShift ,
    Deref            ,
    LessEq           ,
    GreaterEq        ,
    Equal            ,
    NotEqual         ,
    Add              ,
    Sub              ,
    Mul              ,
    Indirection      ,
    CommentCpp       ,
    Modulo           ,
    LessThan         ,
    GreaterThan      ,
    Assign           ,
    BinaryAnd        ,
    BinaryOr         ,
    BinaryXor        ,
    C1               ,
    C2               ,
    BinaryNot        ,
    LogicalAnd       ,
    LogicalOr        ,
    OpenAbsOp        ,
    CloseAbsOp       ,
    OpenPar          ,
    ClosePar         ,
    OpenIndex        ,
    CloseIndex       ,
    OpenBrace        ,
    CloseBrace       ,
    BeginComment     ,
    EndComment       ,
    Div              ,
    Comma            ,
    Scope            ,
    Semicolon        ,
    Colon            ,
    Range            ,
    Factorial        ,
    Positive         ,
    Negative         ,
    SQuote           ,
    DQuote           ,
    Ternary          ,
    Hash             ,
    Eos              ,
    Dot              ,
    Return           ,
    Function         ,
    Procedure        ,
    If               ,
    Pi               ,
    Number           ,
    U8               ,
    U16              ,
    U32              ,
    U64              ,
    I8               ,
    I16              ,
    I32              ,
    I64              ,
    Float            ,
    String           ,
    Then             ,
    Else             ,
    Const            ,
    Include          ,
    Amu              ,
    AddressOf        ,
    Prime            ,
    Do               ,
    While            ,
    For              ,
    Until            ,
    Repeat           ,
    Switch           ,
    Case             ,
    Type             ,
    LowHex           ,
    UpHex            ,
    Cosinus          ,
    ArcCosinus       ,
    Tangent          ,
    ArcTangent       ,
    Sinus            ,
    ArcSinus         ,
    Object           ,
    Static           ,
    This             ,
    UnShadow         ,
    Catch            ,
    Throw            ,
    Try              ,
    Noop             ,
    LineComment      ,
    BlocComment      ,
    Identifier       ,
    Numeric          ,
    Void
};


namespace lexeme
{

using Value = const char*;//std::string_view;
constexpr Value Void          = "void";
constexpr Value Null          = "null";
constexpr Value LeftShift     = "<<";
constexpr Value Radical       = "^/";
constexpr Value Exponent      = "^";
constexpr Value RightShift    = ">>";
constexpr Value Decr          = "--";
constexpr Value Incr          = "++";
constexpr Value AssignAdd     = "+=";
constexpr Value AssignSub     = "-=";
constexpr Value AssignMul     = "*=";
constexpr Value AssignDiv     = "/=";
constexpr Value AssignMod     = "%=";
constexpr Value AssignAnd     = "&=";
constexpr Value AssignOr      = "|=";
constexpr Value AssignXor     = "><=";
constexpr Value AssignC1      = "`="; // Insérer AssignCompletment à 2 =>  { ``=  }
constexpr Value AssignLShift  = "<<=";
constexpr Value AssignRShift  = ">>=";
constexpr Value Deref         = "->";
constexpr Value LessEqual     = "<=";
constexpr Value GreaterEqual  = ">=";
constexpr Value Equal         = "==";
constexpr Value NotEqual      = "!=";
constexpr Value NotEqualAlt   = "<>";// != <>
constexpr Value Addition      = "+";
constexpr Value Sub           = "-";
constexpr Value Multiply      = "*";
constexpr Value Indirection   = "*";
constexpr Value CommentCpp    = "//";
constexpr Value Modulo        = "%";
constexpr Value Xor           = "><";
constexpr Value LessThan      = "<";
constexpr Value GreaterThan   = ">";
constexpr Value Assign        = "=";
constexpr Value BinaryAnd     = "&";
constexpr Value BinaryOr      = "|";
constexpr Value C1            = "`"; ///< compl&eacute;ment &agrave; 1
constexpr Value C2            = "``";///< compl&eacute;ment &agrave; 2
constexpr Value Not           = "!";
constexpr Value BoolAnd       = "&&";
constexpr Value BoolOr        = "||";
constexpr Value AbsBegin      = "|<";// |< absolute value expr >|
constexpr Value AbsEnd        = ">|";
constexpr Value OpenPar       = "(";
constexpr Value ClosePar      = ")";
constexpr Value OpenIndex     = "[";
constexpr Value CloseIndex    = "]";
constexpr Value BraceBegin    = "{";
constexpr Value BraceEnd      = "}";
constexpr Value CommentBegin  = "/*";
constexpr Value CommentEnd    = "*/";
constexpr Value Division      = "/";
constexpr Value Comma         = ",";
constexpr Value Scope         = "::";
constexpr Value Semicolon     = ";";
constexpr Value Colon         = ":";
constexpr Value Range         = "..";
constexpr Value Factorial     = "!";
constexpr Value Positive      = "+";
constexpr Value Negative      = "-";
constexpr Value SQuote        = "'";  // '
constexpr Value DQuote        = "\""; // "
constexpr Value Ternary       = "?";  // ? :
constexpr Value Hash          = "#";  // #
constexpr Value Eos           = "$";
constexpr Value Dot           = ".";
constexpr Value Return        = "return";
constexpr Value Function      = "function";
constexpr Value Procedure     = "procedure";
constexpr Value If            = "if";
constexpr Value Pi            = "pi";
constexpr Value Number        = "number";
constexpr Value U8            = "u8";
constexpr Value U16           = "u16";
constexpr Value U32           = "u32";
constexpr Value U64           = "u64";
constexpr Value I8            = "i8";
constexpr Value I16           = "i16";
constexpr Value I32           = "i32";
constexpr Value I64           = "i64";
constexpr Value Float         = "real";
constexpr Value String        = "string";
constexpr Value Then          = "then";
constexpr Value Else          = "else";
constexpr Value Const         = "const";
constexpr Value Include       = "include";
constexpr Value Amu           = "amu";
constexpr Value Address       = "@";
constexpr Value Prime         = "`"; ///@todo change...
constexpr Value Do            = "do";
constexpr Value While         = "while";
constexpr Value For           = "for";
constexpr Value Until         = "until";
constexpr Value Repeat        = "repeat";
constexpr Value Switch        = "switch";
constexpr Value Case          = "case";
constexpr Value Type          = "type";
constexpr Value LowHex        = "0x";
constexpr Value UpHex         = "0X";
constexpr Value Cosinus       = "cos";
constexpr Value ArcCosinus    = "acos";
constexpr Value Tangent       = "tan";// tan(4*a*m)  - sin(4ac) sina
constexpr Value ArcTangent    = "atan";
constexpr Value Sinus         = "sin";
constexpr Value ArcSinus      = "asin";
constexpr Value Object        = "object";
constexpr Value Static        = "static";
constexpr Value This          = "me";
constexpr Value Unshadow      = ".::"; // If a local variable shadows a higher scoped variable, then this operator will address the variable from within the first bloc where the given variable is scoped.
constexpr Value Catch         = "catch";
constexpr Value Throw         = "throw";
constexpr Value Try           = "try";
constexpr Value LineComment   = "//";
constexpr Value BlocComment   = "/*";
constexpr Value Identifier    = "Identifier/Symbol";
constexpr Value Numeric       = "Literal Number";

} // namespace lexem


namespace operand
{
using Index = uint8_t;
constexpr Index Scope      = 0;
constexpr Index Assign_rhs = 1; //  A = B = ER!
constexpr Index Deref      = 2;
constexpr Index Prefix     = 3;
constexpr Index Postfix    = 3;
constexpr Index Unary      = 3;
constexpr Index Pair       = 5;
constexpr Index Exponent   = 6;
constexpr Index Bits       = 7;
constexpr Index Product    = 8;
constexpr Index Addition   = 9;
constexpr Index Shift      = 10;
constexpr Index Equality   = 11;
constexpr Index Comp       = 12;
constexpr Index Bool_and   = 13;
constexpr Index Bool_xor   = 14;
constexpr Index Bool_or    = 15;
constexpr Index Boolean    = 16;
constexpr Index Ternary    = 17;
constexpr Index Comma      = 18;
constexpr Index Identifier = 19;
constexpr Index Assign     = 20; // a + b * m = 0 - 1 ( assign is right hand side association so the operator has lowest priority)
constexpr Index Noop_      = 21;

} // namespace operand
} // namespace tux::lang;


namespace tux::lang
{
struct component
{
    static std::map<lang::type::value, const char*>
                            type_enums;
    // static std::map<lang::mnemonic, std::string_view>
    //                         mnemonic_names;

    static std::string      type_name(lang::type::value lex_type);
    static type::value      type_enum(const std::string& lex_type_name);
    static ui::color::code  fg_color_attr(mnemonic m);
    static ui::color::code  bg_color_attr(mnemonic m);
};


} // end-namespace tux;
