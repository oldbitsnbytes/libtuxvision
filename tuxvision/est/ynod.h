
/******************************************************************************************
*   Copyright (C) ...,2025,... by Serge Lussier                                          *
 *   serge.lussier@oldbitsnbytes.club / lussier.serge@gmail.com                            *
 *   ----------------------------------------------------------------------------------   *
 *   Unless otherwise specified, all Codes ands files in this project is written          *
 *   by the author and owned by the auther (Serge Lussier), unless otherwise specified.   *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here.                    *
 *   Open source FREE licences also apply To the Code from the author (Serge Lussier)     *
 *   ----------------------------------------------------------------------------------   *
 *   Usual GNU FREE GPL-1,2, MIT... or whatever -  apply to this project.                 *
 *   ----------------------------------------------------------------------------------   *
 *   NOTE : All code source that I am the only author, I reserve for myself, the rights to*
 *   make it to restricted private license.                                               *
 ******************************************************************************************/



#pragma once
#include <tuxvision/lexer/lexer.h>
#include <tuxvision/utility/object.h>
#include <tuxvision/est/alu.h>


namespace tux::lang::est 
{
/**
 * @class ynod
 * @brief Represents a node in an arithmetic parse tree and provides runtime operations for mathematical processing.
 *
 * This class is derived from the `object` class and defines components for managing the structure
 * and behavior of arithmetic operations in a node-based parse tree. It provides facilities for
 * parsing input tokens into a tree and performing runtime operations on the nodes.
 */
class ynod : public object
{
    CLASSNAME_DERIVED(ynod)

    //using alu = std::shared_ptr<alu>;
    alu::shared acc{nullptr};
    using operator_fn = alu(ynod::*)();
    ynod::operator_fn operator_ptr{nullptr};
    static std::map<lang::mnemonic, ynod::operator_fn> operations_map;

protected:
    ynod* left{nullptr};
    ynod* right{nullptr};
    ynod* op{nullptr};
    token_info* token{nullptr};

    std::stack<ynod*> open_pairs{};

public:
    ynod():object(){};
    ~ynod() override=default;
    ynod(ynod* parent_ynod, token_info* token, alu::shared value_ref);

    ynod& operator*() {return *this;}

#pragma region arithmetic_parse_tree
    static ynod* tree_parse_input_open(ynod* parent_op, token_info* token,std::function<ynod*(token_info*)> ynod_maker_function);
    ynod* tree_parse_input_node(ynod* op_nod, token_info* token);
    static ynod* tree_parse_input_close(ynod* last_node);

private:
    ynod* tree_parse_input_binary(token_info* op_token);
    ynod* tree_parse_input_prefix(token_info* op_token);
    ynod* tree_parse_input_suffix(token_info* op_token);
    ynod* tree_parse_input_leaf(token_info* leaftoken);

    ynod* tree_set_left(ynod* operand);
    ynod* tree_set_right(ynod* operand);
    ynod* tree_set_op(ynod* op_token);

#pragma endregion arithmetic_parse_tree
protected:
#pragma region rt_operations
    virtual alu LeftShift         ();
    virtual alu Radical           ();
    virtual alu Exponent          ();
    virtual alu RightShift        ();
    virtual alu Decr              ();
    virtual alu Incr              ();
    virtual alu Address           ();
    virtual alu AssignAdd         ();
    virtual alu AssignSub         ();
    virtual alu AssignMul         ();
    virtual alu AssignDiv         ();
    virtual alu AssignMod         ();
    virtual alu AssignAnd         ();
    virtual alu AssignOr          ();
    virtual alu AssignXor         ();
    virtual alu AssignC1          ();
    virtual alu AssignLeftShift   ();
    virtual alu AssignRightShift  ();
    virtual alu Deref             ();
    virtual alu LessEq            ();
    virtual alu GreaterEq         ();
    virtual alu Equal             ();
    virtual alu NotEqual          ();
    virtual alu Add               ();
    virtual alu Sub               ();
    virtual alu Mul               ();
    virtual alu Indirection       ();
    virtual alu Modulo            ();
    virtual alu LessThan          ();
    virtual alu GreaterThan       ();
    virtual alu Assign            ();
    virtual alu BinaryAnd         ();
    virtual alu BinaryOr          ();
    virtual alu BinaryXor         ();
    virtual alu C1                ();
    virtual alu C2                ();
    virtual alu BinaryNot         ();
    virtual alu LogicalAnd        ();
    virtual alu LogicalOr         ();
    virtual alu Div                ();
    virtual alu Comma              ();
    virtual alu Factorial          ();
    virtual alu Positive           ();
    virtual alu Negative           ();
    virtual alu Pi                 ();
    virtual alu Number             ();
    virtual alu U8                 ();
    virtual alu U16                ();
    virtual alu U32                ();
    virtual alu U64                ();
    virtual alu I8                 ();
    virtual alu I16                ();
    virtual alu I32                ();
    virtual alu I64                ();
    virtual alu Float              ();
    virtual alu String             ();
    virtual alu AddressOf          ();
    virtual alu Prime              ();
    virtual alu Cosinus            ();
    virtual alu ArcCosinus         ();
    virtual alu Tangent            ();
    virtual alu ArcTangent         ();
    virtual alu Sinus              ();
    virtual alu ArcSinus           ();


#pragma endregion rt_operations
};




} // namespace tux::lang::est
