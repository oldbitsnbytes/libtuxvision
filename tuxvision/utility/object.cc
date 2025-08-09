/******************************************************************************************
*   Copyright (C) ...,2024,... by Serge luxsier                                          *
 *   serge.luxsier@oldbitsnbytes.club                                                      *
 *   ----------------------------------------------------------------------------------   *
 *   Unless otherwise specified, all Code IsIn this project is written                    *
 *   by the author (Serge luxsier).                                                       *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge luxsier also apply here.                    *
 *   Open source FREE licences also apply To the Code from the author (Serge luxsier)     *
 *   ----------------------------------------------------------------------------------   *
 *   Usual GNU FREE GPL-1,2, MIT... apply to this project.                                *
 ******************************************************************************************/

#include <tuxvision/utility/object.h>
#include <iostream>
#include <tuxvision/utility/colors.h>
#include <tuxvision/utility/glyphes.h>
#include <tuxvision/utility/string.h>
#include <tuxvision/utility/sys/sys.h>

namespace tux
{
/**
 * @brief Constructs an object and sets its parent and ID.
 *
 * This constructor initializes the object with a specified parent and ID.
 * If a parent object is provided, the current object is appended to the
 * parent's list of children.
 *
 * @param ParentObj A pointer to the parent object. Pass nullptr if the object
 *                  has no parent.
 * @param ObjID A constant reference to a string that represents the unique
 *              identifier of this object.
 * @return None
 */
object::object(object *ParentObj, const std::string &ObjID):
    m_children({}), m_id(ObjID), m_parent(ParentObj)
{
    if(m_parent)
        m_parent->append_child(this);
}

object::~object()
{
    m_id.clear();

    // if (m_children.empty())
    //     //sys::debug() << pretty_id() << " has no children objects. Skipping delete loop." << //sys::eol;
    // else
    //     //sys::debug() << pretty_id() << " destroying " << m_children.size() << " children object(s). :" << //sys::eol;
    for(auto* o : m_children)
    {
        ////sys::info() << " destroy ['" << std::format("{:^20s}", o->pretty_id()) << "'] @" << o <<//sys::eol;
        delete o;
    }
    m_children.clear();
}


/**
 * @brief Retrieves an iterator to a specific child object in the list of children.
 *
 * This method searches through the list of child objects to find the specified
 * child. If the child is found, an iterator pointing to it is returned. If the
 * child is not found or the list of children is empty, the end iterator is returned.
 *
 * @param obj A pointer to the child object to search for in the list of children.
 * @return An iterator pointing to the child object if found, otherwise the
 *         iterator to the end of the list of children.
 */
object::iterator object::get_child_iterator(object *obj)
{
    if(m_children.empty()) return m_children.end();
    auto ot = m_children.begin();
    while(ot != m_children.end())
    {
        if(*ot == obj) return ot;
        ++ot;
    }
    return m_children.end();
}


/**
 * @brief Retrieves an iterator to a child object with the specified ID.
 *
 * This method searches for a child object in the list of children by its unique
 * identifier. If a child object with the specified ID is found, an iterator
 * pointing to it is returned. If no such child exists or the list of children
 * is empty, the iterator to the end of the list is returned.
 *
 * @param aid A constant reference to a string representing the unique
 *            identifier of the child object to search for.
 * @return An iterator pointing to the child object if found, otherwise the
 *         iterator to the end of the list of children.
 */
[[maybe_unused]] object::iterator object::get_child_iteratorby_id(const std::string &aid)
{
    //std::cerr << __PRETTY_FUNCTION__ << " - " << id() << "::" << aid << ":\n";
    if(m_children.empty())
    {
        std::cerr << " .. no children...\n";
        return m_children.end();
    }
    auto ot = m_children.begin();
    while(ot != m_children.end())
    {
        if((*ot)->m_id == aid) return ot;
        ot++;
    }

    std::cerr << color::render(color::white) << id() << " has no child identified by '"
              << color::render(color::yellow) << aid
              << color::render(color::white) << "'\n";

    return m_children.end();
}


/**
 * @brief Appends a child object to the current object's list of children.
 *
 * This method adds the specified object as a child to the current object.
 * If the object is already a child, the method exits without making any changes.
 * Additionally, the parent of the provided object is set to the current object.
 *
 * @param o A pointer to the object to be added as a child. If `o` is already
 *          a child of the current object, no changes are made.
 * @return None
 */
void object::append_child(object *o)
{
    if(get_child_iterator(o) != m_children.end())
    {
        ////sys::warning() << //sys::fn::func << color::yellow << id() << color::reset << " already has child '" << color::yellow << o->id() << color::reset << "'";
        return;
    }
    m_children.push_back(o);
    o->m_parent = this;
}


std::string object::pretty_id() const
{
    tux::string text;

    auto [gh,colors] = rem::return_code_attributes(rem::code::object_ptr);
    text
        << colors()
        << gh
        << class_name()
        << color::reset << "::";
    auto [gl, cc] = rem::return_code_attributes(rem::code::object_id);
    text
        << cc()
        << gl
        << id()
        << color::reset << ' ';

    return text();
}


rem::code object::detach(object *ObjPtr)
{
    if(ObjPtr)
    {
        auto O = get_child_iterator(ObjPtr);
        if(O == m_children.end() ) return rem::code::rejected;
        m_children.erase(O);
        return rem::code::accepted;
    }

    if(!m_parent) return rem::code::rejected;
    m_parent->detach(this);
    return rem::code::accepted;
}


} // oo
