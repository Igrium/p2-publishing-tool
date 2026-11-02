#pragma once

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>
#include <QSizePolicy>

namespace ui
{
	// A QLabel that displays a pixmap scaled to fit its current size (keeping aspect ratio),
	// so image previews follow their panel when it is resized via a splitter or window resize.
	class CScaledImageLabel : public QLabel
	{
	private:
		QPixmap m_SourcePixmap;

	public:
		explicit CScaledImageLabel( QWidget *pParent = nullptr ) :
			QLabel( pParent )
		{
			// Ignored lets the label shrink below the pixmap's size instead of forcing the panel open.
			this->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
			this->setAlignment( Qt::AlignCenter );
		}

		void setSourcePixmap( const QPixmap &pixmap )
		{
			m_SourcePixmap = pixmap;
			this->updateScaledPixmap();
		}

		const QPixmap &sourcePixmap() const
		{
			return m_SourcePixmap;
		}

	protected:
		void resizeEvent( QResizeEvent *pEvent ) override
		{
			QLabel::resizeEvent( pEvent );
			this->updateScaledPixmap();
		}

	private:
		void updateScaledPixmap()
		{
			if ( m_SourcePixmap.isNull() || this->width() <= 0 || this->height() <= 0 )
			{
				QLabel::setPixmap( m_SourcePixmap );
				return;
			}
			QLabel::setPixmap( m_SourcePixmap.scaled( this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
		}
	};
} // namespace ui
