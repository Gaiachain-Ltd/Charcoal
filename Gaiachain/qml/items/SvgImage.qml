import QtQuick 2.11

Image {
    fillMode: Image.PreserveAspectFit

    function getPreserveAspectSize(sourceWidth, sourceHeight, width, height, fillMode) {
        if (fillMode !== Image.PreserveAspectCrop && fillMode !== Image.PreserveAspectFit) {
            console.warn("SvgImage:getPreserveAspectSize: unsupported fillMode!");
            return
        }

        var widthScale = width / sourceWidth
        var heightScale = height / sourceHeight

        //Reverse flag if PreserveAspectCrop is used to fill to the "bigger" scale
        var scaleHeight = widthScale <= heightScale
        scaleHeight = fillMode === Image.PreserveAspectCrop ? !scaleHeight : scaleHeight

        var size = Qt.size(0, 0)
        if (scaleHeight) {
            size.width = width
            size.height = widthScale * sourceHeight
        } else {
            size.width = heightScale * sourceWidth
            size.height = height
        }
        return size
    }

    function updateSourceSize() {
        if (fillMode === Image.PreserveAspectFit || fillMode === Image.PreserveAspectCrop) {
            sourceSize = getPreserveAspectSize(sourceSize.width, sourceSize.height, width, height, fillMode)
        } else {
            if (fillMode !== Image.Stretch){
                console.warn("SvgImage: unsupported fillMode!")
            }

            sourceSize.width = width
            sourceSize.height = width
        }
    }

    Component.onCompleted: updateSourceSize()
    onWidthChanged: updateSourceSize()
    onHeightChanged: updateSourceSize()
    onSourceChanged: updateSourceSize()
}
