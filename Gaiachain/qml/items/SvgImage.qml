import QtQuick 2.11

Image {

    fillMode: Image.PreserveAspectFit

    function getPreserveAspectSize(sourceWidth, sourceHeight, fillMode) {
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

    Component.onCompleted: {
        var sourceWidth = sourceSize.width
        var sourceHeight = sourceSize.height

        if (fillMode === Image.PreserveAspectFit || fillMode === Image.PreserveAspectCrop) {
            sourceSize.width = Qt.binding(function() {return getPreserveAspectSize(sourceWidth, sourceHeight, fillMode).width})
            sourceSize.height = Qt.binding(function() {return getPreserveAspectSize(sourceWidth, sourceHeight, fillMode).height})
        } else {
            if (fillMode !== Image.Stretch){
                console.warn("SvgImage: unsupported fillMode!")
            }

            sourceSize.width = Qt.binding(function() { return width })
            sourceSize.height = Qt.binding(function() { return height })
        }
    }
}
