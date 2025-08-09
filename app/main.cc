#include <tuxvision/tui/application.h>
#include <tuxvision/utility/app_base.h>
#include <tuxvision/lexer/lexer.h>
//#include </vision/element.h>
#include <tuxvision/est/ynod.h>  //-- Future y-nod interpreter ;)
#include <tuxvision/utility/io/console.h>
#include <tuxvision/utility/strscan.h>
#include <tuxvision/tui/element.h>
#include <tuxvision/tui/event.h>
#include <tuxvision/tui/screen_dc.h>
#include <tuxvision/tui/label_dc.h>

namespace tux::vision
{


class app : public application
{
    CLASSNAME_DERIVED(app)
    window_dc* window{nullptr};
    label_dc* label{nullptr};
    strscan  scanner{};
    lang::token_reference_table lang_tokens{};
    lang::token_info::list  tokens{};
public:
    app(const std::string& app_name, tux::string::view_list&& arguments, tux::string::view_list&& env={});
    ~app() override;

    rem::code run() override;
    rem::code setup()override;
    rem::code terminate(rem::type reason) override;
    rem::code setup_ui()override;
    rem::code scan_tokens();
    rem::code alu_playground();
    rem::code geometry_playground();

};


app::app(const std::string&app_name, tux::string::view_list&&arguments, tux::string::view_list&&env):
application(app_name, std::move(arguments), std::move(env))
{
}


app::~app()
{
    ;
}


rem::code app::run()
{
   try
    {
        auto ret = setup();
        geometry_playground();
        scan_tokens();
        alu_playground();
        if (!ret)
           return terminate(rem::type::err);

        auto l = sys::message(1); l << "app::run" << l;

        auto rac = rem::action::cont;
        do{
            event::poll();
            auto evt = event::pop();
            while (evt)
            {
                auto ret = process_event(evt);
                if (ret==rem::code::terminate)
                    return terminate(rem::type::grace);

                if (evt.is<event::command>())
                {
                    l = sys::info(1); l << "app::run: command:" <<evt.data.cmd.name() << l;
                    if (evt.data.cmd.evt == event::Cancel)
                    {
                        l = sys::message(1); l << "Quitting on Cancel command...(bypass end-loop;)" << l;
                        rac = rem::action::end;
                        return terminate(rem::type::grace);
                    }
                }
                else
                {
                    if (evt.is<io::mouse>())
                    {
                        l = sys::info(1); l << "mouse event:" << evt.data.mouse() << l;
                        auto* e = screen_dc::target_xy(evt.data.mouse.pos);
                        if (e)
                        {
                            //if (e->event_mask() & event::Mouse)
                                auto res = e->process_event(evt);
                        }
                    }
                }
                evt = event::pop();
            }while (evt);
        }while(rac == rem::action::cont);
        if (!!ret)
            return terminate(rem::type::grace);

        return terminate(rem::type::aborted);
    }
    catch (sys::exception& e)
    {
        std::cout << e.what() << std::endl;
        return rem::code::failed;
    }

}


rem::code app::setup()
{
    if (auto r = application::setup(); !!r) // He ouais, !! ( not-not ) = true :) Je peux pas override l'operateur boolean globalement sur un rem::type ou rem::code sauf pour l'operateur '!' en prefixe ... :(
        return setup_ui();
    else
        return r; // else oblige pour l'accès à `r` qui est dans la portée/scope du bloc `if` ...ugly c++ moderne
}


rem::code app::terminate(rem::type reason)
{
    //delete window;
    //delete label;
    return application::terminate(reason);
}


rem::code app::setup_ui()
{
    auto rt = application::setup_ui();
    if (!rt) return rt;
    window = new window_dc("Root",windowtype::Toplevel);
    label = new label_dc(window, "Hello world");
    window->set_theme("C128");
    window->set_geometry({{1,1},ui::whinfo{80,20}});
    window->set_anchor(anchor::Center);
    screen_dc::anchor_element(window);
    label->set_geometry({{2,1},ui::whinfo{50,1}});
    label->set_anchor(anchor::Width|anchor::Bottom);

    screen_dc::push(window);
    screen_dc::draw();

    return rem::code::done;
}



rem::code app::scan_tokens()
{
    auto str = R"(
// ---------- The beginning of sys::lang !


function i_am_a_function u64 a=0, f32 b : u64
{
    if a==1
        return a/b;
    else
        return 0.9090*b*4ac;
}

procedure radicalment_votre i32 arg = @r::n, f16# arg2
{
    arg ^/ 2;
    arg += |<.14159>|;

}

// ----------- LineComment test ...



/*!
    test bloc comment
*/
function main int argc, string[] argv : i32
{
    console::out << 'Hello world!' << sys::eol;
    main = rem::code::ok;
}

)";

    //scanner = str;

    lang_tokens.set();
    lang::lexer lexer;
    lexer.config() = {
        .tokens_table = &lang_tokens,
        .tokens = &tokens,
        .text = str,
    };
    auto [ret,sz] = lexer.parse();
    auto l = sys::info(1); l << "Scanned " << color::blue << sz << color::r << " tokens" << l;
    if (!!ret || sz>0)
        lexer.debug_dump();


    return ret;
}


rem::code app::alu_playground()
{
    lang::est::alu a=0.4;
    auto l = sys::test(1); l << "a=" << a.value<float>() << l;
    l << "alu::infos: " << a.debug_infos() << l;
    return rem::code::success;
}


rem::code app::geometry_playground()
{
    ui::rectangle ra,rb,rc;
    ra = {{10,10}, whinfo{30,20}};
    rb = ra + ui::cxy{-3,-1};
    rc = ra & rb;
    auto l = sys::test(1); l << "ra{" << ra << "};" << l;
    l = sys::test(1); l << "rb{" << rb << "};" << l;
    l = sys::test(1); l << "rc = ra & rb: {" << rc << "};" << l;
    rc = ra /rb;
    l = sys::test(1); l << "rc = ra / rb: {" << rc << "};" << l;
    rc.moveat(ra.a - rb.a);
    l = sys::test(1); l << "rc = ra.a-rb.a{" << rc << "};" << l;
    return rem::code::success;
}

}


auto main(int argc, char** argv, char** env) -> int
{
    tux::vision::app app("app", tux::string::string_view_list(argc, argv,1));
    return static_cast<int>(app.run());
}
