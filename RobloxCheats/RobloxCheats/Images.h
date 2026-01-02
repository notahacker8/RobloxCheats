#import <Cocoa/Cocoa.h>
#import <Vision/Vision.h>

float COLORMAG(ESP_Color c) {
    return sqrt(c.r * c.r + c.g * c.g + c.b * c.b);
}

NSImage *NSImageFromPNGFile(NSString *filePath) {
    return [[NSImage alloc] initWithContentsOfFile:filePath];
}

NSImage *NSImageFromBytes(const uint8_t *bytes, size_t width, size_t height) {
    NSBitmapImageRep *rep = [[NSBitmapImageRep alloc]
        initWithBitmapDataPlanes:(unsigned char **)&bytes
                      pixelsWide:width
                      pixelsHigh:height
                   bitsPerSample:8
                 samplesPerPixel:4
                        hasAlpha:YES
                        isPlanar:NO
                  colorSpaceName:NSCalibratedRGBColorSpace
                    bytesPerRow:width * 4
                   bitsPerPixel:32];
    
    NSImage *image = [[NSImage alloc] initWithSize:NSMakeSize(width, height)];
    [image addRepresentation:rep];
    return image;
}

BOOL WritePNGFromNSImage(NSImage *image, NSString *filePath) {
    if (!image) return NO;
    NSBitmapImageRep *rep = [[NSBitmapImageRep alloc] initWithData:[image TIFFRepresentation]];
    NSData *pngData = [rep representationUsingType:NSBitmapImageFileTypePNG properties:@{}];
    return [pngData writeToFile:filePath atomically:YES];
}

NSImage *ScaleNSImage(NSImage *image, CGFloat factor) {
    if (!image || factor <= 0) return nil;

    NSSize newSize = NSMakeSize(image.size.width * factor, image.size.height * factor);
    NSImage *scaled = [[NSImage alloc] initWithSize:newSize];

    [scaled lockFocus];
    [image setSize:newSize];
    [[NSGraphicsContext currentContext] setImageInterpolation:NSImageInterpolationHigh];
    [image drawAtPoint:NSZeroPoint fromRect:NSMakeRect(0,0,newSize.width,newSize.height) operation:NSCompositingOperationCopy fraction:1.0];
    [scaled unlockFocus];

    return scaled;
}


NSImage *CropNSImage(NSImage *image, NSRect cropRect) {
    if (!image) return nil;

    // Ensure the image has at least one representation
    NSBitmapImageRep *rep = [[image representations] firstObject];
    if (!rep) return nil;

    // Create a bitmap context for the cropped image
    NSBitmapImageRep *croppedRep = [[NSBitmapImageRep alloc]
        initWithBitmapDataPlanes:NULL
                      pixelsWide:cropRect.size.width
                      pixelsHigh:cropRect.size.height
                   bitsPerSample:rep.bitsPerSample
                 samplesPerPixel:rep.samplesPerPixel
                        hasAlpha:rep.hasAlpha
                        isPlanar:NO
                  colorSpaceName:rep.colorSpaceName
                    bytesPerRow:0
                   bitsPerPixel:0];

    // Set graphics context to the new bitmap
    NSGraphicsContext *ctx = [NSGraphicsContext graphicsContextWithBitmapImageRep:croppedRep];
    [NSGraphicsContext saveGraphicsState];
    [NSGraphicsContext setCurrentContext:ctx];

    // Draw the specified portion of the original image into the new bitmap
    NSRect destRect = NSMakeRect(0, 0, cropRect.size.width, cropRect.size.height);
    [image drawInRect:destRect
             fromRect:cropRect
            operation:NSCompositingOperationCopy
             fraction:1.0
       respectFlipped:NO
                hints:nil];

    [NSGraphicsContext restoreGraphicsState];

    // Create the final NSImage
    NSImage *croppedImage = [[NSImage alloc] initWithSize:cropRect.size];
    [croppedImage addRepresentation:croppedRep];

    return croppedImage;
}

NSString *ReadTextFromNSImage(NSImage *image, double min_text_height) {
    if (!image) return nil;

    CGImageRef cgImage = [[NSBitmapImageRep imageRepWithData:[image TIFFRepresentation]] CGImage];
    VNRecognizeTextRequest *request = [[VNRecognizeTextRequest alloc] init];
    request.recognitionLevel = VNRequestTextRecognitionLevelFast;
    request.usesCPUOnly = YES;
    request.minimumTextHeight = min_text_height;
    request.usesLanguageCorrection = NO;

    VNImageRequestHandler *handler = [[VNImageRequestHandler alloc] initWithCGImage:cgImage options:@{}];

    NSError *error = nil;
    [handler performRequests:@[request] error:&error];
    if (error) return nil;

    NSMutableString *result = [NSMutableString string];
    for (VNRecognizedTextObservation *obs in request.results) {
        VNRecognizedText *best = [[obs topCandidates:1] firstObject];
        if (best) {
            [result appendString:best.string];
            [result appendString:@"\n"];
        }
    }
    return result;
}

ESP_Color AverageColorOfNSImage(NSImage *image) {
    ESP_Color color = {0, 0, 0, 0};
    if (!image) return color;

    NSBitmapImageRep *rep = [[NSBitmapImageRep alloc] initWithData:[image TIFFRepresentation]];
    if (!rep) return color;

    NSUInteger width = rep.pixelsWide;
    NSUInteger height = rep.pixelsHigh;
    NSInteger bytesPerPixel = 4;

    UInt8 *data = [rep bitmapData];
    if (!data) return color;

    float totalR = 0, totalG = 0, totalB = 0, totalA = 0;

    for (NSUInteger y = 0; y < height; y++) {
        for (NSUInteger x = 0; x < width; x++) {
            NSUInteger idx = (y * width + x) * bytesPerPixel;
            totalR += data[idx]     / 255.0f; // Red
            totalG += data[idx + 1] / 255.0f; // Green
            totalB += data[idx + 2] / 255.0f; // Blue
            totalA += data[idx + 3] / 255.0f; // Alpha
        }
    }

    NSUInteger pixelCount = width * height;
    color.r = totalR / pixelCount;
    color.g = totalG / pixelCount;
    color.b = totalB / pixelCount;
    color.a = totalA / pixelCount;

    return color;
}


CGFloat TotalNetGreen(NSImage *image) {
    if (!image) return 0.0;

    NSBitmapImageRep *rep = [[NSBitmapImageRep alloc] initWithData:[image TIFFRepresentation]];
    NSUInteger width = rep.pixelsWide;
    NSUInteger height = rep.pixelsHigh;
    NSInteger bytesPerPixel = 4;

    UInt8 *data = [rep bitmapData];
    if (!data) return 0.0;

    CGFloat total = 0.0;

    for (NSUInteger y = 0; y < height; y++) {
        for (NSUInteger x = 0; x < width; x++) {
            NSUInteger idx = (y * width + x) * bytesPerPixel;
            CGFloat r = data[idx] / 255.0;
            CGFloat g = data[idx + 1] / 255.0;
            CGFloat b = data[idx + 2] / 255.0;
            total += g - (r + b);
        }
    }
    return total;
}

