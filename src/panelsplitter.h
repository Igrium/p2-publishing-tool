#pragma once

#include <QSplitter>

namespace ui
{
	// Creates a splitter used to divide a window into user-resizable panels.
	// Panels can't be collapsed to zero size, so they can never be "lost" by dragging.
	inline QSplitter *createPanelSplitter( Qt::Orientation orientation, QWidget *pParent )
	{
		auto pSplitter = new QSplitter( orientation, pParent );
		pSplitter->setChildrenCollapsible( false );
		pSplitter->setHandleWidth( 6 );
		return pSplitter;
	}
} // namespace ui
