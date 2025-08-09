#include <tuxvision/est/ynod.h>



namespace tux::lang::est
{


std::map<lang::mnemonic, ynod::operator_fn> ynod::operations_map{
    {lang::mnemonic::LeftShift,     &ynod::LeftShift     },
    {lang::mnemonic::Radical,       &ynod::Radical       },
    {lang::mnemonic::Exponent,      &ynod::Exponent      },
    {lang::mnemonic::RightShift,    &ynod::RightShift    },
    {lang::mnemonic::Decr,          &ynod::Decr          },
    {lang::mnemonic::Incr,          &ynod::Incr          },
    {lang::mnemonic::Address,       &ynod::Address       },
    {lang::mnemonic::AssignAdd,     &ynod::AssignAdd     },
    {lang::mnemonic::AssignSub,     &ynod::AssignSub     },
    {lang::mnemonic::AssignMul,     &ynod::AssignMul     },
    {lang::mnemonic::AssignDiv,     &ynod::AssignDiv     },
    {lang::mnemonic::AssignMod,     &ynod::AssignMod     },
    {lang::mnemonic::AssignAnd,     &ynod::AssignAnd     },
    {lang::mnemonic::AssignOr,      &ynod::AssignOr      },
    {lang::mnemonic::AssignXor,     &ynod::AssignXor     },
    {lang::mnemonic::AssignC1,      &ynod::AssignC1      },
    {lang::mnemonic::AssignLeftShift, &ynod::AssignLeftShift},
    {lang::mnemonic::AssignRightShift, &ynod::AssignRightShift},
    {lang::mnemonic::Deref,         &ynod::Deref         },
    {lang::mnemonic::LessEq,        &ynod::LessEq        },
    {lang::mnemonic::GreaterEq,     &ynod::GreaterEq     },
    {lang::mnemonic::Equal,         &ynod::Equal         },
    {lang::mnemonic::NotEqual,      &ynod::NotEqual      },
    {lang::mnemonic::Add,           &ynod::Add           },
    {lang::mnemonic::Sub,           &ynod::Sub           },
    {lang::mnemonic::Mul,           &ynod::Mul           },
    {lang::mnemonic::Indirection,   &ynod::Indirection   },
    {lang::mnemonic::Modulo,        &ynod::Modulo        },
    {lang::mnemonic::LessThan,      &ynod::LessThan      },
    {lang::mnemonic::GreaterThan,   &ynod::GreaterThan   },
    {lang::mnemonic::Assign,        &ynod::Assign        },
    {lang::mnemonic::BinaryAnd,     &ynod::BinaryAnd     },
    {lang::mnemonic::BinaryOr,      &ynod::BinaryOr      },
    {lang::mnemonic::BinaryXor,     &ynod::BinaryXor     },
    {lang::mnemonic::C1,            &ynod::C1            },
    {lang::mnemonic::C2,            &ynod::C2            },
    {lang::mnemonic::BinaryNot,     &ynod::BinaryNot     },
    {lang::mnemonic::LogicalAnd,    &ynod::LogicalAnd    },
    {lang::mnemonic::LogicalOr,     &ynod::LogicalOr     },
    {lang::mnemonic::Div,           &ynod::Div           },
    {lang::mnemonic::Comma,         &ynod::Comma         },
    {lang::mnemonic::Factorial,     &ynod::Factorial     },
    {lang::mnemonic::Positive,      &ynod::Positive      },
    {lang::mnemonic::Negative,      &ynod::Negative      },
    {lang::mnemonic::Pi,            &ynod::Pi            },
    {lang::mnemonic::Number,        &ynod::Number        },
    {lang::mnemonic::U8,            &ynod::U8            },
    {lang::mnemonic::U16,           &ynod::U16           },
    {lang::mnemonic::U32,           &ynod::U32           },
    {lang::mnemonic::U64,           &ynod::U64           },
    {lang::mnemonic::I8,            &ynod::I8            },
    {lang::mnemonic::I16,           &ynod::I16           },
    {lang::mnemonic::I32,           &ynod::I32           },
    {lang::mnemonic::I64,           &ynod::I64           },
    {lang::mnemonic::Float,         &ynod::Float         },
    {lang::mnemonic::String,        &ynod::String        },
    {lang::mnemonic::AddressOf,     &ynod::AddressOf     },
    {lang::mnemonic::Prime,         &ynod::Prime         },
    {lang::mnemonic::Cosinus,       &ynod::Cosinus       },
    {lang::mnemonic::ArcCosinus,    &ynod::ArcCosinus    },
    {lang::mnemonic::Tangent,       &ynod::Tangent       },
    {lang::mnemonic::ArcTangent,    &ynod::ArcTangent    },
    {lang::mnemonic::Sinus,         &ynod::Sinus         },
    {lang::mnemonic::ArcSinus,      &ynod::ArcSinus      },

};



ynod::ynod(ynod* parent_ynod, token_info* token, alu::shared value_ref):object(parent_ynod, "ynod")
{
    if (value_ref)
        acc = value_ref;
    else
        acc = std::make_shared<alu>();

    if (token)
        operator_ptr = ynod::operations_map[token->m];
    
}


ynod* ynod::tree_parse_input_open(ynod* parent_op, token_info* token, std::function<ynod*(token_info*)> ynod_maker_function)
{
    if(ynod_maker_function)
        return ynod_maker_function(token);

    return nullptr;
}


ynod* ynod::tree_parse_input_node(ynod* op_nod, token_info* token)
{
    return nullptr;
}


ynod* ynod::tree_parse_input_close(ynod* last_node)
{
    return nullptr;
}


ynod* ynod::tree_parse_input_binary(token_info* op_token)
{
    return nullptr;
}


ynod* ynod::tree_parse_input_prefix(token_info* op_token)
{
    return nullptr;
}


ynod* ynod::tree_parse_input_suffix(token_info* op_token)
{
    return nullptr;
}


ynod* ynod::tree_parse_input_leaf(token_info* leaftoken)
{
    return nullptr;
}


ynod* ynod::tree_set_left(ynod* operand)
{
    return nullptr;
}


ynod* ynod::tree_set_right(ynod* operand)
{
    return nullptr;
}


ynod* ynod::tree_set_op(ynod* op_token)
{
    return nullptr;
}


alu ynod::LeftShift()
{
    if((left->token->s & lang::type::Float) || (right->token->s & lang::type::Float))
    {
        *acc = 0.f;
        sys::warning(1) << left->token->name << " " << token->text() << " " << lang::component::type_name(right->token->t) << " are incompatible" ;
    }

    *acc = left->acc->number<uint64_t>() << right->acc->number<uint64_t>();
    sys::debug(1) << ui::color::cornflowerblue << " = " << ui::color::lime << (*acc)();
    return *acc;
}


alu ynod::Radical()
{
    return {};
}


alu ynod::Exponent()
{
    return {};
}


alu ynod::RightShift()
{
    return {};
}


alu ynod::Decr()
{
    return {};
}


alu ynod::Incr()
{
    return {};
}


alu ynod::Address()
{
    return {};
}


alu ynod::AssignAdd()
{
    return {};
}


alu ynod::AssignSub()
{
    return {};
}


alu ynod::AssignMul()
{
    return {};
}


alu ynod::AssignDiv()
{
    return {};
}


alu ynod::AssignMod()
{
    return {};
}


alu ynod::AssignAnd()
{
    return {};
}


alu ynod::AssignOr()
{
    return {};
}


alu ynod::AssignXor()
{
    return {};
}


alu ynod::AssignC1()
{
    return {};
}


alu ynod::AssignLeftShift()
{
    return {};
}


alu ynod::AssignRightShift()
{
    return {};
}


alu ynod::Deref()
{
    return {};
}


alu ynod::LessEq()
{
    return {};
}


alu ynod::GreaterEq()
{
    return {};
}


alu ynod::Equal()
{
    return {};
}


alu ynod::NotEqual()
{
    return {};
}


alu ynod::Add()
{
    return {};
}


alu ynod::Sub()
{
    return {};
}


alu ynod::Mul()
{
    return {};
}


alu ynod::Indirection()
{
    return {};
}


alu ynod::Modulo()
{
    return {};
}


alu ynod::LessThan()
{
    return {};
}


alu ynod::GreaterThan()
{
    return {};
}


alu ynod::Assign()
{
    return {};
}


alu ynod::BinaryAnd()
{
    return {};
}


alu ynod::BinaryOr()
{
    return {};
}


alu ynod::BinaryXor()
{
    return {};
}


alu ynod::C1()
{
    return {};
}


alu ynod::C2()
{
    return {};
}


alu ynod::BinaryNot()
{
    return {};
}


alu ynod::LogicalAnd()
{
    return {};
}


alu ynod::LogicalOr()
{
    return {};
}


alu ynod::Div()
{
    return {};
}


alu ynod::Comma()
{
    return {};
}


alu ynod::Factorial()
{
    return {};
}


alu ynod::Positive()
{
    return {};
}


alu ynod::Negative()
{
    return {};
}


alu ynod::Pi()
{
    return {};
}


alu ynod::Number()
{
    return {};
}


alu ynod::U8()
{
    return {};
}


alu ynod::U16()
{
    return {};
}


alu ynod::U32()
{
    return {};
}


alu ynod::U64()
{
    return {};
}


alu ynod::I8()
{
    return {};
}


alu ynod::I16()
{
    return {};
}


alu ynod::I32()
{
    return {};
}


alu ynod::I64()
{
    return {};
}


alu ynod::Float()
{
    return {};
}


alu ynod::String()
{
    return {};
}


alu ynod::AddressOf()
{
    return {};
}


alu ynod::Prime()
{
    return {};
}


alu ynod::Cosinus()
{
    return {};
}


alu ynod::ArcCosinus()
{
    return {};
}


alu ynod::Tangent()
{
    return {};
}


alu ynod::ArcTangent()
{
    return {};
}


alu ynod::Sinus()
{
    return {};
}


alu ynod::ArcSinus()
{
    return {};
}

}
