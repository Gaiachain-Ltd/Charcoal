# qzxing

## Version

Code in qzxing folder is a dump of commit d3ef6881905022de9b058b8732019250fbd24542 from [QZxing repo](https://github.com/ftylitak/qzxing).

## Custom modifications

ImageHandler.cpp:
Use `QElapsedTimer` instead of `QTimer`.

ResultIO.cpp:
Add `namespace zxing` around `operator<<()`.

UPCAReader.h:
Change signature:
`Ref<Result> decodeRow(int rowNumber, Ref<BitArray> row, Range const& startGuardRange)`

To:
`Ref<Result> decodeRow(int rowNumber, Ref<BitArray> row, Range const& startGuardRange, zxing::DecodeHints hints = DecodeHints());`

And update the .cpp file.

EncodeHint.h:
Remove `int margin_;`
