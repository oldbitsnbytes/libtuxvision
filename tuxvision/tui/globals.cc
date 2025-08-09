


#include "globals.h"


namespace tux::ui
{


colordb::palette colordb::palettedb=
{
    {
        "Default",
        {
            {
                "Element",
                {
                    {uistatus::Active, {color::grey100, color::grey27}},
                    {uistatus::Disabled, {color::grey30, color::grey35}}, // keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::grey27}},
                    {uistatus::Focussed, {color::red4, color::grey27}}
                    //...
                }
            },
            {
                "Frame",
                {
                    {uistatus::Active, {color::lightcyan3, color::darkblue}},
                    {uistatus::Disabled, {color::grey30, color::darkblue}}, // keep coherence with widgets base
                    {uistatus::Normal, {color::grey100, color::darkblue}},
                    {uistatus::Focussed, {color::greenyellow, color::darkblue}}
                    //...
                }
            },
            {
                "Label",
                {
                    {uistatus::Active, {color::indianred, color::blue}},
                    {uistatus::Disabled, {color::grey19, color::grey35}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::blue}}, // Keep coherence with widgets base
                    {uistatus::Focussed, {color::yellow, color::blue}}
                }
            },
            {
                "Shortkey",
                {
                    {uistatus::Active, {color::hotpink4, color::grey27}},
                    {uistatus::Disabled, {color::grey19, color::grey35}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::grey27}},
                    {uistatus::Focussed, {color::red4, color::grey27}}
                }
            },
            {
                "Icon",
                {
                    {uistatus::Active, {color::blue1, color::grey27}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::grey19, color::grey35}},
                    {uistatus::Normal, {color::blue1, color::grey35}},
                    {uistatus::Focussed, {color::aquamarine3, color::grey27}},
                    //{uistatus::Success, {color::green5, color::grey27}}
                }
            },
            {
                "Input-Area", // widgets::text_input
                {
                    {uistatus::Active, {color::white, color::grey30}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::grey39, color::grey35}},
                    {uistatus::Normal, {color::grey37, color::grey30}},
                    {uistatus::Focussed, {color::white, color::grey30}}
                }
            },
            {
                "Input-Field",
                {
                    {uistatus::Active, {color::grey70, color::grey27}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::grey39, color::grey35}},
                    {uistatus::Normal, {color::grey70, color::grey27}},
                    {uistatus::Focussed, {color::white, color::grey27}}
                },
            },
            {
                "Input-Field:Placeholder",
                {
                    {uistatus::Active, {color::darkcyan, color::grey27}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::blue, color::grey27}},
                    {uistatus::Normal, {color::blue, color::grey27}},
                    {uistatus::Focussed, {color::blue, color::grey27}}
                }
            },
            {
                "StatusBar",
                {
                    {uistatus::Active, {color::lime,      color::grey27}},
                    {uistatus::Disabled, {color::grey30,   color::grey27}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70,    color::grey27}},
                    {uistatus::Focussed, {color::red4,       color::grey27}}
                  //...
                }
            }
        }
    },
    {
        "C64",
        {
            {
                "Element",
                {
                    {uistatus::Active, {color::darkturquoise, color::darkblue}},
                    {uistatus::Disabled, {color::grey30, color::darkblue}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::darkblue}},
                    {uistatus::Focussed, {color::red4, color::darkblue}}
                    //...
                }
            },
            {
                "Frame",
                {
                    {uistatus::Active, {color::lightcyan3, color::darkblue}},
                    {uistatus::Disabled, {color::grey30, color::darkblue}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey100, color::darkblue}},
                    {uistatus::Focussed, {color::greenyellow, color::darkblue}}
                    //...
                }
            },
            {
                "Label",
                {
                    {uistatus::Active, {color::lightcyan3, color::darkblue}},
                    {uistatus::Disabled, {color::grey19, color::grey35}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::darkblue}}, // Keep coherence with widgets base
                    {uistatus::Focussed, {color::yellow, color::darkblue}}
                }
            },
            {
                "Shortkey",
                {
                    {uistatus::Active, {color::hotpink4, color::darkblue}},
                    {uistatus::Disabled, {color::grey19, color::darkblue}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::darkblue}},
                    {uistatus::Focussed, {color::red4, color::darkblue}}
                }
            },
            {
                "Icon",
                {
                    {uistatus::Active, {color::blue1, color::darkblue}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::grey19, color::darkblue}},
                    {uistatus::Normal, {color::blue1, color::darkblue}},
                    {uistatus::Focussed, {color::aquamarine3, color::darkblue}},
                    //{uistatus::Success, {color::green5, color::darkblue}}
                }
            },
            {
                "Input-Area", // widgets::text_input
                {
                    {uistatus::Active, {color::white, color::blue}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::grey39, color::blue}},
                    {uistatus::Normal, {color::grey37, color::blue}},
                    {uistatus::Focussed, {color::white, color::blue}}
                }
            },
            {
                "Input-Field",
                {
                    {uistatus::Active, {color::lightcyan3, color::blue4}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::blue, color::darkblue}},
                    {uistatus::Normal, {color::blue, color::darkblue}},
                    {uistatus::Focussed, {color::blue, color::darkblue}}
                },

            },
            {
                "Input-Field:Placeholder",
                {
                    {uistatus::Active, {color::darkcyan, color::blue4}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::blue, color::darkblue}},
                    {uistatus::Normal, {color::blue, color::darkblue}},
                    {uistatus::Focussed, {color::blue, color::darkblue}}
                },
            },
            {
                "StatusBar",
                {
                    {uistatus::Active, {color::lime, color::blue4}},
                    {uistatus::Disabled, {color::grey30, color::blue4}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::blue4}},
                    {uistatus::Focussed, {color::red4, color::blue4}}
                      //...
                }
            }
        }
    },
    {
        "C128",
        {
            {
                "Element",
                {
                    {uistatus::Active, {color::darkseagreen, color::grey15}},
                    {uistatus::Disabled, {color::grey30, color::grey15}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::grey15}},
                    {uistatus::Focussed, {color::red4, color::grey15}}
                    //...
                }
            },
            {
                "Frame",
                {
                    {uistatus::Active, {color::darkseagreen, color::grey15}},
                    {uistatus::Disabled, {color::grey30, color::grey15}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::lightcyan3, color::grey15}},
                    {uistatus::Focussed, {color::greenyellow, color::grey15}}
                    //...
                }
            },
            {
                "Label",
                {
                    {uistatus::Active, {color::darkseagreen4, color::grey15}},
                    {uistatus::Disabled, {color::grey19, color::grey35}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::darkblue}}, // Keep coherence with widgets base
                    {uistatus::Focussed, {color::yellow, color::darkblue}}
                }
            },
            {
                "Shortkey",
                {
                    {uistatus::Active, {color::hotpink4, color::darkblue}},
                    {uistatus::Disabled, {color::grey19, color::darkblue}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::darkblue}},
                    {uistatus::Focussed, {color::red4, color::darkblue}}
                }
            },
            {
                "Icon",
                {
                    {uistatus::Active, {color::yellow3, color::darkblue}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::grey19, color::darkblue}},
                    {uistatus::Normal, {color::blue1, color::darkblue}},
                    {uistatus::Focussed, {color::aquamarine3, color::darkblue}},
                    //{uistatus::Success, {color::green5, color::darkblue}}
                }
            },
            {
                "Input-Area", // widgets::text_input
                {
                    {uistatus::Active, {color::darkseagreen, color::grey15}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::darkseagreen, color::grey15}},
                    {uistatus::Normal, {color::darkseagreen, color::grey15}},
                    {uistatus::Focussed, {color::darkseagreen, color::grey15}}
                }
            },
            {
                "Input-Field",
                {
                    {uistatus::Active, {color::darkseagreen, color::grey11}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::darkseagreen, color::grey15}},
                    {uistatus::Normal, {color::darkseagreen, color::grey15}},
                    {uistatus::Focussed, {color::darkseagreen, color::grey15}}
                }
            },
            {
                "Input-Field:Placeholder",
                {
                    {uistatus::Active, {color::grey23, color::grey11}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::darkseagreen, color::grey15}},
                    {uistatus::Normal, {color::darkseagreen, color::grey15}},
                    {uistatus::Focussed, {color::darkseagreen, color::grey15}}
                }
            },
            {
                "StatusBar",
                {
                    {uistatus::Active, {color::lime, color::grey15}},
                    {uistatus::Disabled, {color::grey30, color::grey15}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::grey15}},
                    {uistatus::Focussed, {color::red4, color::grey15}}
                    //...
                }
            }
        }
    },
    {
        "Cat-Latte",
        {
            {
                "Element",
                {
                    {uistatus::Active, {color::darkblue, color::grey85}},
                    {uistatus::Disabled, {color::grey30, color::grey74}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::turquoise2, color::grey78}},
                    {uistatus::Focussed, {color::darkblue, color::white}}
                    //...
                }
            },
            {
                "Frame",
                {
                    {uistatus::Active, {color::lightcyan3, color::darkblue}},
                    {uistatus::Disabled, {color::grey30, color::darkblue}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey100, color::darkblue}},
                    {uistatus::Focussed, {color::greenyellow, color::darkblue}}
                    //...
                }
            },
            {
                "Label",
                {
                    {uistatus::Active, {color::lightcyan3, color::darkblue}},
                    {uistatus::Disabled, {color::grey19, color::grey35}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::darkblue}}, // Keep coherence with widgets base
                    {uistatus::Focussed, {color::yellow, color::darkblue}}
                }
            },
            {
                "Shortkey",
                {
                    {uistatus::Active, {color::hotpink4, color::darkblue}},
                    {uistatus::Disabled, {color::grey19, color::darkblue}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::darkblue}},
                    {uistatus::Focussed, {color::red4, color::darkblue}}
                }
            },
            {
                "Icon",
                {
                    {uistatus::Active, {color::blue1, color::darkblue}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::grey19, color::darkblue}},
                    {uistatus::Normal, {color::blue1, color::darkblue}},
                    {uistatus::Focussed, {color::aquamarine3, color::darkblue}},
                    //{uistatus::Success, {color::green5, color::darkblue}}
                }
            },
            {
                "Input-Area", // widgets::text_input
                {
                    {uistatus::Active, {color::white, color::blue}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::grey39, color::blue}},
                    {uistatus::Normal, {color::grey37, color::blue}},
                    {uistatus::Focussed, {color::white, color::blue}}
                }
            },
            {
                "Input-Field",
                {
                    {uistatus::Active, {color::lightcyan3, color::blue4}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::blue, color::darkblue}},
                    {uistatus::Normal, {color::blue, color::darkblue}},
                    {uistatus::Focussed, {color::blue, color::darkblue}}
                },

            },
            {
                "Input-Field:Placeholder",
                {
                    {uistatus::Active, {color::darkcyan, color::blue4}}, // Keep coherence with widgets base
                    {uistatus::Disabled, {color::blue, color::darkblue}},
                    {uistatus::Normal, {color::blue, color::darkblue}},
                    {uistatus::Focussed, {color::blue, color::darkblue}}
                },
            },
            {
                "StatusBar",
                {
                    {uistatus::Active, {color::lime, color::blue4}},
                    {uistatus::Disabled, {color::grey30, color::blue4}}, // Keep coherence with widgets base
                    {uistatus::Normal, {color::grey70, color::blue4}},
                    {uistatus::Focussed, {color::red4, color::blue4}}
                      //...
                }
            }
        }
    }

};


colordb::elements& colordb::operator[](const std::string&palette_name) const
{
    ///@todo Check exist

    if (auto palette_it = colordb::palettedb.find(palette_name); palette_it != colordb::palettedb.end())
    {
       return palette_it->second;
    }

    return palettedb["Default"];
}



}
