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

#include "MBarcodeCaptureFilter.h"

#include <QtConcurrentRun>

#include "QZXing.h"

#ifndef Q_OS_ANDROID
#   include "MImageConverters.h"
#endif

/*!
 * \brief Class represents video filter to detect and recognize barcodes from camera output.
 * \class BarcodeCaptureFilter
 */

BarcodeCaptureFilter::BarcodeCaptureFilter(QObject *parent) :
    QAbstractVideoFilter(parent)
{
}

/*!
 * \brief BarcodeCaptureFilter::createFilterRunnable - creates worker class for video filter.
 * \return
 */
QVideoFilterRunnable *BarcodeCaptureFilter::createFilterRunnable()
{
    return new BarcodeCaptureFilterRunnable(this);
}

#ifdef DRAW_REGION
/*!
 * \brief BarcodeCaptureFilter::processTagFoundAdvanced - handles taf found advanced signal with
 * additional info (barcode format, charset, tag rectangle).
 * It's used to draw tag region.
 * \param tag
 * \param format
 * \param charSet
 * \param rect
 */
void BarcodeCaptureFilter::processTagFoundAdvanced(const QString &tag,
                                                   const QString &format,
                                                   const QString &charSet,
                                                   const QRectF &rect)
{
    Q_UNUSED(format);
    Q_UNUSED(charSet);

    emit tagFound(tag);

    const QPointF topLeft(rect.topLeft());
    const QPointF bottomRight(rect.bottomRight());
    QRectF rectangle(QPointF(topLeft.x() * m_sourceWidth, topLeft.y() * m_sourceHeight),
                     QPointF(bottomRight.x() * m_sourceWidth, bottomRight.y() * m_sourceHeight));
    emit requestDrawRect(rectangle);
    m_hasCleanView = false;
}

/*!
 * \brief BarcodeCaptureFilter::processDecodingFinished - handles decoding finished signal.
 * It's used to clean view if we don't have any barcode.
 * \param success
 */
void BarcodeCaptureFilter::processDecodingFinished(bool success)
{
    if (success || m_hasCleanView)
        return;

    m_hasCleanView = true;
    emit requestCleanView();
}
#endif

/*!
 * \brief Class represents worker for video filter.
 * \class BarcodeCaptureFilterRunnable
 */

BarcodeCaptureFilterRunnable::BarcodeCaptureFilterRunnable(BarcodeCaptureFilter *filter) :
    QVideoFilterRunnable(),
    m_filter(filter),
    m_qzxing(new QZXing()),
    m_inProgress(false)
{
    m_qzxing->setDecoder(QZXing::DecoderFormat_CODE_39
                         | QZXing::DecoderFormat_CODE_128
                         | QZXing::DecoderFormat_EAN_8
                         | QZXing::DecoderFormat_EAN_13
                         | QZXing::DecoderFormat_QR_CODE);

#ifdef DRAW_REGION
    QObject::connect(m_qzxing, &QZXing::decodingFinished,
                     m_filter, &BarcodeCaptureFilter::processDecodingFinished);
    QObject::connect(m_qzxing, &QZXing::tagFoundAdvanced,
                     m_filter, &BarcodeCaptureFilter::processTagFoundAdvanced);
#else
    QObject::connect(m_qzxing, &QZXing::tagFound,
                     m_filter, &BarcodeCaptureFilter::tagFound);
#endif
}

BarcodeCaptureFilterRunnable::~BarcodeCaptureFilterRunnable()
{
    delete m_qzxing;
}

/*!
 * \brief BarcodeCaptureFilterRunnable::run - executes worker's task.
 * Here we try to get camera output and prepare all data for image analyzing.
 * \param input
 * \param surfaceFormat
 * \param flags
 * \return
 */
QVideoFrame BarcodeCaptureFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
    Q_UNUSED(surfaceFormat);
    Q_UNUSED(flags);

    if (!input->isValid() || m_inProgress)
        return *input;

    if (!input->map(QAbstractVideoBuffer::ReadOnly))
        return *input;

    m_inProgress = true;
    uchar *data = new uchar[input->mappedBytes()];
    memcpy(data, input->bits(), input->mappedBytes() * sizeof(uchar));
    const QSize size = input->size();
    int bpl = input->bytesPerLine();
    input->unmap();
    QtConcurrent::run(this, &BarcodeCaptureFilterRunnable::decode, data, size, bpl);
    return *input;
}

/*!
 * \brief BarcodeCaptureFilterRunnable::decode - prepares output data to suitable QImage
 * and runs QZXing image decoding.
 * This method is executed in separate thread.
 * \param data
 * \param size
 * \param bpl
 */
void BarcodeCaptureFilterRunnable::decode(uchar *data, const QSize &size, int bpl)
{
#ifdef Q_OS_ANDROID
    // BRG30 pixel format
    QImage image(data, size.width(), size.height(), bpl, QImage::Format_RGB32);
    image = image.mirrored();
    QTransform transform;
    transform.rotate(360 - m_filter->orientation());
    image = image.transformed(transform);
#else
    Q_UNUSED(bpl);
    QImage image(size, QImage::Format_RGB32);
    // YUV420P pixel format
    android::YV12ToRGB32(data, image.bits(), size.width(), size.height());
#endif

    m_qzxing->decodeImage(image);
    delete[] data;
    m_inProgress = false;
}

