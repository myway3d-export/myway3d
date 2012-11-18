#pragma once

#include "MWCore.h"


namespace Myway {

    class MW_ENTRY RenderEvent
    {
    public:
        static Event OnPreUpdateScene;
        static Event OnPostUpdateScene;

        static Event OnPreRenderEvent;
        static Event OnPostRenderEvent;

        static Event OnPreVisibleCull;
        static Event OnPostVisibleCull;

        static Event OnLostDevice;
        static Event OnResetDevice;

        static Event OnResize;
    };
}