/******************************************************************************************
*   Copyright (C) ...,2025,... by Serge luxsier                                          *
 *   serge.luxsier@oldbitsnbytes.club / luxsier.serge@gmail.com                            *
 *   ----------------------------------------------------------------------------------   *
 *   Unless otherwise specified, all Codes ands files in this project is written          *
 *   by the author and owned by the auther (Serge luxsier), unless otherwise specified.   *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge luxsier also apply here.                    *
 *   Open source FREE licences also apply To the Code from the author (Serge luxsier)     *
 *   ----------------------------------------------------------------------------------   *
 *   Usual GNU FREE GPL-1,2, MIT... or whatever -  apply to this project.                 *
 *   ----------------------------------------------------------------------------------   *
 *   NOTE : All code source that I am the only author, I reserve for myself, the rights to*
 *   make it to restricted private license.                                               *
 ******************************************************************************************/




#pragma once
#include <tuxvision/utility/rem.h>
#include <vector>
#include <functional>


namespace tux
{

using namespace integers;


/**
 *  @brief Class object
 */
class   object
{
    CLASSNAME_START(object)
protected:
    u64 _event_mask{0};
public:

    using list      = std::vector<object*>;
    using iterator  = object::list::iterator;


    object(){} // no default destructor to prevent "auto-deleted constructor" by accident in derived classes.
    object(object* parent_obj, const std::string& obj_id);
    object(object&&)  = delete;
    object(const object&) = delete;

    virtual ~object();

    object& operator = (object&&) noexcept = delete;
    object& operator = (const object&) = delete;

    [[maybe_unused]] void set_id(const std::string& obj_id) { m_id = obj_id; }
    object::iterator get_child_iterator(object* obj);
    [[maybe_unused]] object::iterator get_child_iteratorby_id(const std::string& aid);

    ////sys::Enums::Code detach();
    rem::code detach(object* obj_ptr=nullptr);
    // -- All does the same, ...as a different name lol!

    template<typename T> [[maybe_unused]] T* as() { return dynamic_cast<T*>(this); }

    // -----------------------------------------------------------------------------
    template<typename T> T* parent() {
        if(!m_parent) return nullptr;
        return dynamic_cast<T*>(m_parent);
    }


    template<typename T> [[maybe_unused]] T* first_parent_of() {
        if(!m_parent) return nullptr;
        object* o = m_parent;
        while(o)
        {
            T* tp = dynamic_cast<T*>(o);
            if(tp) return tp;
            o = o->m_parent;
        }
        return nullptr;
    }

    template<typename T> [[maybe_unused]] std::vector<T*> children_list()
    {
        std::vector<T*> A;
        for(auto* o : m_children)
        {
            T* tp = dynamic_cast<T*>(o);
            if(tp) A.push_back(tp);
        }
        return A;
    }

    [[nodiscard]] std::string id() const { return m_id; }


    /*!
     * @brief Apply function to requested derived object type in this object's children list.
     * @tparam T derived type
     * @param fn function ( typically lambda ).
     */
    template<typename T> [[maybe_unused]] std::vector<T*> for_each(std::function<bool(T&, bool)> fn)
    {
        auto it = m_children.begin();
        std::vector<T*> A;
        while(it != m_children.end())
        {
            bool last_obj = (it + 1) == m_children.end();
            if (T* obj_type_ptr= (*it)->as<T>(); obj_type_ptr)
            {
                if (fn)
                {
                    if (fn(*obj_type_ptr, last_obj))
                        A.push_back(obj_type_ptr);
                }
            }
            ++it;
        }
        return A;
    }
    void append_child(object* o);
    //void append_child(object::shared o);

    [[maybe_unused]] object::iterator begin() { return m_children.begin(); }
    [[maybe_unused]] object::iterator end()   { return m_children.end(); }
    [[nodiscard]] std::string pretty_id() const;
    u64& event_mask() { return _event_mask; }
protected:
    object::list m_children = {};
    std::string m_id{};
    object* m_parent{nullptr};
};

} // lsg
