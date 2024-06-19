#include <Cocoa/Cocoa.h>

float getMacOSScaleFactor() {
    // NSScreen* screen = [NSScreen mainScreen];
    // NSDictionary* description = [screen deviceDescription];
    // CGSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
    // CGSize screenSize = [screen frame].size;
    // CGFloat scale NSScreen.mainScreen.backingScaleFactor;
    NSScreen* screen = [NSScreen mainScreen];
    CGFloat scale = [screen backingScaleFactor];

    return scale;
}