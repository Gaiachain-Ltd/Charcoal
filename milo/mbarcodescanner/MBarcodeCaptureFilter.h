/*******************************************************************************
Copyright (C) 2017 Milo Solutions
Contact: https://www.milosolutions.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#ifndef BARCODECAPTUREFILTER_H
#define BARCODECAPTUREFILTER_H

#include <QAbstractVideoFilter>

class QZXing;

class BarcodeCaptureFilter : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(qreal sourceWidth MEMBER m_sourceWidth NOTIFY sourceWidthChanged)
    Q_PROPERTY(qreal sourceHeight MEMBER m_sourceHeight NOTIFY sourceHeightChanged)
    Q_PROPERTY(int orientation MEMBER m_orientation NOTIFY orientationChanged)

public:
    explicit BarcodeCaptureFilter(QObject *parent = Q_NULLPTR);

    QVideoFilterRunnable *createFilterRunnable() Q_DECL_OVERRIDE;

    int orientation() const { return m_orientation; }

#ifdef DRAW_REGION
    void processDecodingFinished(bool success);
    void processTagFoundAdvanced(const QString &tag,
                                 const QString &format,
                                 const QString &charSet,
                                 const QRectF &rect);
#endif

signals:
    void tagFound(const QString &tag) const;
#ifdef DRAW_REGION
    void requestDrawRect(const QRectF &rect) const;
    void requestCleanView() const;
#endif

    void sourceWidthChanged() const;
    void sourceHeightChanged() const;
    void orientationChanged() const;

private:
    qreal m_sourceWidth;
    qreal m_sourceHeight;
    int m_orientation;
#ifdef DRAW_REGION
    bool m_hasCleanView = true;
#endif
};

class BarcodeCaptureFilterRunnable : public QVideoFilterRunnable
{
public:
    BarcodeCaptureFilterRunnable(BarcodeCaptureFilter *filter);
    ~BarcodeCaptureFilterRunnable();

    QVideoFrame run(QVideoFrame *input,
                    const QVideoSurfaceFormat &surfaceFormat,
                    RunFlags flags) Q_DECL_OVERRIDE;

private:
    void decode(uchar *data, const QSize &size, int bpl);

    BarcodeCaptureFilter *m_filter;
    QZXing *m_qzxing;
    bool m_inProgress;
};

#endif // BARCODECAPTUREFILTER_H
