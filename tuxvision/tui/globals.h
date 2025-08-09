
////////////////////////////////////////////////////////////////////////////////////////////
//   Copyright (C) ...,2025,... by Serge Lussier
//   serge.lussier@oldbitsnbytes.club / lussier.serge@gmail.com
//   ----------------------------------------------------------------------------------
//   Unless otherwise specif  §ied, all Codes and files in this project is written
//   by the author and owned by the author (Serge Lussier), unless otherwise specified.
//   ----------------------------------------------------------------------------------
//   Copyrights from authors other than Serge Lussier also apply here.
//   Open source FREE licences also apply to the code from the author (Serge Lussier)
//   ----------------------------------------------------------------------------------
//   Usual GNU FREE GPL-1,2, MIT... or whatever -  apply to this project.
//   ----------------------------------------------------------------------------------
//   NOTE : All source code that I am the only author, I reserve for myself, the rights to
//   make it to restricted private license.
////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------


#pragma once
//#include <tuxvision/utility/defs.h>
#include <tuxvision/utility/colors.h>

using namespace tux::integers;
namespace tux::ui
{




namespace windowtype
{
    using value = u32;
    static constexpr value None      = 0x0000;
    static constexpr value Floating  = 0x0001;
    static constexpr value Dialog    = 0x0002;
    static constexpr value FloatingMenu      = 0x0004;
    static constexpr value FloatingToolBar   = 0x0008;
    static constexpr value Toplevel  = 0x0010;
}

namespace component
{
using value = u32;
static constexpr value None      = 0x0000;
static constexpr value Frame     = 0x0001;
static constexpr value Caption   = 0x0002;
static constexpr value StatusBar = 0x0004;
static constexpr value Input     = 0x0008;
static constexpr value Button    = 0x0010;
static constexpr value Scroll    = 0x0020;
static constexpr value SuffixGlyph    = 0x0040;
static constexpr value PrefixGlyph    = 0x0080;
static constexpr value MenuBar       = 0x0100;
static constexpr value Menu          = 0x0200;
//...
}


namespace buttons
{
using value = u32;
static constexpr value BtnNone   = 0x0000;
static constexpr value BtnCancel = 0x0010;
static constexpr value BtnOk     = 0x0020;
static constexpr value BtnYes    = 0x0040;
static constexpr value BtnNo     = 0x0080;
static constexpr value BtnHelp   = 0x0100;
static constexpr value BtnClose  = 0x0200;
static constexpr value BtnSave   = 0x0400;
static constexpr value BtnLoad   = 0x0800;
static constexpr value BtnNew    = 0x1000;
static constexpr value BtnOpen   = 0x2000;
static constexpr value BtnEdit   = 0x4000;
static constexpr value BtnDelete = 0x8000;
static constexpr value BtnRun    = 0x10000;
static constexpr value BtnStop   = 0x20000;
static constexpr value BtnPause  = 0x40000;
static constexpr value BtnResume = 0x80000;
static constexpr value BtnDone   = 0x100000;
static constexpr value BtnAbort  = 0x200000;
static constexpr value BtnRetry  = 0x400000;
static constexpr value BtnIgnore = 0x800000;
static constexpr value BtnQuit   = 0x1000000;
static constexpr value BtnAbout  = 0x4000000;
static constexpr value BtnExit   = 0x8000000;
static constexpr value BtnReset  = 0x10000000;
static constexpr value BtnApply  = 0x20000000;

//...

}

namespace uistatus
{
using value = u32;
static constexpr value None      = 0x0000;
static constexpr value Normal    = 0x0001;
static constexpr value Warning   = 0x0002;
static constexpr value Error     = 0x0004;
static constexpr value Success   = 0x0008;
static constexpr value Focussed  = 0x0010;
static constexpr value Disabled  = 0x0020;
static constexpr value Selected  = 0x0040;
static constexpr value Checked   = 0x0080;
static constexpr value Unchecked = 0x0100;
static constexpr value Inactive  = 0x0200;
static constexpr value Active    = 0x0400;
static constexpr value Hidden    = 0x0800;
static constexpr value Visible   = 0x1000;
static constexpr value Locked    = 0x2000;
static constexpr value Unlocked  = 0x4000;

}




namespace anchor
{
using value = U16;
static constexpr anchor::value None     = 0x0000; ///< free fixed xy placement
static constexpr anchor::value fixed    = 0x0001; ///< free fixed xy placement
static constexpr anchor::value Width    = 0x0002; ///< Fits the entire width size
static constexpr anchor::value Height   = 0x0004; ///< Fits the entire height size
static constexpr anchor::value Frame    = 0x0008; ///< Overlaps if wh size > 1, or sits on the frame.
static constexpr anchor::value Right    = 0x0010;
static constexpr anchor::value Left     = 0x0020;
static constexpr anchor::value Top      = 0x0040;
static constexpr anchor::value Bottom   = 0x0080;
static constexpr anchor::value Center   = 0x0100; ///< Both fit_h&vcenter ...
static constexpr anchor::value HCenter  = 0x0200;
static constexpr anchor::value VCenter  = 0x0400;

//...
}





struct padding_info
{
    i8 top{0};
    i8 left{0};
    i8 bottom{0};
    i8 right{0};
};

struct margin_info
{
    i8 top{0};
    i8 left{0};
    i8 bottom{0};
    i8 right{0};
};


struct colordb
{
    using status = std::map<uistatus::value, color::pair>;
    using elements = std::map<std::string, status>;
    using palette = std::map<std::string, elements>;

    static colordb::palette palettedb;
    static colordb select() { return {};}
    colordb::elements& operator[](const std::string& palette_name) const;

};



}
