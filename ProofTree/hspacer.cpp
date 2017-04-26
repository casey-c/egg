#include "hspacer.h"

namespace HSpacer
{
    QSpacerItem* makeSpacer()
    {
        return new QSpacerItem( 1,
                                1,
                                QSizePolicy::Expanding,
                                QSizePolicy::Fixed );
    }
}
