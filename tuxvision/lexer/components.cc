#include "tuxvision/lexer/components.h"



namespace tux::lang
{
std::map<lang::type::value,     const char*> component::type_enums = {
    {lang::type::Void        ,"Void"       },
    {lang::type::Number      ,"Number"     },
    {lang::type::Text        ,"Text"       },
    {lang::type::VoidPtr     ,"VoidPtr"    },
    {lang::type::Any         ,"Any"        },
    {lang::type::Leaf        ,"Leaf"       },
    {lang::type::Sign        ,"Sign"       },
    {lang::type::I8          ,"I8"         },
    {lang::type::I16         ,"I16"        },
    {lang::type::I32         ,"I32"        },
    {lang::type::I64         ,"I64"        },
    {lang::type::U8          ,"U8"         },
    {lang::type::U16         ,"U16"        },
    {lang::type::U32         ,"U32"        },
    {lang::type::U64         ,"U64"        },
    {lang::type::Float       ,"Float"      },
    {lang::type::Var         ,"Var"        },
    {lang::type::Bloc        ,"Bloc"       },
    {lang::type::Pipe        ,"Pipe"       },
    {lang::type::Keyword     ,"Keyword"    },
    {lang::type::Unary       ,"Unary"      },
    {lang::type::Prefix      ,"Prefix"     },
    {lang::type::Postfix     ,"Postfix"    },
    {lang::type::Operator    ,"Operator"   },
    {lang::type::Binary      ,"Binary"     },
    {lang::type::Function    ,"Function"   },
    {lang::type::FunctionPtr ,"FunctionPtr"},
    {lang::type::Object      ,"Object"     },
    {lang::type::Pointer     ,"Pointer"    },
    {lang::type::Type        ,"Type"       },
    {lang::type::Id          ,"Id"         },
    {lang::type::Ref         ,"Ref"        },
    {lang::type::Punctuation ,"Punctuation"},
    {lang::type::Assign      ,"Assign"     },
    {lang::type::Bool        ,"Bool"       },
    {lang::type::Hex         ,"Hex"        },
    {lang::type::OpenPair    ,"OpenPair"   },
    {lang::type::ClosePair   ,"ClosePair"  },
    {lang::type::Const       ,"Const"      },
    {lang::type::Static      ,"Static"     },
    {lang::type::F32         ,"F32"        },
    {lang::type::F64         ,"F64"        },
    {lang::type::F128        ,"F128"       },
    {lang::type::OctalFormat ,"OctalFormat"},
    {lang::type::BinFormat   ,"BinFormat"  },
    {lang::type::LineComment ,"LineComment"},
    {lang::type::BlocComment ,"BlocComment"},
    {lang::type::Literal     ,"Literal"    },
    {lang::type::Null        ,"Null"       },
    {lang::type::Integer     ,"Integer"    },
    {lang::type::Unsigned    ,"Unsigned"   },
    {lang::type::Procedure   ,"Procedure"  },
};

//static std::map<lex::Mnemonic::T, std::string_view> MnemonicEnums;

std::map<lang::mnemonic, ui::color::pair> mnemonics_colors_attr = {
    {mnemonic::Null            ,{ui::color::white,ui::color::r }} ,
    {mnemonic::LeftShift       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Radical         ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Exponent        ,{ui::color::white,ui::color::r }} ,
    {mnemonic::RightShift      ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Decr            ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Incr            ,{ui::color::white,ui::color::r }} ,
    {mnemonic::AssignAdd       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::AssignSub       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::AssignMul       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::AssignDiv       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::AssignMod       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::AssignAnd       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::AssignOr        ,{ui::color::white,ui::color::r }} ,
    {mnemonic::AssignXor       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::AssignC1        ,{ui::color::white,ui::color::r }} ,
    {mnemonic::AssignLeftShift ,{ui::color::white,ui::color::r }} ,
    {mnemonic::AssignRightShift,{ui::color::white,ui::color::r }} ,
    {mnemonic::Deref           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::LessEq          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::GreaterEq       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Equal           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::NotEqual        ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Add             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Sub             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Mul             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Indirection     ,{ui::color::white,ui::color::r }} ,
    {mnemonic::CommentCpp      ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Modulo          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::LessThan        ,{ui::color::white,ui::color::r }} ,
    {mnemonic::GreaterThan     ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Assign          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::BinaryAnd       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::BinaryOr        ,{ui::color::white,ui::color::r }} ,
    {mnemonic::BinaryXor       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::C1              ,{ui::color::white,ui::color::r }} ,
    {mnemonic::C2              ,{ui::color::white,ui::color::r }} ,
    {mnemonic::BinaryNot       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::LogicalAnd      ,{ui::color::white,ui::color::r }} ,
    {mnemonic::LogicalOr       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::OpenAbsOp       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::CloseAbsOp      ,{ui::color::white,ui::color::r }} ,
    {mnemonic::OpenPar         ,{ui::color::white,ui::color::r }} ,
    {mnemonic::ClosePar        ,{ui::color::white,ui::color::r }} ,
    {mnemonic::OpenIndex       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::CloseIndex      ,{ui::color::white,ui::color::r }} ,
    {mnemonic::OpenBrace       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::CloseBrace      ,{ui::color::white,ui::color::r }} ,
    {mnemonic::BeginComment    ,{ui::color::white,ui::color::r }} ,
    {mnemonic::EndComment      ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Div             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Comma           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Scope           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Semicolon       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Colon           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Range           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Factorial       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Positive        ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Negative        ,{ui::color::white,ui::color::r }} ,
    {mnemonic::SQuote          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::DQuote          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Ternary         ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Hash            ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Eos             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Dot             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Return          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::If              ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Pi              ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Number          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::U8              ,{ui::color::white,ui::color::r }} ,
    {mnemonic::U16             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::U32             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::U64             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::I8              ,{ui::color::white,ui::color::r }} ,
    {mnemonic::I16             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::I32             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::I64             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Float           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::String          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Then            ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Else            ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Const           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Include         ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Amu             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::AddressOf       ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Prime           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Do              ,{ui::color::white,ui::color::r }} ,
    {mnemonic::While           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::For             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Until           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Repeat          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Switch          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Case            ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Type            ,{ui::color::white,ui::color::r }} ,
    {mnemonic::LowHex          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::UpHex           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Cosinus         ,{ui::color::white,ui::color::r }} ,
    {mnemonic::ArcCosinus      ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Tangent         ,{ui::color::white,ui::color::r }} ,
    {mnemonic::ArcTangent      ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Sinus           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::ArcSinus        ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Object          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Static          ,{ui::color::white,ui::color::r }} ,
    {mnemonic::This            ,{ui::color::white,ui::color::r }} ,
    {mnemonic::UnShadow        ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Catch           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Throw           ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Try             ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Noop            ,{ui::color::white,ui::color::r }} ,
    {mnemonic::LineComment     ,{ui::color::white,ui::color::r }} ,
    {mnemonic::BlocComment     ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Identifier      ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Numeric         ,{ui::color::white,ui::color::r }} ,
    {mnemonic::Function        ,{ui::color::fuchsia,ui::color::r }},
    {mnemonic::Procedure       ,{ui::color::yellow6,ui::color::r }},
    {mnemonic::Void            ,{ui::color::red,ui::color::r }},
};


/**
 * Converts a `lang::type` enum value into its string representation.
 * The method iterates through each bit of the provided `lang::type` value,
 * maps the corresponding bit-flagged enum to its string equivalent using
 * `component::type_enums`, and concatenates the results using the `|` delimiter.
 *
 * @param lex_type A `lang::type` value containing bit-encoded flags representing types.
 * @return A string representation of the combined `lang::type` flags.
 *         If no flags are set, an empty string is returned.
 */
std::string  component::type_name(lang::type::value lex_type)
{
    tux::string str;
    u64 bits = lex_type;
    for (u64 n=0;n<64;n++)
    {
        if(u64 bit = static_cast<u64>(1)<<n; bit & bits)
        {
            const auto it = component::type_enums[bit];
            if (!str.empty()) str << "|";
            if (it)
                str << it;
            bits &= ~bit;
            if (bits == 0)
                break;
        }
    }
    return str();
}


/**
 * Converts a lexeme type name into a corresponding bit-encoded `lang::type` enum.
 * The method splits the given `lex_type_name` string using the delimiter `:`
 * and maps each token to its corresponding enum value from `lang_component::type_enums`.
 * The resulting bitwise combination of the mapped values is returned as a `lang::type`.
 *
 * @param lex_type_name The string representing the lexeme type name,
 *                      where tokens are delimited by `:`.
 * @return A `lang::type` enum bit-encoded value representing the combination of resolved types.
 *         If the `lex_type_name` cannot be parsed or mapped, returns `lang::type::Null`.
 */
lang::type::value   component::type_enum(const std::string& lex_type_name)
{
    tux::string str = lex_type_name;
    tux::string::word::list list;
    integers::u64 typebits = 0;

    if(!str.words(list, false,":")) return lang::type::Null;

    for(auto S: list)
    {
        for(auto const& [k,v] : component::type_enums)
        {
            if(std::strcmp(v, (*S).c_str()) == 0) typebits |= k;
        }
    }
    return typebits;
}


ui::color::code component::fg_color_attr(mnemonic m)
{
    return mnemonics_colors_attr[m].fg;
}

ui::color::code component::bg_color_attr(mnemonic m)
{
    return mnemonics_colors_attr[m].bg;
}


} // end namespace tux::lang;
